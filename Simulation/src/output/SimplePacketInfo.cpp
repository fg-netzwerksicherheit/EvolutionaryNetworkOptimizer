/*
 * SimplePacketInfo.cpp
 *
 *  Created on: May 2, 2017
 *      Author: Kiet Vo
 */

#include "SimplePacketInfo.h"

namespace simul {

SimplePacketInfo::SimplePacketInfo() {
}

SimplePacketInfo::SimplePacketInfo(SimplePacketInfo &obj) {
	this->m_pid = obj.m_pid;
	this->m_size = obj.m_size;
	this->m_type = obj.m_type;
	this->m_sent = obj.m_sent;
	this->m_delivered = obj.m_delivered;
}

SimplePacketInfo::~SimplePacketInfo() {
}

void SimplePacketInfo::Print(ostream &os) const {
	switch (m_type) {
	case PACKET_SENT:
		os << "Outgoing packet";
		break;
	case PACKET_FORWARDED:
		os << "Forwarded packet";
		break;
	case PACKET_DELIVERED:
		os << "Delivered packet";
		break;
	case PACKET_DROPPED:
		os << "Dropped packet";
		break;
	case PACKET_UNKNOWN:
		os << "Unknown packet";
		break;
	default:
		os << "Unknown packet type (critical!)";
	}
	os << " (pid = " << m_pid << ",";
	os << " size = " << m_size << ",";
	os << " sent at " << m_sent.GetSeconds() << "s";
	if (m_type == PACKET_DELIVERED)
		os << ", delivered at " << m_delivered.GetSeconds() << "s";
	os << ")" << endl;
}

uint64_t SimplePacketInfo::GetPID() {
	return m_pid;
}

uint32_t SimplePacketInfo::GetSize() {
	return m_size;
}

PacketType SimplePacketInfo::GetType() {
	return m_type;
}

ns3::Time SimplePacketInfo::GetSentTime() {
	return m_sent;
}

ns3::Time SimplePacketInfo::GetDeliveredTime() {
	return m_delivered;
}

void SimplePacketInfo::SetPacketID(uint64_t p_pid) {
	m_pid = p_pid;
}

void SimplePacketInfo::SetSize(uint32_t p_size) {
	m_size = p_size;
}

void SimplePacketInfo::SetType(PacketType p_type) {
	m_type = p_type;
}

void SimplePacketInfo::SetSentTime(ns3::Time p_sent) {
	m_sent = p_sent;
}

void SimplePacketInfo::SetDeliveredTime(ns3::Time p_delivered) {
	m_delivered = p_delivered;
}

}
/* namespace simul */
