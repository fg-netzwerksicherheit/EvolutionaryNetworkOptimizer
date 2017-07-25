/*
 * UdpSocketEvent.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Kiet Vo
 */

#include "UdpTestEvent.h"
#include "tags/TimestampTag.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::UdpTestEvent");

UdpTestEvent::UdpTestEvent(NodePtr sourceNode, NodePtr destNode, bool p_reply) :
		Event(sourceNode, destNode), reply(p_reply) {
}

UdpTestEvent::~UdpTestEvent() {
	if(srcSocket != nullptr)
		srcSocket->Close();

	if (dstSocket != nullptr)
		dstSocket->Close();
}

uint64_t UdpTestEvent::GetTotalDataSize() {
	if (reply)
		return static_cast<uint64_t>(maxBytes * 2);
	else
		return static_cast<uint64_t>(maxBytes);
}

/**
 * This event is considered successful if:
 * - destination node receives all data in "no-reply" case
 * - source node receives reply from destination node in "reply" case
 */
bool UdpTestEvent::IsSuccessful() {
	return successful;
}

using namespace ns3;

bool UdpTestEvent::IsValidToSetStartTime(double p_startTime) {
	if (p_startTime < 0)
		return false;

	return true;
}

bool UdpTestEvent::IsValidToSetUp() {
	if (hasStartTime == false)	// not yet set start-time
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

void UdpTestEvent::DoSetStartTime(double p_startTime) {
	startTime = p_startTime;
	hasStartTime = true;
	successful = false;
}

static void UdpTestSendStuff(Ptr<Socket> sock, Ipv4Address dstaddr, uint16_t port, uint64_t bytes);
static void srcSocketRecv(bool * successful, Ptr<Socket> socket);
static void dstSocketRecv(const bool reply, bool * successful, uint64_t bytes, Ptr<Socket> socket);
static uint16_t GetAPortNumber(Ptr<ns3::Node> node);

void UdpTestEvent::DoSetUp() {
	Ptr<ns3::Node> source = sourceNode->GetNs3Node();
	Ptr<ns3::Node> dest = destNode->GetNs3Node();

	Ipv4Address dstaddr = dest->GetObject<Ipv4>()->GetAddress(1, 0).GetLocal();

	// source a.k.a client
	if (srcSocket == nullptr) {
		srcSocket = Socket::CreateSocket(source, TypeId::LookupByName("ns3::UdpSocketFactory"));
		srcSocket->Bind();
		srcSocket->SetRecvCallback(MakeBoundCallback(&srcSocketRecv, &successful));
	}

	// destination a.k.a server
	if (dstSocket == nullptr) {
		dstSocket = Socket::CreateSocket(dest, TypeId::LookupByName("ns3::UdpSocketFactory"));
		dstport = GetAPortNumber(dest);
		InetSocketAddress dst = InetSocketAddress(dstaddr, dstport);
		dstSocket->Bind(dst);
		dstSocket->SetRecvCallback(MakeBoundCallback(&dstSocketRecv, reply, &successful, maxBytes));
	}

	Simulator::Schedule(Seconds(startTime), &UdpTestSendStuff, srcSocket, dstaddr, dstport,
			maxBytes);
	successful = false;
}

void UdpTestEvent::NotifySimulatorDestroyed() {
	srcSocket = nullptr;
	dstSocket = nullptr;
	dstport = -1;
}

//
//
static void UdpTestSendStuff(Ptr<Socket> sock, Ipv4Address dstaddr, uint16_t port, uint64_t bytes) {
	Ptr<Packet> p = Create<Packet>(bytes);

	simul::TimestampTag tag;
	tag.SetTimestamp(Simulator::Now());
	p->AddPacketTag(tag);

	sock->SendTo(p, 0, InetSocketAddress(dstaddr, port));
}

static void srcSocketRecv(bool * successful, Ptr<Socket> socket) {
	Address from;
	Ptr<Packet> packet = socket->RecvFrom(from);
	packet->RemoveAllPacketTags();
	packet->RemoveAllByteTags();
	InetSocketAddress address = InetSocketAddress::ConvertFrom(from);

	NS_LOG_DEBUG(
			"Source Received " << packet->GetSize() << " bytes from " << address.GetIpv4()<<":"<<address.GetPort());

	*successful = true;
}

static void dstSocketRecv(const bool reply, bool * successful, uint64_t bytes, Ptr<Socket> socket) {
	Address from;
	Ptr<Packet> packet = socket->RecvFrom(from);
	packet->RemoveAllPacketTags();
	packet->RemoveAllByteTags();
	InetSocketAddress address = InetSocketAddress::ConvertFrom(from);

	NS_LOG_DEBUG(
			"Destination Received " << packet->GetSize() << " bytes from " << address.GetIpv4()<<":"<<address.GetPort());

	if (reply) {
		NS_LOG_DEBUG("Send reply to Source");
		UdpTestSendStuff(socket, address.GetIpv4(), address.GetPort(), bytes);
	} else {
		NS_LOG_DEBUG("Destination will not send reply!");
		*successful = true;
	}
}

static uint16_t GetAPortNumber(Ptr<ns3::Node> node) {
	Ptr<Socket> socket = Socket::CreateSocket(node, TypeId::LookupByName("ns3::UdpSocketFactory"));
	socket->Bind();

	Address adr;
	socket->GetSockName(adr);
	socket->Close();

	return InetSocketAddress::ConvertFrom(adr).GetPort();
}

} /* namespace simul */

