/*
 * MyApp.h
 *
 *  Created on: Apr 27, 2017
 *      Author: Quang Pham
 */

#ifndef SIMULATION_MYAPP_H_
#define SIMULATION_MYAPP_H_

#include "../../../Si_Type.h"

namespace simul {

/**
 * \ingroup events
 * \defgroup apps
 */

/**
 * \ingroup apps
 *
 * \brief An implementation for application that uses TimestampTag to record the sending
 * time of each packet.
 *
 * \attention
 * This application is not re-usable, an instance of this type can only run once
 * and only be disposed when the ns-3 simulator is destroyed.
 *
 * \see Simulation::SetUpNetworks()
 */
class OnOffApp: public ns3::Application {
public:
	static ns3::TypeId GetTypeId(void);

	OnOffApp();
	virtual ~OnOffApp();

	void SetMaxBytes(uint64_t maxBytes);
	ns3::Ptr<ns3::Socket> GetSocket(void) const;
	int64_t AssignStreams(int64_t stream);

protected:
	virtual void DoDispose(void);

private:
	// inherited from Application base class.
	virtual void StartApplication(void); // Called at time specified by Start
	virtual void StopApplication(void); // Called at time specified by Stop

	//helpers
	void CancelEvents();
	// Event handlers
	void StartSending();
	void StopSending();
	void SendPacket();
	ns3::Ptr<ns3::Socket> m_socket;
	ns3::Address m_peer;
	bool m_connected;
	ns3::Ptr<ns3::RandomVariableStream> m_onTime;
	ns3::Ptr<ns3::RandomVariableStream> m_offTime;
	ns3::DataRate m_cbrRate;
	ns3::DataRate m_cbrRateFailSafe;
	uint32_t m_pktSize;
	uint32_t m_residualBits;
	ns3::Time m_lastStartTime;
	uint64_t m_maxBytes;
	uint64_t m_totBytes;
	ns3::EventId m_startStopEvent;
	ns3::EventId m_sendEvent;
	ns3::TypeId m_tid;
	ns3::TracedCallback<ns3::Ptr<const ns3::Packet> > m_txTrace;

	void ScheduleNextTx();
	void ScheduleStartEvent();
	void ScheduleStopEvent();
	void ConnectionSucceeded(ns3::Ptr<ns3::Socket> socket);
	void ConnectionFailed(ns3::Ptr<ns3::Socket> socket);
};

}
#endif
/* namespace std */
