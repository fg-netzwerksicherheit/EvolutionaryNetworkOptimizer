/*
 * TimestampTag.h
 *
 *  Created on: May 4, 2017
 */

#ifndef SIMULATION_EVENTS_TAGS_TIMESTAMPTAG_H_
#define SIMULATION_EVENTS_TAGS_TIMESTAMPTAG_H_

#include "../../../Si_Type.h"

namespace simul {

/**
 * \ingroup events
 * \defgroup tags
 */

/**
 * \ingroup tags
 */
class TimestampTag: public ns3::Tag {
public:

	static ns3::TypeId GetTypeId(void);
	virtual ns3::TypeId GetInstanceTypeId(void) const;

	virtual uint32_t GetSerializedSize(void) const;
	virtual void Serialize(ns3::TagBuffer i) const;
	virtual void Deserialize(ns3::TagBuffer i);

	// these are our accessors to our tag structure
	void SetTimestamp(ns3::Time time);
	ns3::Time GetTimestamp(void) const;

	void Print(std::ostream &os) const;

private:
	ns3::Time m_timestamp;
};

} /* namespace simul */

#endif /* SIMULATION_EVENTS_TAGS_TIMESTAMPTAG_H_ */
