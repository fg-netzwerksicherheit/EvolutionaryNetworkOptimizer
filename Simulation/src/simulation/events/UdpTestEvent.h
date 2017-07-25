/*
 * UdpTestEvent.h
 *
 *  Created on: Mar 27, 2017
 *      Author: Kiet Vo
 */

#ifndef EVENTS_UDPTESTEVENT_H_
#define EVENTS_UDPTESTEVENT_H_

#include "Event.h"

namespace simul {

/**
 * \ingroup events
 *
 * \brief Event implementation that uses a pair of udp sockets for communication.
 *
 * This class employs TimestampTag to record the sending time of each packet.
 * It is also reusable but the number of sockets on each node is limited due to the limited
 * number of ephemeral ports, typically ranging from 49152 to 65535.
 *
 */
class UdpTestEvent: public Event {
	friend class Simulation;

private:
	uint64_t maxBytes = 1024;
	ns3::Ptr<ns3::Socket> srcSocket = nullptr;
	ns3::Ptr<ns3::Socket> dstSocket = nullptr;
	uint16_t dstport = -1;

	const bool reply = false;
	bool successful = false;

	double startTime = -1;
	virtual bool IsValidToSetStartTime(double startTime) override;
	virtual void DoSetStartTime(double startTime) override;

	bool hasStartTime = false;
	virtual bool IsValidToSetUp() override;
	virtual void DoSetUp() override;

	virtual void NotifySimulatorDestroyed() override;

protected:
	UdpTestEvent(NodePtr sourceNode, NodePtr destNode, bool reply = false);

public:
	virtual ~UdpTestEvent();

	virtual uint64_t GetTotalDataSize() override;
	virtual bool IsSuccessful() override;
};

} /* namespace simul */

#endif /* EVENTS_UDPTESTEVENT_H_ */
