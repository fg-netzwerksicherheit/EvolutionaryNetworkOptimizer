/*
 * TimestampTag.cpp
 *
 *  Created on: May 4, 2017
 */

#include "TimestampTag.h"

namespace simul {

using namespace ns3;

TypeId TimestampTag::GetTypeId(void) {
	static TypeId tid =
			TypeId("simul::TimestampTag").SetParent<Tag>().AddConstructor<TimestampTag>().AddAttribute(
					"Timestamp", "Some momentous point in time!", EmptyAttributeValue(),
					MakeTimeAccessor(&TimestampTag::GetTimestamp), MakeTimeChecker());
	return tid;
}

TypeId TimestampTag::GetInstanceTypeId(void) const {
	return GetTypeId();
}

uint32_t TimestampTag::GetSerializedSize(void) const {
	return 8;
}

void TimestampTag::Serialize(TagBuffer i) const {
	int64_t t = m_timestamp.GetNanoSeconds();
	i.Write((const uint8_t *) &t, 8);
}

void TimestampTag::Deserialize(TagBuffer i) {
	int64_t t;
	i.Read((uint8_t *) &t, 8);
	m_timestamp = NanoSeconds(t);
}

void TimestampTag::SetTimestamp(Time time) {
	m_timestamp = time;
}

Time TimestampTag::GetTimestamp(void) const {
	return m_timestamp;
}

void TimestampTag::Print(std::ostream &os) const {
	os << "t=" << m_timestamp;
}

} /* namespace simul */
