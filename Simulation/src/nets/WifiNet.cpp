/*
 * WifiNet.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: Kiet Vo
 */

#include "../nets/WifiNet.h"

#include "../components/Edge.h"
#include "../components/Node.h"
#include "../components/P2PEdge.h"
#include "../components/WifiEdge.h"

#include "../exceptions/LimitationException.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::WifiNet");

int WifiNet::wifiNetCount = 0;
void WifiNet::ResetNetCount() {
	wifiNetCount = 0;
}

WifiNet::WifiNet() {
	wifiNetCount++;
	localNetID = wifiNetCount;

	apNode = ns3::NodeContainer();
	staNodes = ns3::NodeContainer();
}

WifiNet::~WifiNet() {
}

ns3::NetDeviceContainer WifiNet::GetAllNetDevices() {
	return devices;
}

using namespace ns3;

void static AssignIPAddressWifi(int localNetID, NetDeviceContainer & devices);

void WifiNet::Configure() {
	NodePtr accessPoint = (*edgeList->begin())->GetSourceNode();

	apNode.Add(accessPoint->GetNs3Node());

	// find station nodes from edge list
	for (auto it = edgeList->begin(); it != edgeList->end(); it++) {
		EdgePtr currentEdge = *it;
		staNodes.Add(currentEdge->GetDestNode()->GetNs3Node());
	}

	//
	//
	YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
	YansWifiPhyHelper phy = YansWifiPhyHelper::Default();
	phy.SetChannel(channel.Create());

	WifiHelper wifi;
	wifi.SetRemoteStationManager("ns3::AarfWifiManager");

	WifiMacHelper mac;
	Ssid ssid = Ssid("wifi" + std::to_string(localNetID));
	mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid), "ActiveProbing",
			ns3::BooleanValue(false));

	NetDeviceContainer staDevices;
	staDevices = wifi.Install(phy, mac, staNodes);

	mac.SetType("ns3::ApWifiMac", "Ssid", ns3::SsidValue(ssid));

	NetDeviceContainer apDevices;
	apDevices = wifi.Install(phy, mac, apNode);

	MobilityHelper mobility;
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobility.Install(staNodes);
	mobility.Install(apNode);

	// add to device list and assign ip address
	devices.Add(staDevices);
	devices.Add(apDevices);
	AssignIPAddressWifi(localNetID, devices);
}

void static AssignIPAddressWifi(int localNetID, NetDeviceContainer & devices) {
	if (localNetID > 254) {
		string msg = "Too many P2PNet-s";
		NS_LOG_ERROR(msg);
		throw LimitationException(msg);
	}

	// calculate
	Ipv4AddressHelper address;
	std::ostringstream subnet;
	subnet << "10." << localNetID << ".0.0";
	if (devices.GetN() < 255)
		address.SetBase(subnet.str().c_str(), "255.255.255.0");
	else
		address.SetBase(subnet.str().c_str(), "255.255.0.0");

	// assign IP addresses
	address.Assign(devices);
}

} /* namespace simul */
