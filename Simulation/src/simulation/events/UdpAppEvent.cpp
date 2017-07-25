/*
 * UdpAppEvent.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Quang Pham, Kiet Vo
 */

#include "UdpAppEvent.h"
#include "apps/OnOffAppHelper.h"

namespace simul {

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("simul::UdpAppEvent");

UdpAppEvent::UdpAppEvent(NodePtr source, NodePtr dest, uint32_t bytes) :
		Event(source, dest), maxBytes(bytes) {
}

UdpAppEvent::~UdpAppEvent() {
}

uint64_t UdpAppEvent::GetTotalDataSize() {
	return static_cast<uint64_t>(maxBytes);
}

/**
 * \brief not implemented
 */
bool UdpAppEvent::IsSuccessful() {
	return false;	//TODO:implement
}

bool UdpAppEvent::IsValidToSetStartTime(double p_startTime) {
	if (p_startTime < 0)
		return false;

	return true;
}

bool UdpAppEvent::IsValidToSetUp() {
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

void UdpAppEvent::DoSetStartTime(double p_startTime) {
	startTime = p_startTime;
	hasStartTime = true;
}

void UdpAppEvent::DoSetUp() {
	Ptr<ns3::Node> ns3NodeSrc = sourceNode->GetNs3Node();
	Ptr<ns3::Node> ns3NodeDst = destNode->GetNs3Node();
	uint16_t port = 99;
	Ipv4Address destAddr = ns3NodeDst->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();

	OnOffAppHelper client("ns3::UdpSocketFactory", Address());
	client.SetAttribute("MaxBytes", UintegerValue(maxBytes));
	client.SetAttribute("Remote", AddressValue(InetSocketAddress(destAddr, port)));
	client.SetAttribute("DataRate", DataRateValue(DataRate("6Mbps")));
	ApplicationContainer sourceApps = client.Install(ns3NodeSrc);
	sourceApps.Start(Seconds(startTime));

	Address sinkLocalAddress(InetSocketAddress(Ipv4Address::GetAny(), port));
	PacketSinkHelper sinkHelper("ns3::UdpSocketFactory", sinkLocalAddress);
	ApplicationContainer sinkApps = sinkHelper.Install(ns3NodeDst);
	sinkApps.Start(Seconds(0.0));
}


void UdpAppEvent::NotifySimulatorDestroyed() {
}

} /* namespace std */


