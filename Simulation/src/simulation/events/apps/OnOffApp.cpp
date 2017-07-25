/*
 * MyApp.cpp
 *
 *  Created on: Apr 27, 2017
 *      Author: Quang Pham
 */

#include "../tags/TimestampTag.h"
#include "OnOffApp.h"

namespace simul {

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("simul::OnOffApp");

NS_OBJECT_ENSURE_REGISTERED(OnOffApp);

TypeId OnOffApp::GetTypeId(void) {
	static TypeId tid =
			TypeId("simul::OnOffApp").SetParent<Application>().SetGroupName("Applications").AddConstructor<
					OnOffApp>().AddAttribute("DataRate", "The data rate in on state.",
					DataRateValue(DataRate("1Mb/s")), MakeDataRateAccessor(&OnOffApp::m_cbrRate),
					MakeDataRateChecker()).AddAttribute("PacketSize",
					"The size of packets sent in on state", UintegerValue(512),
					MakeUintegerAccessor(&OnOffApp::m_pktSize), MakeUintegerChecker<uint32_t>(1)).AddAttribute(
					"Remote", "The address of the destination", AddressValue(),
					MakeAddressAccessor(&OnOffApp::m_peer), MakeAddressChecker()).AddAttribute(
					"OnTime", "A RandomVariableStream used to pick the duration of the 'On' state.",
					StringValue("ns3::ConstantRandomVariable[Constant=1.0]"),
					MakePointerAccessor(&OnOffApp::m_onTime),
					MakePointerChecker<RandomVariableStream>()).AddAttribute("OffTime",
					"A RandomVariableStream used to pick the duration of the 'Off' state.",
					StringValue("ns3::ConstantRandomVariable[Constant=1.0]"),
					MakePointerAccessor(&OnOffApp::m_offTime),
					MakePointerChecker<RandomVariableStream>()).AddAttribute("MaxBytes",
					"The total number of bytes to send. Once these bytes are sent, "
							"no packet is sent again, even in on state. The value zero means "
							"that there is no limit.", UintegerValue(0),
					MakeUintegerAccessor(&OnOffApp::m_maxBytes), MakeUintegerChecker<uint64_t>()).AddAttribute(
					"Protocol", "The type of protocol to use.",
					TypeIdValue(UdpSocketFactory::GetTypeId()), MakeTypeIdAccessor(&OnOffApp::m_tid),
					MakeTypeIdChecker()).AddTraceSource("Tx", "A new packet is created and is sent",
					MakeTraceSourceAccessor(&OnOffApp::m_txTrace), "ns3::Packet::TracedCallback");
	return tid;
}

OnOffApp::OnOffApp() :
		m_socket(0), m_connected(false), m_residualBits(0), m_lastStartTime(Seconds(0)), m_totBytes(
				0) {
	NS_LOG_FUNCTION(this);
}

OnOffApp::~OnOffApp() {
	NS_LOG_FUNCTION(this);
}

void OnOffApp::SetMaxBytes(uint64_t maxBytes) {
	NS_LOG_FUNCTION(this << maxBytes);
	m_maxBytes = maxBytes;
}

Ptr<Socket> OnOffApp::GetSocket(void) const {
	NS_LOG_FUNCTION(this);
	return m_socket;
}

int64_t OnOffApp::AssignStreams(int64_t stream) {
	NS_LOG_FUNCTION(this << stream);
	m_onTime->SetStream(stream);
	m_offTime->SetStream(stream + 1);
	return 2;
}

void OnOffApp::DoDispose(void) {
	NS_LOG_FUNCTION(this);

	m_socket = 0;
	// chain up
	Application::DoDispose();
}

// Application Methods
void OnOffApp::StartApplication() // Called at time specified by Start
{
	NS_LOG_FUNCTION(this);

	// Create the socket if not already
	if (!m_socket) {
		m_socket = Socket::CreateSocket(GetNode(), m_tid);
		if (Inet6SocketAddress::IsMatchingType(m_peer)) {
			m_socket->Bind6();
		} else if (InetSocketAddress::IsMatchingType(m_peer)
				|| PacketSocketAddress::IsMatchingType(m_peer)) {
			m_socket->Bind();
		}
		m_socket->Connect(m_peer);
		m_socket->SetAllowBroadcast(true);
		m_socket->ShutdownRecv();

		m_socket->SetConnectCallback(MakeCallback(&OnOffApp::ConnectionSucceeded, this),
				MakeCallback(&OnOffApp::ConnectionFailed, this));
	}
	m_cbrRateFailSafe = m_cbrRate;

	// Insure no pending event
	CancelEvents();
	// If we are not yet connected, there is nothing to do here
	// The ConnectionComplete upcall will start timers at that time
	//if (!m_connected) return;
	ScheduleStartEvent();
}

void OnOffApp::StopApplication() // Called at time specified by Stop
{
	NS_LOG_FUNCTION(this);

	CancelEvents();
	if (m_socket != 0) {
		m_socket->Close();
	} else {
		NS_LOG_WARN("MyApp found null socket to close in StopApplication");
	}
}

void OnOffApp::CancelEvents() {
	NS_LOG_FUNCTION(this);

	if (m_sendEvent.IsRunning() && m_cbrRateFailSafe == m_cbrRate) { // Cancel the pending send packet event
																	 // Calculate residual bits since last packet sent
		Time delta(Simulator::Now() - m_lastStartTime);
		int64x64_t bits = delta.To(Time::S) * m_cbrRate.GetBitRate();
		m_residualBits += bits.GetHigh();
	}
	m_cbrRateFailSafe = m_cbrRate;
	Simulator::Cancel(m_sendEvent);
	Simulator::Cancel(m_startStopEvent);
}

// Event handlers
void OnOffApp::StartSending() {
	NS_LOG_FUNCTION(this);
	m_lastStartTime = Simulator::Now();
	ScheduleNextTx();  // Schedule the send packet event
	ScheduleStopEvent();
}

void OnOffApp::StopSending() {
	NS_LOG_FUNCTION(this);
	CancelEvents();

	ScheduleStartEvent();
}

// Private helpers
void OnOffApp::ScheduleNextTx() {
	NS_LOG_FUNCTION(this);

	if (m_maxBytes == 0 || m_totBytes < m_maxBytes) {
		uint32_t bits = m_pktSize * 8 - m_residualBits;
		NS_LOG_LOGIC("bits = " << bits);
		Time nextTime(Seconds(bits / static_cast<double>(m_cbrRate.GetBitRate()))); // Time till next packet
		NS_LOG_LOGIC("nextTime = " << nextTime);
		m_sendEvent = Simulator::Schedule(nextTime, &OnOffApp::SendPacket, this);
	} else { // All done, cancel any pending events
		StopApplication();
	}
}

void OnOffApp::ScheduleStartEvent() { // Schedules the event to start sending data (switch to the "On" state)
	NS_LOG_FUNCTION(this);

	Time offInterval = Seconds(m_offTime->GetValue());
	NS_LOG_LOGIC("start at " << offInterval);
	m_startStopEvent = Simulator::Schedule(offInterval, &OnOffApp::StartSending, this);
}

void OnOffApp::ScheduleStopEvent() { // Schedules the event to stop sending data (switch to "Off" state)
	NS_LOG_FUNCTION(this);

	Time onInterval = Seconds(m_onTime->GetValue());
	NS_LOG_LOGIC("stop at " << onInterval);
	m_startStopEvent = Simulator::Schedule(onInterval, &OnOffApp::StopSending, this);
}

void OnOffApp::SendPacket() {
	NS_LOG_FUNCTION(this);

	NS_ASSERT(m_sendEvent.IsExpired());
	Ptr<Packet> packet = Create<Packet>(m_pktSize);
	//SeqTsHeader header;
	//packet->AddHeader(header);

	simul::TimestampTag tag;
	tag.SetTimestamp(Simulator::Now());
	packet->AddPacketTag(tag);
	m_txTrace(packet);
	m_socket->Send(packet);
	m_totBytes += m_pktSize;
	//NS_LOG_INFO("Current time " << header.GetTs().GetNanoSeconds());
	if (InetSocketAddress::IsMatchingType(m_peer)) {
		NS_LOG_INFO(
				"At time " << Simulator::Now ().GetNanoSeconds () << "ns on-off application sent " << packet->GetSize () << " bytes to " << InetSocketAddress::ConvertFrom(m_peer).GetIpv4 () << " port " << InetSocketAddress::ConvertFrom (m_peer).GetPort () << " total Tx " << m_totBytes << " bytes");
	} else if (Inet6SocketAddress::IsMatchingType(m_peer)) {
		NS_LOG_INFO(
				"At time " << Simulator::Now ().GetSeconds () << "s on-off application sent " << packet->GetSize () << " bytes to " << Inet6SocketAddress::ConvertFrom(m_peer).GetIpv6 () << " port " << Inet6SocketAddress::ConvertFrom (m_peer).GetPort () << " total Tx " << m_totBytes << " bytes");
	}
	m_lastStartTime = Simulator::Now();
	m_residualBits = 0;
	ScheduleNextTx();
}

void OnOffApp::ConnectionSucceeded(Ptr<Socket> socket) {
	NS_LOG_FUNCTION(this << socket);
	m_connected = true;
}

void OnOffApp::ConnectionFailed(Ptr<Socket> socket) {
	NS_LOG_FUNCTION(this << socket);
}
}
/* namespace std */
