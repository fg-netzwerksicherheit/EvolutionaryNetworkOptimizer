/*
 * Ipv4PacketInfoOutput.h
 *
 *  Created on: May 1, 2017
 *      Author: Kiet Vo
 */

#ifndef OUTPUT_IPV4PACKETINFOOUTPUT_H_
#define OUTPUT_IPV4PACKETINFOOUTPUT_H_

#include "IPacketInfoOutput.h"

namespace simul {

/**
 * Types for packet lists
 */
enum Ipv4PIOListType {
	OUTGOING_LIST,	//!< all packets that have been sent
	DROPPED_LIST,	//!< all packets that have been dropped
	DELIVERED_LIST	//!< all packets that have been delivered
};

/**
 * \ingroup output
 *
 * \brief Implementation of an output system on the Ipv4 protocol at layer 3.
 *
 * This class requires TimestampTag in order to capture the flow of packets.
 *
 * The callback system it employed relies on the ns-3 simulator's configuration and will
 * become invalid when the simulator is configured anew.
 * Users will need to re-activate the callback functions when that happens.
 *
 * Example:
 * \code
 * Simulation si;
 * ...;
 * Ipv4PacketInfoOutput out;
 * out.EnableCbLocalDeliver();
 * ...;
 * si.SetUpNetworks();
 * ...;
 * out.EnableCbLocalDeliver();	// must re-enable this
 * \endcode
 *
 * \see OnOffApp
 * \see Simulation::SetUpNetworks()
 * \see ns3::Ipv4L3Protocol
 */
class Ipv4PacketInfoOutput: public IPacketInfoOutput {
private:
	VectorPtr<PacketInfoPtr> outgoingList;
	VectorPtr<PacketInfoPtr> droppedList;
	VectorPtr<PacketInfoPtr> deliveredList;

	Ipv4PIOListType m_type = DELIVERED_LIST;

	void CbTx(ns3::Ptr<const ns3::Packet> packet, ns3::Ptr<ns3::Ipv4> ipv4, uint32_t interface);
	void CbDrop(const ns3::Ipv4Header &header, ns3::Ptr<const ns3::Packet> packet,
			ns3::Ipv4L3Protocol::DropReason reason, ns3::Ptr<ns3::Ipv4> ipv4, uint32_t interface);
	void CbSendOutgoing(const ns3::Ipv4Header &header, ns3::Ptr<const ns3::Packet> packet,
			uint32_t interface);
	void CbLocalDeliver(const ns3::Ipv4Header &header, ns3::Ptr<const ns3::Packet> packet,
			uint32_t interface);

public:
	Ipv4PacketInfoOutput();
	virtual ~Ipv4PacketInfoOutput();

	void SetListType(Ipv4PIOListType p_type);
	virtual void ClearPacketInfoList() override;
	virtual VectorPtr<PacketInfoPtr> GetPacketInfoList() override;

	void EnableCbTx();
	void EnableCbDrop();
	void EnableCbSendOutgoing();
	void EnableCbUnicastForward();
	void EnableCbLocalDeliver();

	void DisableCbTx();
	void DisableCbDrop();
	void DisableCbSendOutgoing();
	void DisableCbUnicastForward();
	void DisableCbLocalDeliver();

	void DisableCbAll();
}
;

} /* namespace simul */

#endif /* OUTPUT_IPV4PACKETINFOOUTPUT_H_ */
