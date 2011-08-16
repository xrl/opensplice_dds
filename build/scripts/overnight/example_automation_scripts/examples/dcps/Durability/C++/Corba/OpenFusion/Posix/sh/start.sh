echo "=== Scenario 3.1"
sh ./startPublisher.sh transient true true > pubResult_3.1.txt&
#sleep 2

rm -f subResult_3.1.txt
SUB_PID=`sh ./startSubscriber.sh transient "./subResult_3.1.txt" &` 
echo "SUB_PID ===> $SUB_PID"

echo "=== waiting end of DurabilityDataSubscriber  (Scenario 3.1)"
APPLI="DurabilityDataSubscriber"
sh ./waiting_end_of.sh $SUB_PID $APPLI

echo "=== Scenario 3.2" 
sh ./startPublisher.sh transient false true> pubResult_3.2.txt &
sleep 2
echo === stopping publisher >> pubResult_3.2.txt

echo === running a first Subscriber   > subResult_3.2.1.txt
SUB_PID=`sh ./startSubscriber.sh transient "./subResult_3.2.1.txt" &` 
echo "SUB_PID ===> $SUB_PID"
echo "=== waiting end of first DurabilityDataSubscriber  (Scenario 3.2)"
APPLI="DurabilityDataSubscriber"
sh ./waiting_end_of.sh $SUB_PID  $APPLI

echo === running a second Subscriber  > subResult_3.2.2.txt
SUB_PID=`sh ./startSubscriber.sh transient "./subResult_3.2.2.txt" &` 
echo "SUB_PID ===> $SUB_PID"
echo "=== waiting end of second DurabilityDataSubscriber (Scenario 3.2)"
APPLI="DurabilityDataSubscriber"
sh ./waiting_end_of.sh $SUB_PID  $APPLI

echo "=== Scenario 3.3"
echo "=== Stop OpenSplice"
$VG_OSPL_START ospl stop
sleep 2
echo "=== Start OpenSplice"
$VG_OSPL_START ospl start
$VG_START_SLEEP
sh ./startPublisher.sh persistent false true> pubResult_3.3.txt &
sleep 2

echo === running a first Subscriber > subResult_3.3.1.txt
SUB_PID=`sh ./startSubscriber.sh persistent "./subResult_3.3.1.txt" &` 
echo "SUB_PID ===> $SUB_PID"
echo === "waiting end of first DurabilityDataSubscriber (Scenario 3.3)"
APPLI="DurabilityDataSubscriber"
sh ./waiting_end_of.sh  $SUB_PID $APPLI

echo "=== Stop OpenSplice"
$VG_OSPL_START ospl stop
echo "=== waiting 10s to let OpenSplice finish stopping"
sleep 10
echo "=== Start OpenSplice"
$VG_OSPL_START ospl start
$VG_START_SLEEP
sleep 15
echo === running a second Subscriber after stop/start of OpenSplice > subResult_3.3.2.txt
sh ./startSubscriber.sh persistent >> subResult_3.3.2.txt&
SUB_PID=`sh ./startSubscriber.sh persistent "./subResult_3.3.txt" &` 
echo "SUB_PID ===> $SUB_PID"
echo "=== waiting end of second DurabilityDataSubscriber (Scenario 3.3)"
APPLI="DurabilityDataSubscriber"
sh ./waiting_end_of.sh  $SUB_PID $APPLI

