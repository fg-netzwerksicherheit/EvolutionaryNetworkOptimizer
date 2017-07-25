/*
 * PacketInfo.h
 *
 *  Created on: May 2, 2017
 *      Author: Kiet Vo
 */

#ifndef OUTPUT_PACKETINFO_H_
#define OUTPUT_PACKETINFO_H_

#include "../Si_Type.h"

namespace simul {

/**
 * The interpretation of a packet's type may heavily depend on the specific implementation
 * of the output system which in turn depends on ns-3.
 *
 * \see Ipv4PacketInfoOutput
 */
enum PacketType {
	PACKET_SENT,		//!< has been sent and no other information
	PACKET_FORWARDED,	//!< has been forwarded and no other information
	PACKET_DELIVERED,	//!< has been delivered
	PACKET_DROPPED,		//!< has been dropped and no other information
	PACKET_UNKNOWN		//!< other cases
};

/**
 * \ingroup output
 *
 * \brief This class models a packet's meta-data.
 */
class PacketInfo {
public:
	virtual ~PacketInfo() {
	}
	;

	virtual uint64_t GetPID() = 0;
	virtual uint32_t GetSize() = 0;

	/**
	 * \brief This packet's current state
	 */
	virtual PacketType GetType() = 0;

	virtual ns3::Time GetSentTime() = 0;

	/**
	 * \brief The returned value should only be legitimate if this packet is delivered.
	 *
	 * \see PacketType
	 */
	virtual ns3::Time GetDeliveredTime() = 0;

	virtual void Print(ostream &os) const = 0;
};

inline ostream & operator <<(ostream & os, const PacketInfo & pInfo) {
	pInfo.Print(os);
	return os;
}

} /* namespace simul */

#endif /* OUTPUT_PACKETINFO_H_ */
