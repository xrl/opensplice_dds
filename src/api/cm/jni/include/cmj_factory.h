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

 /* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class org_opensplice_cm_com_JniCommunicator */

#ifndef _Included_org_opensplice_cm_com_JniCommunicator
#define _Included_org_opensplice_cm_com_JniCommunicator
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniInitialise
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniInitialise
  (JNIEnv *, jobject);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniDetach
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniDetach
  (JNIEnv *, jobject);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniEntityFree
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniEntityFree
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniEntityStatus
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniEntityStatus
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniEntityQoS
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniEntityQoS
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniEntitySetQoS
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniEntitySetQoS
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniGetOwnedEntities
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniGetOwnedEntities
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniGetDependantEntities
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniGetDependantEntities
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniEntityResetStatistics
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniEntityResetStatistics
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniEntityGetStatistics
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniEntityGetStatistics
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniEntityEnable
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniEntityEnable
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniCreateParticipant
 * Signature: (Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniCreateParticipant
  (JNIEnv *, jobject, jstring, jint, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniParticipantAllParticipants
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniParticipantAllParticipants
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniParticipantAllTopics
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniParticipantAllTopics
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniParticipantAllDomains
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniParticipantAllDomains
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniParticipantFindTopic
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniParticipantFindTopic
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniRegisterType
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniRegisterType
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniPublisherNew
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniPublisherNew
  (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniSubscriberNew
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniSubscriberNew
  (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniDomainNew
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniDomainNew
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniTopicNew
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniTopicNew
  (JNIEnv *, jobject, jstring, jstring, jstring, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniGetTopicDataType
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniGetTopicDataType
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniGetServiceState
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniGetServiceState
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniReaderSnapshotNew
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniReaderSnapshotNew
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniReaderDataType
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniReaderDataType
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniReaderTake
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniReaderTake
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniReaderRead
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniReaderRead
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniReaderReadNext
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniReaderReadNext
  (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniQueryNew
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniQueryNew
  (JNIEnv *, jobject, jstring, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniSnapshotFree
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniSnapshotFree
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniSnapshotRead
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniSnapshotRead
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniSnapshotTake
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniSnapshotTake
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWriterSnapshotNew
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWriterSnapshotNew
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWriterDataType
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWriterDataType
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWriterWrite
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWriterWrite
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWriterDispose
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWriterDispose
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWriterWriteDispose
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWriterWriteDispose
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWriterRegister
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWriterRegister
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWriterUnregister
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWriterUnregister
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniPublisherPublish
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniPublisherPublish
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniCreateWriter
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniCreateWriter
  (JNIEnv *, jobject, jstring, jstring, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniSubscriberSubscribe
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniSubscriberSubscribe
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniCreateDataReader
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniCreateDataReader
  (JNIEnv *, jobject, jstring, jstring, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWaitsetNew
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWaitsetNew
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWaitsetAttach
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWaitsetAttach
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWaitsetDetach
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWaitsetDetach
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWaitsetWait
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWaitsetWait
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWaitsetTimedWait
 * Signature: (Ljava/lang/String;II)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWaitsetTimedWait
  (JNIEnv *, jobject, jstring, jint, jint);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWaitsetGetEventMask
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWaitsetGetEventMask
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniWaitsetSetEventMask
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniWaitsetSetEventMask
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     org_opensplice_cm_com_JniCommunicator
 * Method:    jniDataReaderWaitForHistoricalData
 * Signature: (Ljava/lang/String;II)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_opensplice_cm_com_JniCommunicator_jniDataReaderWaitForHistoricalData
  (JNIEnv *, jobject, jstring, jint, jint);

#ifdef __cplusplus
}
#endif
#endif
