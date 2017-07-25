/*
 * EventInputTest.cpp
 *
 *  Created on: May 8, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../TestSuiteSwitches.h"
#ifdef EventInputTestSuite_ENABLE
#define EventInputTestSuite_SWITCH	*boost::unit_test::enabled()
#else
#define EventInputTestSuite_SWITCH	*boost::unit_test::disabled()
#endif

#include "../../Simul.h"
using namespace simul;

string GetProjectSimulDirectory();

BOOST_AUTO_TEST_SUITE(EventInputTestSuite, EventInputTestSuite_SWITCH)

BOOST_AUTO_TEST_CASE(EventInputTestwRun_n_Output01) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case1.p2pnetml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case1a_events.json",
			si);

	// configure simple output
	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	// check
	BOOST_REQUIRE_EQUAL(eventList.size(), 20);
	BOOST_REQUIRE_NO_THROW(si->Run());

	// out list
	uint64_t totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(OUTGOING_LIST);
	VectorPtr<PacketInfoPtr> outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);

	// delivered list
	totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(DELIVERED_LIST);
	outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);
}

BOOST_AUTO_TEST_CASE(EventInputTestwRun_n_Output02) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case1b.p2pnetml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case1b_events.json",
			si);

	// configure simple output
	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	// check
	BOOST_REQUIRE_EQUAL(eventList.size(), 20);
	BOOST_REQUIRE_NO_THROW(si->Run());

	// out list
	uint64_t totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(OUTGOING_LIST);
	VectorPtr<PacketInfoPtr> outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_LT(2, totalReadBytes / 2);
	BOOST_REQUIRE_LT(totalReadBytes / 2, totalSentBytes);
	BOOST_REQUIRE_LT(totalSentBytes, totalReadBytes);	// packet loss due to small MTU

	// delivered list
	totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(DELIVERED_LIST);
	outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_LT(2, totalReadBytes / 2);
	BOOST_REQUIRE_LT(totalReadBytes / 2, totalSentBytes);
	BOOST_REQUIRE_LT(totalSentBytes, totalReadBytes);	// packet loss due to small MTU
}

BOOST_AUTO_TEST_CASE(EventInputTestwRun_n_Output03) {	// tcp, udp, udptest
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case1.p2pnetml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case2a_events.json",
			si);

	// configure simple output
	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	// check
	BOOST_REQUIRE_EQUAL(eventList.size(), 20);
	BOOST_REQUIRE_NO_THROW(si->Run());

	// out list
	uint64_t totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(OUTGOING_LIST);
	VectorPtr<PacketInfoPtr> outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);

	// delivered list
	totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(DELIVERED_LIST);
	outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);
}

BOOST_AUTO_TEST_CASE(EventInputTestwRun_n_Output04) {	// only udptest
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case1b.p2pnetml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case2b_events.json",
			si);

	// configure simple output
	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	// check
	BOOST_REQUIRE_EQUAL(eventList.size(), 20);
	BOOST_REQUIRE_NO_THROW(si->Run());

	out.SetListType(DELIVERED_LIST);
	VectorPtr<PacketInfoPtr> list = out.GetPacketInfoList();
	uint32_t totalBytes = 0;
	for (auto pack : *list)
		totalBytes += pack->GetSize();

	BOOST_REQUIRE_LE(20 * eventList[0].lock()->GetTotalDataSize(), totalBytes);
	BOOST_REQUIRE_LE(totalBytes, 20 * (eventList[0].lock()->GetTotalDataSize() + 8));
}

BOOST_AUTO_TEST_CASE(EventInputTestwRun_n_Output05) { // unconnected nodes
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case3.p2pnetml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case2a_events.json",
			si);

	uint64_t sum = 0;
	for (auto event : eventList)
		sum += event.lock()->GetTotalDataSize();

	// check
	BOOST_REQUIRE_EQUAL(eventList.size(), 20);
	BOOST_REQUIRE_GE(sum, 20 * 1024);
	BOOST_REQUIRE_NO_THROW(si->Run());
}

BOOST_AUTO_TEST_CASE(EventInputAttributesTest) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case1.p2pnetml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case1a_events.json",
			si);

	uint nTcp = 0;
	uint nUdp = 0;
	for (weak_ptr<Event> event : eventList)
		if (dynamic_pointer_cast<TcpAppEvent>(event.lock()))
			nTcp++;
		else if (dynamic_pointer_cast<UdpAppEvent>(event.lock()))
			nUdp++;
	BOOST_REQUIRE_EQUAL(nTcp, 15);
	BOOST_REQUIRE_EQUAL(nUdp, 5);

	//
	// 2nd pair of files
	//
	SimulationPtr si2 = make_shared<Simulation>();
	si2->AddEdges(input.ReadGraph(path + "/graphfiles/use_case1b.p2pnetml"));
	si2->SetUpNetworks();

	eventList = in.ReadEvent(path + "/eventfiles/use_case1b_events.json", si2);

	nTcp = 0;
	nUdp = 0;
	for (weak_ptr<Event> event : eventList)
		if (dynamic_pointer_cast<TcpAppEvent>(event.lock()))
			nTcp++;
		else if (dynamic_pointer_cast<UdpAppEvent>(event.lock()))
			nUdp++;
	BOOST_REQUIRE_EQUAL(nTcp, 9);
	BOOST_REQUIRE_EQUAL(nUdp, 11);
}

//
// events for graphml
//
BOOST_AUTO_TEST_CASE(EventInputTestwRun_n_Output06) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	GraphMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case4a.graphml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case4a_events.json",
			si);

	// configure simple output
	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	// check
	BOOST_REQUIRE_EQUAL(eventList.size(), 20);
	BOOST_REQUIRE_NO_THROW(si->Run());

	// out list
	uint64_t totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(OUTGOING_LIST);
	VectorPtr<PacketInfoPtr> outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);

	// delivered list
	totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(DELIVERED_LIST);
	outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);
}

BOOST_AUTO_TEST_CASE(EventInputTestwRun_n_Output07) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	GraphMLInput input;
	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(input.ReadGraph(path + "/graphfiles/use_case4b.graphml"));
	si->SetUpNetworks();

	// read event file
	EventInput in;
	vector<weak_ptr<Event>> eventList = in.ReadEvent(path + "/eventfiles/use_case4b_events.json",
			si);

	// configure simple output
	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	// check
	BOOST_REQUIRE_EQUAL(eventList.size(), 20);
	BOOST_REQUIRE_NO_THROW(si->Run());

	// out list
	uint64_t totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(OUTGOING_LIST);
	VectorPtr<PacketInfoPtr> outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);

	// delivered list
	totalSentBytes = 0, totalReadBytes = 0;

	out.SetListType(DELIVERED_LIST);
	outList = out.GetPacketInfoList();
	for (auto pack : *outList)
		totalSentBytes += pack->GetSize();
	for (weak_ptr<Event> event : eventList)
		totalReadBytes += event.lock()->GetTotalDataSize();

	BOOST_REQUIRE_GE(totalReadBytes, 1);
	BOOST_REQUIRE_GE(totalSentBytes, totalReadBytes);
}

BOOST_AUTO_TEST_SUITE_END()

