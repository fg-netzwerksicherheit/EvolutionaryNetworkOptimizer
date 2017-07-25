/*
 * SimplePacketInfo.h
 *
 *  Created on: May 2, 2017
 *      Author: Kiet Vo
 */

#ifndef OUTPUT_SIMPLEPACKETINFO_H_
#define OUTPUT_SIMPLEPACKETINFO_H_

#include "PacketInfo.h"

namespace simul {

/**
 * \ingroup output
 */
class SimplePacketInfo: public PacketInfo {
private:
	uint64_t m_pid = 0;
	uint32_t m_size = 0;
	PacketType m_type = PACKET_UNKNOWN;
	ns3::Time m_sent;
	ns3::Time m_delivered;

public:
	SimplePacketInfo();
	SimplePacketInfo(SimplePacketInfo &obj);
	virtual ~SimplePacketInfo();

	virtual void Print(ostream &os) const override;

	virtual uint64_t GetPID() override;
	virtual uint32_t GetSize() override;
	virtual PacketType GetType() override;
	virtual ns3::Time GetSentTime() override;
	virtual ns3::Time GetDeliveredTime() override;

	void SetPacketID(uint64_t p_pid);
	void SetSize(uint32_t p_size);
	void SetType(PacketType p_type);
	void SetSentTime(ns3::Time p_sent);
	void SetDeliveredTime(ns3::Time p_delivered);
};

} /* namespace simul */

#endif /* OUTPUT_SIMPLEPACKETINFO_H_ */
