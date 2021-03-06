/*
 *                         OpenSplice DDS
 *
 *   This software and documentation are Copyright 2006 to 2010 PrismTech
 *   Limited and its licensees. All rights reserved. See file:
 *
 *                     $OSPL_HOME/LICENSE 
 *
 *   for full copyright notice and license terms. 
 *
 */
#include "u_user.h"
#include "u_participant.h"
#include "u_subscriber.h"
#include "u_dataReader.h"
#include "u_waitset.h"
#include "u_waitsetEvent.h"
#include "u_networkReader.h"
#include "v_networkReaderEntry.h"
#include "v_public.h"
#include "c_iterator.h"
#include "v_event.h"
#include "v_readerSample.h"
#include "v_dataReaderSample.h"
#include "v_public.h"
#include "v_topic.h"
#include "v_builtin.h"
#include "v_service.h"
#include "v_state.h"
#include "v_group.h"
#include "v_networkQueue.h"
#include "rtps.h"
#include "in_entity.h"
#include "in_config.h"
#include "os_defs.h"
#include "os_mutex.h"

#include "ddsi2.h"
#include "misc.h"
#include "mlv.h"

static c_bool terminate = FALSE;
static u_participant participant = NULL;
static v_service service = NULL;
static u_subscriber clientSubscriber = NULL;
static u_networkReader clientReader = NULL;
static v_networkReader vclientReader = NULL;
static v_networkQueue vclientQueue = NULL;
static c_ulong queueId = 0;
static os_threadId clientWriterThread;

static c_char* SERVICE_NAME = "ddsi2";
static c_char* SERVICE_URI = NULL;


os_mutex gluelock;
v_networkId myNetworkId;
v_gid ddsi2_participant_gid;
int minimal_sedp_endpoint_set_flag = 0;

struct builtin_datareader_set {
  u_subscriber subscriber;
  u_dataReader participant_dr;
  u_dataReader subscription_dr;
  u_dataReader publication_dr;
};

static u_result
attachAndMonitor(
    const u_participant participant,
    const struct builtin_datareader_set *drset);

static u_result
startMonitoring(
    const u_participant participant,
    const u_waitset waitset,
    const struct builtin_datareader_set *drset);

static c_bool
takeOne(
    c_object o,
    c_voidp arg);

static u_result
handleParticipant(
    u_dataReader dataReader,
    c_long dataOffset);

static u_result
handleSubscription(
    u_dataReader dataReader,
    c_long dataOffset,
    u_dataReader pDataReader,
    c_long pDataOffset);

static u_result
handlePublication(
    u_dataReader dataReader,
    c_long dataOffset,
    u_dataReader pDataReader,
    c_long pDataOffset);

static u_result
handleGroup(
    v_service service,
    v_group group);

static void
resolveOffset(
    v_entity e,
    c_voidp arg);

static void
resolveKernelService(
    v_entity entity,
    c_voidp args);

static void
resolveKernelReader(
    v_entity entity,
    c_voidp args);

static void
in_discoveryWatchSpliced(
    v_serviceStateKind spliceDaemonState,
    c_voidp usrData);

static c_bool
in_discoveryisBuiltinGroup(
    c_char* partition,
    c_char* topic);

static c_bool
in_discoveryisBuiltinGroup(
    c_char* partition,
    c_char* topic)
{
    c_bool result = FALSE;
    assert(partition);
    assert(topic);

    if(strcmp(partition, V_BUILTIN_PARTITION) == 0){
        if( (strcmp(topic, V_PARTICIPANTINFO_NAME) == 0) ||
            (strcmp(topic, V_TOPICINFO_NAME) == 0) ||
            (strcmp(topic, V_PUBLICATIONINFO_NAME) == 0) ||
            (strcmp(topic, V_SUBSCRIPTIONINFO_NAME) == 0))
        {
            result = TRUE;
        }
    }
    return result;
}

static void*
in_discoveryClientWriterMonitor(
    void* arg)
{
    c_bool result, sendTo, more;
    v_message message;
    c_ulong sequenceNumber, priority;
    v_gid sender, receiver;
    c_time sendBefore;
    v_networkReaderEntry entry;
    os_time sleepTime;
    in_writer writer;
    os_boolean proceed = TRUE;
    v_networkReaderWaitResult nrwr;

    if(vclientQueue){
        sleepTime.tv_sec = 0;
        sleepTime.tv_nsec = 50 * 1000 * 1000;

        while(!terminate && proceed){
            nrwr = v_networkReaderWait(vclientReader, queueId, &vclientQueue);

            if((nrwr & V_WAITRESULT_MSGWAITING) == V_WAITRESULT_MSGWAITING){
                more = TRUE;

                while(more){
                    result = v_networkQueueTakeFirst(
                        vclientQueue, &message, &entry, &sequenceNumber, &sender,
                        &sendTo, &receiver, &sendBefore, &priority, &more);

                    if(result){
		        os_mutexLock (&gluelock);
                        writer = in_writerLookup(&sender);

                        if(writer){
                            if(v_nodeState(message) == L_WRITE){
                                proceed = in_writerWrite(writer, message);
                            } else if(v_nodeState(message) == L_DISPOSED){
                                proceed = in_writerDispose(writer, message);
                            } else if(v_nodeState(message) == L_UNREGISTER){
                                proceed = in_writerUnregister(writer, message);
                            }
                        }
			os_mutexUnlock (&gluelock);
                    }
                }
            } else if((nrwr & V_WAITRESULT_TRIGGERED) == V_WAITRESULT_TRIGGERED){
                in_printf(IN_LEVEL_FINER, "clientReader triggered...\n");
                proceed = FALSE;
            }
        }
    }
    return NULL;
}

static void
resolveKernelService(
    v_entity entity,
    c_voidp args)
{
    if(args){}
    service = (v_service)entity;
    return;
}

static void
resolveKernelReader(
    v_entity entity,
    c_voidp args)
{
    if(args){}

    vclientReader = (v_networkReader)entity;
    assert(vclientReader);
    vclientQueue = ((v_networkReader)entity)->defaultQueue;
    assert(vclientQueue);

    return;
}

static void
in_discoveryWatchSpliced(
    v_serviceStateKind spliceDaemonState,
    c_voidp usrData)
{
    switch(spliceDaemonState){
    case STATE_TERMINATING:
    case STATE_TERMINATED:
    case STATE_DIED:
        in_printf(IN_LEVEL_INFO, "Splicedaemon is terminating and so am I...\n");
        terminate = TRUE;
        u_serviceChangeState(u_service(participant), STATE_TERMINATING);
	os_mutexLock (&gluelock);
	if (vclientReader)
	  v_networkReaderTrigger(vclientReader, queueId);
	os_mutexUnlock (&gluelock);
        break;
    default:
        break;
    }
}

static void
resolveOffset(
    v_entity e,
    c_voidp arg)
{
    c_long* offset;

    offset = (c_long*)arg;
    *offset = v_topic(e)->dataField->offset;

    return;
}

static u_result
attachAndMonitor(
    const u_participant participant,
    const struct builtin_datareader_set *drset)
{
    u_waitset waitset;
    u_dataReader dataReader;
    c_iter readers;
    u_result result;
    c_long i, length;

    result = U_RESULT_INTERNAL_ERROR;
    readers = NULL;
    length = 0;
    /*Create waitset.*/
    waitset = u_waitsetNew(participant);

    if(waitset){
        /*Set event mask of the waitset.*/
        result = u_waitsetSetEventMask(waitset, V_EVENT_DATA_AVAILABLE | V_EVENT_NEW_GROUP | V_EVENT_SERVICESTATE_CHANGED);

        if(result == U_RESULT_OK){
            result = u_dispatcherSetEventMask(
                    (u_dispatcher)participant, V_EVENT_NEW_GROUP | V_EVENT_SERVICESTATE_CHANGED);

            if(result == U_RESULT_OK){
                v_serviceFillNewGroups(service);
                result = u_waitsetAttach(
                        waitset, (u_entity)participant,
                        (u_entity)participant);

                if(result != U_RESULT_OK){
                    in_printf(IN_LEVEL_SEVERE, "Could not attach datareader to waitset.\n");
                }
            } else {
                in_printf(IN_LEVEL_SEVERE, "Could not set event mask of participant.");
            }

            if(result == U_RESULT_OK){
                readers     = c_iterNew(drset->participant_dr);
                readers     = c_iterInsert(readers, drset->publication_dr);
                readers     = c_iterInsert(readers, drset->subscription_dr);

                result     = U_RESULT_OK;
                length     = c_iterLength(readers);

                for(i=0; i<length && (result == U_RESULT_OK); i++){
                    dataReader = (u_dataReader)(c_iterObject(readers, i));

                    /*Set event mask of the datareader to trigger on available data.*/
                    result = u_dispatcherSetEventMask(
                                (u_dispatcher)dataReader, V_EVENT_DATA_AVAILABLE);

                    if(result == U_RESULT_OK){
                        /*Attach reader to the waitset.*/
                        result = u_waitsetAttach(
                                waitset, (u_entity)dataReader, (u_entity)dataReader);

                        if(result != U_RESULT_OK){
                            in_printf(IN_LEVEL_SEVERE, "Could not attach datareader to waitset.\n");
                        }
                    } else {
                        in_printf(IN_LEVEL_SEVERE, "Could not set event mask of datareader.\n");
                    }
                }
            }
        } else {
            in_printf(IN_LEVEL_SEVERE, "Could not set event mask of waitset.\n");
        }


        if(result == U_RESULT_OK){
            /*Start monitoring the creation/deletion of entities.*/
            result = startMonitoring(participant, waitset, drset);
        }
        u_waitsetDetach(waitset, u_entity(participant));

        if(readers){
            /*Detach all datareaders from the waitset.*/
            for(i=0; i<length; i++){
                u_waitsetDetach(waitset, (u_entity)(c_iterObject(readers, i)));
            }
            c_iterFree(readers);
        }
        /*Delete the waitset.*/
        result = u_waitsetFree(waitset);

        if(result != U_RESULT_OK){
            in_printf(IN_LEVEL_SEVERE, "Deletion of waitset failed.\n");
        }
    } else {
        in_printf(IN_LEVEL_SEVERE, "Could not create waitset.\n");
    }

    return result;
}

static u_result
startMonitoring(
    const u_participant participant,
    const u_waitset waitset,
    const struct builtin_datareader_set *drset)
{
    c_iter events, topics;
    u_waitsetEvent event;
    c_time timeout;
    os_uint32 reportInterval;
    v_gid participantGid, publicationGid, subscriptionGid, gid;
    u_result result;
    u_dataReader dataReader;
    u_topic topic;
    c_iter vgroups;
    v_group vgroup;
    v_duration duration;
    c_long participantOffset, publicationOffset, subscriptionOffset;
    os_threadAttr attr;
    os_result osr;

    /*Resolve unique identifications of readers*/
    participantGid  = u_entityGid((u_entity)drset->participant_dr);
    publicationGid  = u_entityGid((u_entity)drset->publication_dr);
    subscriptionGid = u_entityGid((u_entity)drset->subscription_dr);

    /*Resolve topics to find offsets in the data. The offsets are used later on*/
    duration.seconds = 0;
    duration.nanoseconds = 0;

    topics = u_participantFindTopic(participant, V_PARTICIPANTINFO_NAME, duration);
    topic  = c_iterTakeFirst(topics);

    if(topic){
        result = u_entityAction(u_entity(topic), resolveOffset, &participantOffset);
    } else {
        result = U_RESULT_INTERNAL_ERROR;
        in_printf(IN_LEVEL_SEVERE, "Could not resolve participant info offset.\n");
    }
    c_iterFree(topics);

    if(result == U_RESULT_OK){
        topics = u_participantFindTopic(participant, V_PUBLICATIONINFO_NAME, duration);
        topic  = c_iterTakeFirst(topics);

        if(topic){
            result = u_entityAction(u_entity(topic), resolveOffset, &publicationOffset);
        } else {
            result = U_RESULT_INTERNAL_ERROR;
            in_printf(IN_LEVEL_SEVERE, "Could not resolve publication info offset.\n");
        }
        c_iterFree(topics);
    }

    if(result == U_RESULT_OK){
        topics = u_participantFindTopic(participant, V_SUBSCRIPTIONINFO_NAME, duration);
        topic  = c_iterTakeFirst(topics);

        if(topic){
            result = u_entityAction(u_entity(topic), resolveOffset, &subscriptionOffset);
        } else {
            result = U_RESULT_INTERNAL_ERROR;
            in_printf(IN_LEVEL_SEVERE, "Could not resolve subscription info offset.\n");
        }
        c_iterFree(topics);
    }

    if(result == U_RESULT_OK){
        timeout.seconds     = 0;
        timeout.nanoseconds = 100 * 1000 * 1000; /*100 ms*/

        in_printf(IN_LEVEL_FINE, "Collecting initial entities...\n");
        result = handleParticipant(drset->participant_dr, participantOffset);

        if(result == U_RESULT_OK){
            result = handlePublication(drset->publication_dr, publicationOffset,
                    drset->participant_dr, participantOffset);

            if(result == U_RESULT_OK){
                result = handleSubscription(drset->subscription_dr, subscriptionOffset,
                        drset->participant_dr, participantOffset);

                if(result == U_RESULT_OK){
                    vgroups = v_serviceTakeNewGroups(service);
                    vgroup = (v_group)c_iterTakeFirst(vgroups);

                    while(vgroup && result == U_RESULT_OK){
                        result = handleGroup(service, vgroup);
                        c_free(vgroup);
                        vgroup = (v_group)c_iterTakeFirst(vgroups);
                    }
                    c_iterFree(vgroups);

                    if(result == U_RESULT_OK){
                        in_printf(IN_LEVEL_FINE, "Waiting for entities to be created/deleted...\n");
                    } else {
                        in_printf(IN_LEVEL_SEVERE, "Could not collect initial groups...\n");
                    }
                } else {
                    in_printf(IN_LEVEL_SEVERE, "Could not collect initial subscriptions...\n");
                }
            } else {
                in_printf(IN_LEVEL_SEVERE, "Could not collect initial publications...\n");
            }
        } else {
            in_printf(IN_LEVEL_SEVERE, "Could not collect initial participants...\n");
        }
    }

    osr = os_threadAttrInit(&attr);

    if(osr == os_resultSuccess){
        osr = os_threadCreate(&clientWriterThread,
                "clientWriterMonitor", &attr,
                in_discoveryClientWriterMonitor, NULL);

        if(osr != os_resultSuccess){
            result = U_RESULT_INTERNAL_ERROR;
        }
    } else {
        result = U_RESULT_INTERNAL_ERROR;
    }
    reportInterval = 0;

    while(result == U_RESULT_OK && !terminate){
        events = NULL;
        /*Wait for events to occur*/
        result = u_waitsetTimedWaitEvents(waitset, timeout, &events);

        if(result == U_RESULT_OK){
            event = (u_waitsetEvent)(c_iterTakeFirst(events));

            while(event){
                if(((event->events) & V_EVENT_DATA_AVAILABLE) ==
                    V_EVENT_DATA_AVAILABLE)
                {
                    if(event->entity){
                        dataReader = (u_dataReader)event->entity;
                        gid        = u_entityGid(event->entity);

                        if(v_gidCompare(gid, participantGid) == C_EQ){
                            result = handleParticipant(
                                    drset->participant_dr, participantOffset);
                        } else if(v_gidCompare(gid, subscriptionGid) == C_EQ){
                            result = handleSubscription(
                                    drset->subscription_dr, subscriptionOffset,
                                    drset->participant_dr, participantOffset);
                        } else if(v_gidCompare(gid, publicationGid) == C_EQ){
                            result = handlePublication(
                                    drset->publication_dr, publicationOffset,
                                    drset->participant_dr, participantOffset);
                        } else {
                            in_printf(IN_LEVEL_SEVERE,
                                    "This is impossible...at least in my understanding of the world.\n");
                            result = U_RESULT_INTERNAL_ERROR;
                        }
                    } else {
                        in_printf(IN_LEVEL_WARNING, "DATA_AVAILABLE (%d) but no entity.\n",
                                event->events);
                    }
                } else if(((event->events) & V_EVENT_NEW_GROUP) ==
                    V_EVENT_NEW_GROUP)
                {
                    vgroups = v_serviceTakeNewGroups(service);
                    vgroup = (v_group)c_iterTakeFirst(vgroups);

                    while(vgroup && result == U_RESULT_OK){
                        result = handleGroup(service, vgroup);
                        c_free(vgroup);
                        vgroup = (v_group)c_iterTakeFirst(vgroups);
                    }
                    c_iterFree(vgroups);
                } else {
                    in_printf(IN_LEVEL_SEVERE, "Received unexpected event %d.\n", event->events);
                    result = U_RESULT_INTERNAL_ERROR;
                }
                u_waitsetEventFree(event);
                event = (u_waitsetEvent)(c_iterTakeFirst(events));
            }
        } else if(result == U_RESULT_DETACHING){
            in_printf(IN_LEVEL_INFO, "Starting termination now...\n");
        } else if(result == U_RESULT_TIMEOUT){
            result = U_RESULT_OK;
        } else {
            in_printf(IN_LEVEL_SEVERE, "Waitset wait failed.\n");
        }
        if(events){/* events may be null if waitset was deleted */
            c_iterFree(events);
        }
        reportInterval++;

        if(reportInterval >= 5){
            /*reportEntities();*/
            reportInterval = 0;
        }
    }
    return result;
}

static c_bool
takeOne(
    c_object o,
    c_voidp arg)
{
    v_readerSample s;
    v_readerSample *sample;
    c_bool result;

    s      = (v_readerSample)o;
    sample = (v_readerSample *)arg;

    if (s != NULL) {
        result = TRUE;
        if (v_stateTest(s->sampleState, L_VALIDDATA)) {
            *sample = c_keep(s);
            result = FALSE;
        } else {
            *sample = NULL;
        }
    } else { /* last sample */
        result = FALSE;
    }

    return result;
}

static u_result
handleParticipant(
    u_dataReader dataReader,
    c_long dataOffset)
{
    v_dataReaderSample sample;
    u_result result;
    v_state state;
    v_message msg;
    struct v_participantInfo *data;
    in_participant participant;

    sample = NULL;
    result = u_dataReaderTake(dataReader, takeOne, &sample);

    while(sample && (result == U_RESULT_OK)){
        state = v_readerSample(sample)->sampleState;
        msg   = v_dataReaderSampleMessage(sample);
        data  = (struct v_participantInfo *)(C_DISPLACE(msg, dataOffset));

	os_mutexLock (&gluelock);
	if(v_stateTest(state, L_DISPOSED)){
	  participant = in_participantLookup(&(data->key));
	    
	  if(participant){
	    in_participantFree(participant, NULL);
	  }
	} else {
	  in_participantNew(data);
	}
	os_mutexUnlock (&gluelock);
        c_free(sample);
        sample = NULL;
        result = u_dataReaderTake(dataReader, takeOne, &sample);
    }
    return result;
}

static u_result
handleSubscription(
    u_dataReader dataReader,
    c_long dataOffset,
    u_dataReader pDataReader,
    c_long pDataOffset)
{
    v_dataReaderSample sample;
    u_result result;
    v_state state;
    v_message msg;
    in_reader reader;
    in_participant participant;
    struct v_subscriptionInfo *data;

    sample = NULL;
    result = u_dataReaderTake(dataReader, takeOne, &sample);

    while(sample && (result == U_RESULT_OK)){
        state = v_readerSample(sample)->sampleState;
        msg   = v_dataReaderSampleMessage(sample);
        data  = (struct v_subscriptionInfo *)(C_DISPLACE(msg, dataOffset));
	os_mutexLock (&gluelock);
	if(v_stateTest(state, L_DISPOSED)){
	  reader = in_readerLookup(&data->key);
	  
	  if(reader){
	    in_readerFree(reader, NULL);
	  }
	} else {
	  participant = in_participantLookup(&(data->participant_key));
	  
	  if(!participant){
	    result = handleParticipant(pDataReader, pDataOffset);
	    
	    if(result == U_RESULT_OK){
	      participant = in_participantLookup(&(data->participant_key));
	    }
	  }
	  if(participant){
	    in_readerNew(participant, data);
	  } else {
	    in_printf (IN_LEVEL_SEVERE, "handleSubscription: participant not found\n");
	    result = U_RESULT_INTERNAL_ERROR;
	    /*abort();*/
	  }
	}
	os_mutexUnlock (&gluelock);
        c_free(sample);
        sample = NULL;
        result = u_dataReaderTake(dataReader, takeOne, &sample);
    }
    return result;
}

static u_result
handlePublication(
    u_dataReader dataReader,
    c_long dataOffset,
    u_dataReader pDataReader,
    c_long pDataOffset)
{
    v_dataReaderSample sample;
    u_result result;
    v_state state;
    v_message msg;
    struct v_publicationInfo *data;
    in_writer writer;
    in_participant participant;

    sample = NULL;
    result = u_dataReaderTake(dataReader, takeOne, &sample);

    while(sample && (result == U_RESULT_OK)){
        state = v_readerSample(sample)->sampleState;
        msg   = v_dataReaderSampleMessage(sample);
        data  = (struct v_publicationInfo *)(C_DISPLACE(msg, dataOffset));
	os_mutexLock (&gluelock);
	if(v_stateTest(state, L_DISPOSED)){
	  writer = in_writerLookup(&data->key);

	  if(writer){
	    in_writerFree(writer, NULL);
	  } else {
	    abort();
	  }
	} else {
	  participant = in_participantLookup(&(data->participant_key));

	  if(!participant){
	    result = handleParticipant(pDataReader, pDataOffset);

	    if(result == U_RESULT_OK){
	      participant = in_participantLookup(&(data->participant_key));
	    }
	  }
	  if(participant){
	    in_writerNew(participant, data);
	  } else {
	    in_printf (IN_LEVEL_SEVERE, "handlePublication: participant not found\n");
	    result = U_RESULT_INTERNAL_ERROR;
	    /*abort();*/
	  }
	}
	os_mutexUnlock (&gluelock);
        c_free(sample);
        sample = NULL;
        result = u_dataReaderTake(dataReader, takeOne, &sample);
    }
    return result;
}

static u_result
handleGroup(
    v_service service,
    v_group group)
{
    v_networkReaderEntry entry;
    in_printf(IN_LEVEL_FINE, "Found new group '%s.%s'; adding networkReaderEntry...\n",
	      v_entity(group->partition)->name,
	      v_entity(group->topic)->name);

    entry = v_networkReaderEntryNew(
	    vclientReader, group,
	    v_publicGid(v_public(service)).systemId,
	    1, 0);

    if (group->topic->qos->durability.kind >= V_DURABILITY_TRANSIENT)
    {
      /* For transient topics, DDSI readers are spontaneously
	 generated to ensure data will indeed arrive -- FIXME:
	 currently no provision is made to ensure no "early"
	 publications are lost while DDSI discovery is still digesting
	 these readers.

	 For convenience, we use the regular DDS<->DDSI mapping to
	 handle these ficitious readers, and we pretend these
	 ficitious readers are owned by the DDSI service
	 participant. That one has been created, and as luck has it,
	 the participants are discovered before the groups are. So we
	 just look it up. */
      v_builtinTopicKey pkey;
      in_participant p;
      in_printf(IN_LEVEL_FINE, "Group is transient - creating DDSI data reader...\n");
      
      os_mutexLock (&gluelock);
      pkey = u_entityGid ((u_entity) participant);
      if ((p = in_participantLookup (&pkey)) == NULL)
	in_printf (IN_LEVEL_SEVERE, "handleGroup: participant lookup of self failed, transient data may not work\n");
      else
      {
	if (!in_fictitiousTransientReaderNew (p, group))
	{
	  in_printf (IN_LEVEL_SEVERE, "handleGroup: creation of fictitious transient data reader failed, transient data may not work\n");
	}
      }
      os_mutexUnlock (&gluelock);
    }
    
    v_networkReaderEntryNotifyConnected(entry, SERVICE_NAME);
    v_networkReaderRemoteActivityDetected(vclientReader);
    return U_RESULT_OK;
}

/* NOTE: Blatantly stolen from the "native" networking implementation
   (src/services/networking/code/nw_controller.c). Methinks these
   should be guaranteed unique, not hopefully unique. Surely one could
   just a "service instance id" or somesuch for this purpose ...

   NOTE 2: And fixed after stealing ... Native networking uses (0 <=
   id < (1e9-1)), we use one in (1e-9 <= id' < (2e9-1)). So there
   never should (1) be an id collision and (2) an accidental use of
   LOCAL by DDSI2. */
static v_networkId
getNetworkId(void)
{
    os_time time;

    /* NOTE: for now, let network ID be a "random"-number. This number has to be
     *       retrieved from the network/os layer. */

    assert (V_NETWORKID_LOCAL == 0x0);
    time = os_timeGet();
    return time.tv_nsec + 1000000000;
}

static void destroy_builtin_readers (struct builtin_datareader_set *drset)
{
  if (drset->publication_dr)
    u_dataReaderFree (drset->publication_dr);
  if (drset->subscription_dr)
    u_dataReaderFree (drset->subscription_dr);
  if (drset->participant_dr)
    u_dataReaderFree (drset->participant_dr);
  if (drset->subscriber)
    u_subscriberFree (drset->subscriber);
}

static u_result create_builtin_readers (struct builtin_datareader_set *drset, u_participant p)
{
  v_subscriberQos sQos = NULL;
  v_readerQos rdQos = NULL;
  v_gid gid;
  c_value ps[1];

  drset->subscriber = NULL;
  drset->participant_dr = NULL;
  drset->subscription_dr = NULL;
  drset->publication_dr = NULL;
  
  if ((sQos = u_subscriberQosNew (NULL)) == NULL)
    goto fail;
  sQos->presentation.access_scope = V_PRESENTATION_TOPIC;
  if ((sQos->partition = os_strdup ("__BUILT-IN PARTITION__")) == NULL)
    goto fail;

  if ((rdQos = u_readerQosNew (NULL)) == NULL)
    goto fail;
  rdQos->durability.kind = V_DURABILITY_TRANSIENT;
  rdQos->reliability.kind = V_RELIABILITY_RELIABLE;
  rdQos->history.kind = V_HISTORY_KEEPLAST;
  rdQos->history.depth = 1;

  if ((drset->subscriber = u_subscriberNew (p, "DDSI2BuiltinSubscriber", sQos, TRUE)) == NULL)
    goto fail;
  gid = u_entityGid ((u_entity) drset->subscriber);
  ps[0].kind = V_ULONG;
  ps[0].is.ULong = gid.systemId;
  in_printf (IN_LEVEL_INFO, "create_builtin_readers: systemId = %lx\n", (unsigned long) ps[0].is.ULong);
  
  if ((drset->participant_dr = u_subscriberCreateDataReader (drset->subscriber, "DCPSParticipantReader", "select * from DCPSParticipant where key.systemId = %0", ps, rdQos, TRUE)) == NULL)
    goto fail;
  if ((drset->subscription_dr = u_subscriberCreateDataReader (drset->subscriber, "DCPSSubscriptionReader", "select * from DCPSSubscription where key.systemId = %0", ps, rdQos, TRUE)) == NULL)
    goto fail;
  if ((drset->publication_dr = u_subscriberCreateDataReader (drset->subscriber, "DCPSPublicationReader", "select * from DCPSPublication where key.systemId = %0", ps, rdQos, TRUE)) == NULL)
    goto fail;
  u_readerQosFree (rdQos);
  u_subscriberQosFree (sQos);
  return U_RESULT_OK;

 fail:
  destroy_builtin_readers (drset);
  if (rdQos)
    u_readerQosFree (rdQos);
  if (sQos)
    u_subscriberQosFree (sQos);
  return U_RESULT_INTERNAL_ERROR;
}

static int enable_undoc_feature (C_STRUCT (in_config) const *config, const char *name)
{
  int found = 0;
  if (config->enableUndocFeatures)
  {
    char *options = os_strdup (config->enableUndocFeatures);
    char *pos = options, *o;
    while ((o = ddsi2_strsep (&pos, ",")) != NULL)
    {
      if (os_strcasecmp (o, name) == 0)
      {
	found = 1;
	break;
      }
    }
    os_free (options);
  }
  return found;
}

int
main(
    int argc,
    char* argv[])
{
    int result = 0;
    v_participantQos participantQos;
    u_result uresult;
    os_boolean success;
    in_config config;
    v_subscriberQos subscriberQos;
    c_time resolution;
    c_base base;

    /* Necessary to initialize the user layer. Do this just once per process.*/
    mlv_init ();
    uresult = u_userInitialise();
    mlv_setforreal (1);
    
    if(uresult == U_RESULT_OK){
        /* Allocate default participant qos*/
        participantQos = u_participantQosNew(NULL);

	{
	  os_mutexAttr mattr;
	  os_mutexAttrInit (&mattr);
	  mattr.scopeAttr = OS_SCOPE_PRIVATE;
	  os_mutexInit (&gluelock, &mattr);
	}

        if(participantQos){
            if(argc > 1){
                SERVICE_NAME = argv[1];
            }
            if(argc > 2){
                SERVICE_URI = argv[2];
            }
            /*create participant*/
            participant = u_participant(u_serviceNew(
                                SERVICE_URI, 0, SERVICE_NAME,
                                NULL,
                                U_SERVICE_NETWORKING,
                                (v_qos)participantQos));
	    ddsi2_participant_gid = u_entityGid (u_entity (participant));
	      
            if(participant){
                /*Notify kernel that I am initializing. */
                u_serviceChangeState(u_service(participant),STATE_INITIALISING);

                /*Start monitoring the splicedaemon state. I need to terminate if he says so*/
                u_serviceWatchSpliceDaemon(
                        u_service(participant),
                        in_discoveryWatchSpliced,
                        &terminate);

                config = in_configNew(participant, SERVICE_NAME);

                if (config) {
		    unsigned rtps_flags = 0;
		    unsigned rtps_dbgflags;

		    rtps_dbgflags = rtps_get_debug_flags ();
		    if (config->tracingVerbosityLevel <= IN_LEVEL_FINEST)
		      rtps_dbgflags |= DBGFLAG_TRACING;
		    if (config->tracingVerbosityLevel <= IN_LEVEL_FINE)
		      rtps_dbgflags |= DBGFLAG_INFO;
		    if (config->tracingVerbosityLevel <= IN_LEVEL_SEVERE)
		      rtps_dbgflags |= DBGFLAG_TROUBLE;
		    rtps_set_debug_flags (rtps_dbgflags);

		    if(enable_undoc_feature (config, "minimal-sedp-endpoint-set"))
		    {
		      minimal_sedp_endpoint_set_flag = 1;
		    }

		    myNetworkId = getNetworkId ();
		    in_printf (IN_LEVEL_INFO, "CONFIG networkId = 0x%lx\n", (unsigned long) myNetworkId);
		    in_printf (IN_LEVEL_INFO, "CONFIG minimal-sedp-endpoint-set = %s\n", minimal_sedp_endpoint_set_flag ? "true" : "false");
		    
                    u_entityAction(u_entity(participant), resolveKernelService, NULL);
                    base = c_getBase(service);
                    rtps_set_trace_function(in_printfRtps);
		    if (config->allowMulticast)
		      rtps_flags |= RTPS_USE_MCAST;
		    if (config->enableMulticastLoopback)
		      rtps_flags |= RTPS_USE_MCAST_LOOPBACK;
		    if (config->coexistWithNativeNetworking)
		      rtps_flags |= RTPS_IGNORE_OWN_VENDOR;
		    if (enable_undoc_feature (config, "aggressive-whc"))
		      rtps_flags |= RTPS_AGGRESSIVE_KEEP_LAST1_WHC;
		    if (enable_undoc_feature (config, "conservative-builtin-reader-startup"))
		      rtps_flags |= RTPS_CONSERVATIVE_BUILTIN_READER_STARTUP;
		    if (enable_undoc_feature (config, "noqueue-heartbeat-messages"))
		      rtps_flags |= RTPS_NOQUEUE_HEARTBEAT_MESSAGES;
		    rtps_init (base, config->domainId, config->participantIndex,
			       rtps_flags, config->networkAddressString, config->peers);
                    
                    /* Initialize entity administration. */
                    success = in_entityAdminInit(participant);

                    if(success){
                        /*Create subscriber to receive client writers' messages.*/
                        subscriberQos = u_subscriberQosNew(NULL);
                        os_free(subscriberQos->partition);
                        subscriberQos->partition = NULL;

                        clientSubscriber = u_subscriberNew(
                                participant,
                                "clientSubscriber",
                                subscriberQos,
                                TRUE);

                        if(clientSubscriber){
                            /*Create networkReader to be able to receive client writers' messages.*/
                            clientReader = u_networkReaderNew(
                                    clientSubscriber,
                                    "clientReader",
                                    NULL,
                                    TRUE);

                            if(clientReader){
                                resolution.seconds = 0;
                                resolution.nanoseconds = 10 * 1000 * 1000; /*10 ms*/

                                /*Create network queue*/
                                uresult = u_networkReaderCreateQueue(
                                        clientReader,
                                        1000, 0, FALSE, FALSE,
                                        resolution,
                                        TRUE, &queueId,
                                        "DDSi");

                                if(uresult == U_RESULT_OK){
				  struct builtin_datareader_set drset;
				  u_entityAction(u_entity(clientReader), resolveKernelReader, NULL);

				  uresult = create_builtin_readers (&drset, participant);
				  if (uresult == U_RESULT_OK)
				  {
				    u_serviceChangeState(u_service(participant),STATE_OPERATIONAL);
				    uresult = attachAndMonitor(participant, &drset);

				    if((uresult != U_RESULT_OK) &&
				       (uresult != U_RESULT_DETACHING))
				    {
				      in_printf(IN_LEVEL_WARNING, "Abnormal termination...\n");
				      result = -1;
				    } else {
				      in_printf(IN_LEVEL_INFO, "Deleting entities...\n");
				    }
				    destroy_builtin_readers (&drset);
				  } else {
				    in_printf(IN_LEVEL_SEVERE, "Could not create subscription + readers for builtin topics.\n");
				    result = -1;
				  }
				  terminate = TRUE;
				  v_networkReaderTrigger(vclientReader, queueId);
				  os_threadWaitExit(clientWriterThread, NULL);
                                } else {
                                    in_printf(IN_LEVEL_SEVERE, "Could not create networkQueue.\n");
                                    result = -1;
                                }
                                /*Clean up networkReader*/
				os_mutexLock (&gluelock);
                                u_networkReaderFree(clientReader);
				clientReader = NULL;
				vclientReader = NULL;
				os_mutexUnlock (&gluelock);
                            } else {
                                in_printf(IN_LEVEL_SEVERE, "Could not create networkReader.\n");
                                result = -1;
                            }
                            /*Clean up subscriber*/
                            u_subscriberFree(clientSubscriber);
                        } else {
                            in_printf(IN_LEVEL_SEVERE, "Could not create subscriber.\n");
                            result = -1;
                        }

                        /*Clean up entity administration*/
                        in_entityAdminDestroy();
                    } else {
                        in_printf(IN_LEVEL_SEVERE, "Could not initialize entity adminstration.\n");
                        result = -1;
                    }

		    /* RTPS layer now defines types, cleanup before detaching */
                    rtps_term();

                    /*Notify kernel that I've terminated*/
                    u_serviceChangeState(u_service(participant),STATE_TERMINATED);
                    /*Delete participant*/
                    uresult = u_serviceFree(u_service(participant));

                    if(uresult != U_RESULT_OK){
                        in_printf(IN_LEVEL_SEVERE, "Deletion of participant failed.\n");
                        result = -1;
                    }
                    in_configFree(config);
                } else {
                    in_printf(IN_LEVEL_SEVERE, "Initialization of configuration failed.\n");
                    result = -1;
                }
            } else {
                in_printf(IN_LEVEL_SEVERE, "Could not create participant.\n");
                result = -1;
            }
	    u_participantQosFree (participantQos);
        } else {
            in_printf(IN_LEVEL_SEVERE, "Could not allocate participantQos.\n");
            result = -1;
        }
	os_mutexDestroy (&gluelock);
        /* Detach user layer */
	mlv_setforreal (0);
        uresult = u_userDetach();
	mlv_fini ();

        if(uresult != U_RESULT_OK){
            in_printf(IN_LEVEL_SEVERE, "Detachment of user layer failed.\n");
            result = -1;
        }
    } else {
        in_printf(IN_LEVEL_SEVERE, "Initialization of user layer failed.\n");
        result = -1;
    }
    in_printf(IN_LEVEL_INFO, "Exiting...\n");
    return result;
}
