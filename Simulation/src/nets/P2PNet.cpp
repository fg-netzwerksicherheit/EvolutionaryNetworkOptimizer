/*
 * P2PNet.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#include "../nets/P2PNet.h"

#include "../components/Edge.h"
#include "../components/P2PEdge.h"
#include "../components/WifiEdge.h"
#include "../components/Node.h"

#include "../exceptions/LimitationException.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::P2PNet");

int P2PNet::p2pNetCount = 0;
void P2PNet::ResetNetCount() {
	p2pNetCount = 0;
}

P2PNet::P2PNet() {
	p2pNetCount++;
	localNetID = p2pNetCount;
}

P2PNet::~P2PNet() {

}

ns3::NetDeviceContainer P2PNet::GetAllNetDevices() {
	return devices;
}

using namespace ns3;
void static AssignIPAddressP2P(int localNetID, NetDeviceContainer & devices);

void P2PNet::Configure() {

	// Get two node(s) from P2P node list
	shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*edgeList->begin());
	Ptr<ns3::Node> ns3SrcNode = edge->GetSourceNode()->GetNs3Node();
	Ptr<ns3::Node> ns3DstNode = edge->GetDestNode()->GetNs3Node();

	p2pNodes.Add(ns3SrcNode);
	p2pNodes.Add(ns3DstNode);

	// install p2p
	PointToPointHelper pointToPoint;
	NetDeviceContainer p2pDevices = pointToPoint.Install(p2pNodes);

	// Set attributes
	if (ns3SrcNode == p2pDevices.Get(0)->GetNode()) {
		edge->srcDevice = DynamicCast<PointToPointNetDevice>(p2pDevices.Get(0));
		edge->dstDevice = DynamicCast<PointToPointNetDevice>(p2pDevices.Get(1));
	} else if (ns3DstNode == p2pDevices.Get(0)->GetNode()) {
		edge->dstDevice = DynamicCast<PointToPointNetDevice>(p2pDevices.Get(0));
		edge->srcDevice = DynamicCast<PointToPointNetDevice>(p2pDevices.Get(1));
	}
	edge->UpdateConfiguration();

	// add to device list and assign ip address
	devices.Add(p2pDevices);
	AssignIPAddressP2P(localNetID, devices);
}

void static AssignIPAddressP2P(int localNetID, NetDeviceContainer & devices) {
	if (localNetID > 254 * 64) {
		string msg = "Too many P2PNet-s";
		NS_LOG_ERROR(msg);
		throw LimitationException(msg);
	}

	// calculate
	Ipv4AddressHelper address;
	std::ostringstream subnet;
	subnet << "192.168.";
	subnet << (localNetID - 1) / 64;                // max 64 p2p-nets (64*4 = 256)
	subnet << ".";
	subnet << (4 * (localNetID - 1)) % 256;        // each p2p-net takes 4 addresses
	address.SetBase(subnet.str().c_str(), "255.255.255.252");

	// assign IP addresses
	address.Assign(devices);
}

} /* namespace simul */
