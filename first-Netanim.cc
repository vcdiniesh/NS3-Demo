// ****************************************************************************************
//Self Organizing Networking Group (SONG) @ Kongu Engineering College (KEC) 

// ****************************************************************************************
/*
 This example program helps to enable Netanim Module 
 By default, the program not have suppport with Netanim the following modification included with explanation.
STEP1: Include necesssary Header File
STEP2: creating boundary for Netanim with file_name.anim
STEP3: ./waf --run scratch/first-Netanim
 	   In ns-3.2x directory file_name.anim file created
STEP4: GoTo netanim directory and type 
	   song@song:~/ns-allinone-3.27$ cd netanim-3.108/
	   song@song:~/ns-allinone-3.27/netanim-3.108$ ./NetAnim 
STEP5: Upload a file */

// Necessary header files
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/netanim-module.h"
#include "ns3/mobility-module.h"

// Namespace
using namespace ns3;

// Logging module
NS_LOG_COMPONENT_DEFINE ("FirstScriptExample");

// Main program
int
main (int argc, char *argv[])
{
  // Enabling Log of available modules
  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);

  // Creating nodes
  NodeContainer nodes;
  nodes.Create (2);

  // Creating Network Interfaces - in this case a point-to-point interface
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));

  // Create Network Device Container and install it on the nodes
  NetDeviceContainer devices;
  devices = pointToPoint.Install (nodes);

  // Install protocol stack in nodes
  InternetStackHelper stack;
  stack.Install (nodes);

  // Set network addresses for interfaces
  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer interfaces = address.Assign (devices);

  // Install applications in nodes
  UdpEchoServerHelper echoServer (9);

  ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
  // Set application start and stop time
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (10.0));

  UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (5));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
  clientApps.Start (Seconds (2.0));
  clientApps.Stop (Seconds (10.0));

  // Assigning fixed position to nodes. Optional, but if skipped, gives warning for animation.
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (nodes);

  // Creating a bounding box for NetAnim
  
  AnimationInterface anim ("first.xml");
  AnimationInterface::SetConstantPosition (nodes.Get (0), 10, 25);
  AnimationInterface::SetConstantPosition (nodes.Get (1), 40, 25);
  anim.EnablePacketMetadata (true);

  // Run the simulation
  Simulator::Run ();

  // Release resources at the end of the simulation
  Simulator::Destroy ();
  return 0;
}
