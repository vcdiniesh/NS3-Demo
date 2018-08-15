/**-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-*
//Self Organizing Networking Group (SONG) @ Kongu Engineering College (KEC) 
//vcdiniesh@gmail.com
*-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*-**-*-*-*-*-*-*-*-*-*-*-*--*-***/
/*This example program helps how to add & make effectiently use of commandline argument

STEP1: Include necesssary declaration and add arguments in cmdline
STEP2: ./waf --run "scratch/all-in-one-PGM --help"

    Program Arguments:
    --EnablePCAP:     Number of "extra" CSMA nodes/devices [false]
    --MaxPackets:     Tell echo applications to log if true [1]
    --anim:           Tell echo applications to log if true [false]
    --tracing:        Tell echo applications to log if true [false]
    --EnableMonitor:  Enable Flow Monitor [false]

STEP3:./waf --run "scratch/all-in-one-PGM --EnablePCAP=true"
       By passing either true or False corresponding argument enables */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"
#include "ns3/flow-monitor-helper.h"
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

int
main (int argc, char *argv[])
{
  CommandLine cmd;
  bool EnablePCAP = false;
  uint32_t MaxPackets = 1;
  bool anim= false;
  bool tracing=false;
  bool enableFlowMonitor = false;
  cmd.AddValue ("EnablePCAP", "Number of \"extra\" CSMA nodes/devices", EnablePCAP);
  cmd.AddValue ("MaxPackets", "Tell echo applications to log if true", MaxPackets);
  cmd.AddValue ("anim", "Tell echo applications to log if true", anim);
  cmd.AddValue ("tracing", "Tell echo applications to log if true", tracing);
  cmd.AddValue ("EnableMonitor", "Enable Flow Monitor", enableFlowMonitor);

  cmd.Parse (argc, argv);
  
  Time::SetResolution (Time::NS);
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  NodeContainer nodes;
  nodes.Create (2);

  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  InternetStackHelper stack;
  stack.Install (nodes);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (MaxPackets));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));
 //Enable PCAP
if(EnablePCAP) 
{
pointToPoint.EnablePcapAll ("myfirst");
}

// Creating a bounding box for NetAnim
 if(anim)  
{
  AnimationInterface anim ("first.xml");
  AnimationInterface::SetConstantPosition (nodes.Get (0), 10, 25);
  AnimationInterface::SetConstantPosition (nodes.Get (1), 40, 25);
  anim.EnablePacketMetadata (true);
}
//trace
if(tracing)
{
  AsciiTraceHelper ascii;
  pointToPoint.EnableAsciiAll (ascii.CreateFileStream ("myfirst.tr"));
}
  // Flow Monitor
  FlowMonitorHelper flowmonHelper;
if (enableFlowMonitor)
{
      flowmonHelper.InstallAll ();
}
Simulator::Stop (Seconds(11));
  Simulator::Run ();
if (enableFlowMonitor)
{
      flowmonHelper.SerializeToXmlFile ("simple-global-routing.flowmon", true, true);
 }
  Simulator::Destroy ();
  return 0;
}
