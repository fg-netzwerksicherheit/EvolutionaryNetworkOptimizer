/*
 * Ipv4PacketInfoOutput.cpp
 *
 *  Created on: May 1, 2017
 *      Author: Kiet Vo
 */

#include "Ipv4PacketInfoOutput.h"
#include "../exceptions/LimitationException.h"
#include "SimplePacketInfo.h"
#include "../simulation/events/tags/TimestampTag.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::Ipv4PacketInfoOutput");

Ipv4PacketInfoOutput::Ipv4PacketInfoOutput() {
	outgoingList = make_shared<vector<PacketInfoPtr>>();
	droppedList = make_shared<vector<PacketInfoPtr>>();
	deliveredList = make_shared<vector<PacketInfoPtr>>();
}

Ipv4PacketInfoOutput::~Ipv4PacketInfoOutput() {
	DisableCbAll();
}

/**
 * \brief Type of list to retrieve
 */
void Ipv4PacketInfoOutput::SetListType(Ipv4PIOListType p_type) {
	m_type = p_type;
}

/**
 * \brief Clear previous data in 3 lists.
 *
 * Useful when running simulation many times as new data will be append to the lists
 * instead of replacing the old one.
 *
 * \see Ipv4PIOListType
 */
void Ipv4PacketInfoOutput::ClearPacketInfoList() {
	outgoingList->clear();
	droppedList->clear();
	deliveredList->clear();
}

/**
 * \brief Retrieve packet list with the type specified.
 *
 * Default type is DELIVERED_LIST.
 *
 * \see SetListType(Ipv4PIOListType)
 */
VectorPtr<PacketInfoPtr> Ipv4PacketInfoOutput::GetPacketInfoList() {
	switch (m_type) {
	case OUTGOING_LIST:
		return outgoingList;
	case DROPPED_LIST:
		return droppedList;
	case DELIVERED_LIST:
		return deliveredList;
	}
	return nullptr;
}

// prototype declaration
using namespace ns3;
static void CbUnicastForward(Ptr<Ipv4> ipv4, const Ipv4Header &header, Ptr<const Packet> packet,
		uint32_t interface);
static bool ReadTimestampTag(Ptr<const Packet> packet, Time & time);

//
// enable
//

/**
 * \brief When an ipv4 packet is sent to outgoing interface.
 *
 * \see ns3::Ipv4L3Protocol::TxRxTracedCallback
 */
void Ipv4PacketInfoOutput::EnableCbTx() {
	DisableCbTx();
	Config::ConnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/Tx",
			MakeCallback(&Ipv4PacketInfoOutput::CbTx, this));
}

/**
 * \brief When an ipv4 packet is dropped.
 *
 * \see ns3::Ipv4L3Protocol::DropTracedCallback
 */
void Ipv4PacketInfoOutput::EnableCbDrop() {
	DisableCbDrop();
	Config::ConnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/Drop",
			MakeCallback(&Ipv4PacketInfoOutput::CbDrop, this));
}

/**
 * \brief When a newly-generated packet by a node is about to be queued for transmission.
 *
 * \see ns3::Ipv4L3Protocol::SentTracedCallback
 */
void Ipv4PacketInfoOutput::EnableCbSendOutgoing() {
	DisableCbSendOutgoing();
	Config::ConnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/SendOutgoing",
			MakeCallback(&Ipv4PacketInfoOutput::CbSendOutgoing, this));
}

/**
 * \brief When a unicast IPv4 packet was received by a node and is being
 * forwarded to another node.
 *
 * \see ns3::Ipv4L3Protocol::SentTracedCallback
 */
void Ipv4PacketInfoOutput::EnableCbUnicastForward() {
	DisableCbUnicastForward();

	NodeContainer nodes = NodeContainer::GetGlobal();
	for (uint i = 0; i < nodes.GetN(); i++) {
		Ptr<Ipv4> ipv4 = nodes.Get(i)->GetObject<Ipv4>();
		ipv4->TraceConnectWithoutContext("UnicastForward",
				MakeBoundCallback(&CbUnicastForward, ipv4));
	}
}

/**
 * \brief When an IPv4 packet was received by/for this node, and it is being
 * forward up the stack.
 *
 * \see ns3::Ipv4L3Protocol::SentTracedCallback
 */
void Ipv4PacketInfoOutput::EnableCbLocalDeliver() {
	DisableCbLocalDeliver();
	Config::ConnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/LocalDeliver",
			MakeCallback(&Ipv4PacketInfoOutput::CbLocalDeliver, this));
}

//
// disable
//
void Ipv4PacketInfoOutput::DisableCbTx() {
	Config::DisconnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/Tx",
			MakeCallback(&Ipv4PacketInfoOutput::CbTx, this));
}

void Ipv4PacketInfoOutput::DisableCbDrop() {
	Config::DisconnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/Drop",
			MakeCallback(&Ipv4PacketInfoOutput::CbDrop, this));
}

void Ipv4PacketInfoOutput::DisableCbSendOutgoing() {
	Config::DisconnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/SendOutgoing",
			MakeCallback(&Ipv4PacketInfoOutput::CbSendOutgoing, this));
}

void Ipv4PacketInfoOutput::DisableCbUnicastForward() {
	NodeContainer nodes = NodeContainer::GetGlobal();
	for (uint i = 0; i < nodes.GetN(); i++) {
		Ptr<Ipv4> ipv4 = nodes.Get(i)->GetObject<Ipv4>();
		ipv4->TraceDisconnectWithoutContext("UnicastForward",
				MakeBoundCallback(&CbUnicastForward, ipv4));
	}
}

void Ipv4PacketInfoOutput::DisableCbLocalDeliver() {
	Config::DisconnectWithoutContext("/NodeList/*/$ns3::Ipv4L3Protocol/LocalDeliver",
			MakeCallback(&Ipv4PacketInfoOutput::CbLocalDeliver, this));
}

/**
 * \brief Disable all callback functions
 */
void Ipv4PacketInfoOutput::DisableCbAll() {
	DisableCbTx();
	DisableCbDrop();
	DisableCbSendOutgoing();
	DisableCbUnicastForward();
	DisableCbLocalDeliver();
}

//
// callback-related functions
//
void Ipv4PacketInfoOutput::CbTx(Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface) {
	NS_LOG_DEBUG(
			ipv4->GetAddress(interface, 0).GetLocal() << " starts pumping " << packet->GetSize() << " bytes (pid = " << packet->GetUid() << ")");
}

void Ipv4PacketInfoOutput::CbDrop(const Ipv4Header &header, Ptr<const Packet> packet,
		Ipv4L3Protocol::DropReason reason, Ptr<Ipv4> ipv4, uint32_t interface) {

	Time sentTime;
	if (!ReadTimestampTag(packet, sentTime))
		return;

	SimplePacketInfo simpleInfo;
	simpleInfo.SetPacketID(packet->GetUid());
	simpleInfo.SetSize(packet->GetSize());
	simpleInfo.SetType(PACKET_DROPPED);
	simpleInfo.SetSentTime(sentTime);
	simpleInfo.SetDeliveredTime(Simulator::Now());

	droppedList->push_back(make_shared_base_in_simul(PacketInfo, SimplePacketInfo, simpleInfo));

	NS_LOG_DEBUG(
			ipv4->GetAddress(interface,0).GetLocal() << " dropped a packet of " << packet->GetSize() << " bytes (pid = " << packet->GetUid() << ") from " << header.GetSource() << " to " << header.GetDestination());
}

void Ipv4PacketInfoOutput::CbSendOutgoing(const Ipv4Header &header, Ptr<const Packet> packet,
		uint32_t interface) {

	Time sentTime;
	if (!ReadTimestampTag(packet, sentTime))
		return;

	SimplePacketInfo simpleInfo;
	simpleInfo.SetPacketID(packet->GetUid());
	simpleInfo.SetSize(packet->GetSize());
	simpleInfo.SetType(PACKET_SENT);
	simpleInfo.SetSentTime(sentTime);

	outgoingList->push_back(make_shared_base_in_simul(PacketInfo, SimplePacketInfo, simpleInfo));

	NS_LOG_DEBUG(
			header.GetSource() << " starts sending " << packet->GetSize() << " bytes (pid = " << packet->GetUid() << ") to " << header.GetDestination());
}

static void CbUnicastForward(Ptr<Ipv4> ipv4, const Ipv4Header &header, Ptr<const Packet> packet,
		uint32_t interface) {
	NS_LOG_DEBUG(
			ipv4->GetAddress(interface, 0).GetLocal() << " forwarded " << packet->GetSize() << " bytes (pid = " << packet->GetUid() << ") from " << header.GetSource() << " to " << header.GetDestination());
}

void Ipv4PacketInfoOutput::CbLocalDeliver(const Ipv4Header &header, Ptr<const Packet> packet,
		uint32_t interface) {

	Time sentTime;
	if (!ReadTimestampTag(packet, sentTime))
		return;

	SimplePacketInfo simpleInfo;
	simpleInfo.SetPacketID(packet->GetUid());
	simpleInfo.SetSize(packet->GetSize());
	simpleInfo.SetType(PACKET_DELIVERED);
	simpleInfo.SetSentTime(sentTime);
	simpleInfo.SetDeliveredTime(Simulator::Now());

	deliveredList->push_back(make_shared_base_in_simul(PacketInfo, SimplePacketInfo, simpleInfo));

	NS_LOG_DEBUG(
			header.GetDestination() << " received " << packet->GetSize() << " bytes (pid = " << packet->GetUid() << ") from " << header.GetSource());
}

static bool ReadTimestampTag(Ptr<const Packet> packet, Time & time) {
	TimestampTag tag;
	if (packet->PeekPacketTag(tag)) {
		time = tag.GetTimestamp();
		return true;
	} else
		return false;
}

} /* namespace simul */
