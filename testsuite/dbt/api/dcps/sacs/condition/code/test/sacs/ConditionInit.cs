namespace test.sacs
{
    /// <date>Jun 2, 2005</date>
    public class ConditionInit : Test.Framework.TestItem
    {
        public ConditionInit()
            : base("Initialize condition")
        {
        }

        public override Test.Framework.TestResult Run(Test.Framework.TestCase testCase)
        {
            DDS.DomainParticipantFactory factory;
            DDS.IDomainParticipant participant;
            DDS.DomainParticipantQos pqos = null;
            DDS.SubscriberQos subQos = null;
            DDS.ISubscriber subscriber;
            DDS.PublisherQos pubQos = null;
            DDS.IPublisher publisher;
            mod.tstTypeSupport typeSupport;
            DDS.TopicQos tQos = null;
            DDS.ITopic topic;
            DDS.DataReaderQos drQos = null;
            mod.tstDataReader datareader;
            DDS.DataWriterQos dwQos = null;
            mod.tstDataWriter datawriter;
            Test.Framework.TestResult result;
            DDS.ReturnCode rc;
            result = new Test.Framework.TestResult("Initialization success", string.Empty, Test.Framework.TestVerdict
                .Pass, Test.Framework.TestVerdict.Fail);
            factory = DDS.DomainParticipantFactory.Instance;
            if (factory == null)
            {
                result.Result = "DomainParticipantFactory could not be initialized.";
                return result;
            }

            if (factory.GetDefaultParticipantQos(ref pqos) != DDS.ReturnCode.Ok)
            {
                result.Result = "Default DomainParticipantQos could not be resolved.";
                return result;
            }
            participant = factory.CreateParticipant(string.Empty, pqos);//, null, 0);
            if (participant == null)
            {
                result.Result = "Creation of DomainParticipant failed.";
                return result;
            }

            if (participant.GetDefaultSubscriberQos(ref subQos) != DDS.ReturnCode.Ok)
            {
                result.Result = "Default SubscriberQos could not be resolved.";
                return result;
            }
            subscriber = participant.CreateSubscriber(subQos);//, null, 0);
            if (subscriber == null)
            {
                result.Result = "Subscriber could not be created.";
                return result;
            }
            typeSupport = new mod.tstTypeSupport();
            rc = typeSupport.RegisterType(participant, "tstType");
            if (rc != DDS.ReturnCode.Ok)
            {
                result.Result = "Typesupport could not be registered.";
                return result;
            }

            if (participant.GetDefaultTopicQos(ref tQos) != DDS.ReturnCode.Ok)
            {
                result.Result = "Default TopicQos could not be resolved.";
                return result;
            }

            topic = participant.CreateTopic("tst", "tstType", tQos);//, null, 0);
            if (topic == null)
            {
                result.Result = "Topic could not be created.";
                return result;
            }
            if (participant.GetDefaultPublisherQos(ref pubQos) != DDS.ReturnCode.Ok)
            {
                result.Result = "Default PublisherQos could not be resolved.";
                return result;
            }
            publisher = participant.CreatePublisher(pubQos);//, null, 0);
            if (publisher == null)
            {
                result.Result = "Publisher could not be created.";
                return result;
            }

            if (subscriber.GetDefaultDataReaderQos(ref drQos) != DDS.ReturnCode.Ok)
            {
                result.Result = "Default DataReaderQos could not be resolved.";
                return result;
            }
            datareader = (mod.tstDataReader)subscriber.CreateDataReader(topic, drQos);//, null, 0);
            if (datareader == null)
            {
                result.Result = "DataReader could not be created.";
                return result;
            }

            if (publisher.GetDefaultDataWriterQos(ref dwQos) != DDS.ReturnCode.Ok)
            {
                result.Result = "Default DataWriterQos could not be resolved.";
                return result;
            }
            datawriter = (mod.tstDataWriter)publisher.CreateDataWriter(topic, dwQos);//, null, 0);

            testCase.RegisterObject("factory", factory);
            testCase.RegisterObject("participantQos", pqos);
            testCase.RegisterObject("participant", participant);
            testCase.RegisterObject("topic", topic);
            testCase.RegisterObject("topicQos", tQos);
            testCase.RegisterObject("subscriber", subscriber);
            testCase.RegisterObject("subscriberQos", subQos);
            testCase.RegisterObject("datareader", datareader);
            testCase.RegisterObject("datareaderQos", drQos);
            testCase.RegisterObject("publisher", publisher);
            testCase.RegisterObject("publisherQos", pubQos);
            testCase.RegisterObject("datawriter", datawriter);
            testCase.RegisterObject("datawriterQos", dwQos);
            result.Result = "Initialization success.";
            result.Verdict = Test.Framework.TestVerdict.Pass;
            return result;
        }
    }
}
