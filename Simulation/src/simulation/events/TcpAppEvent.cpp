/*
 * TcpSocketEvent.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: Quang Pham, Kiet Vo
 */

#include "TcpAppEvent.h"
#include "apps/OnOffAppHelper.h"

namespace simul {

using namespace ns3;
NS_LOG_COMPONENT_DEFINE("simul::TcpAppEvent");

TcpAppEvent::TcpAppEvent(NodePtr source, NodePtr dest, uint32_t bytes) :
		Event(source, dest), maxBytes(bytes) {
}

TcpAppEvent::~TcpAppEvent() {
}

uint64_t TcpAppEvent::GetTotalDataSize() {
	return static_cast<uint64_t>(maxBytes);
}

/**
 * \brief not implemented
 */
bool TcpAppEvent::IsSuccessful() {
	return false;	//TODO: implement
}

bool TcpAppEvent::IsValidToSetStartTime(double p_startTime) {
	if (p_startTime < 0)
		return false;

	return true;
}

bool TcpAppEvent::IsValidToSetUp() {
	if (hasStartTime == false)
		return false;

	Ptr<ns3::Node> source = sourceNode->GetNs3Node();
	Ptr<ns3::Node> dest = destNode->GetNs3Node();
	if (source->GetObject<Ipv4>()->GetNInterfaces() < 2) {
		std::string msg = "Source node has no connection!";
		NS_LOG_WARN(msg);
		return false;
	}
	if (dest->GetObject<Ipv4>()->GetNInterfaces() < 2) {
		std::string msg = "Destination node has no connection!";
		NS_LOG_WARN(msg);
		return false;
	}

	return true;
}

void TcpAppEvent::DoSetStartTime(double p_startTime) {
	startTime = p_startTime;
	hasStartTime = true;
}

void TcpAppEvent::DoSetUp() {
	Ptr<ns3::Node> ns3NodeSrc = sourceNode->GetNs3Node();
	Ptr<ns3::Node> ns3NodeDst = destNode->GetNs3Node();

	uint16_t port = 9;
	Ipv4Address dstAddr = ns3NodeDst->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();

	OnOffAppHelper source("ns3::TcpSocketFactory", InetSocketAddress(dstAddr, port));
	// Set the amount of data to send in bytes.  Zero is unlimited.
	source.SetAttribute("MaxBytes", UintegerValue(maxBytes));
	//source.SetAttribute("DataRate", DataRateValue(DataRate("6Mbps")));
	ApplicationContainer sourceApps = source.Install(ns3NodeSrc);
	sourceApps.Start(Seconds(startTime));

	//
	// Create a PacketSinkApplication and install it on destination node.
	//
	PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
	ApplicationContainer sinkApps = sink.Install(ns3NodeDst);
	sinkApps.Start(Seconds(0));
}

void TcpAppEvent::NotifySimulatorDestroyed() {
}

} /* namespace simul */



