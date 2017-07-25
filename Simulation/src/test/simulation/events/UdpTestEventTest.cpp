/*
 * UdpTestEventTest.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../TestSuiteSwitches.h"
#ifdef UdpTestEventTestSuite_ENABLE
#define UdpTestEventTestSuite_SWITCH	*boost::unit_test::enabled()
#else
#define UdpTestEventTestSuite_SWITCH	*boost::unit_test::disabled()
#endif

#include "EventFixtures.h"

BOOST_AUTO_TEST_SUITE(UdpTestEventTestSuite, UdpTestEventTestSuite_SWITCH)

BOOST_AUTO_TEST_CASE(UdpTestEventExceptionTest) {
	NodePtr node = std::make_shared<simul::Node>();
	NodePtr nullNode;
	Simulation si;

	// null
	BOOST_CHECK_THROW(si.CreateEvent<UdpTestEvent>(node, nullNode), NullPointerException);
	BOOST_CHECK_THROW(si.CreateEvent<UdpTestEvent>(nullNode, node), NullPointerException);
	BOOST_CHECK_THROW(si.CreateEvent<UdpTestEvent>(nullNode, nullNode), NullPointerException);

	// invalid argument
	BOOST_CHECK_THROW(si.CreateEvent<UdpTestEvent>(node, node), invalid_argument);
}

BOOST_FIXTURE_TEST_CASE(UdpTestEventP2PTest01, EventFixture_P2P) {	// P2P
	successful = false;
	Run01<UdpTestEvent>(2, false);
	BOOST_REQUIRE_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(2, true);
	BOOST_REQUIRE_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(70, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(70, true);
	BOOST_CHECK_EQUAL(successful, true);
}

BOOST_FIXTURE_TEST_CASE(UdpTestEventP2PTest02, EventFixture_P2P) {
	successful = false;
	Run02<UdpTestEvent>(2, false);
	BOOST_REQUIRE_EQUAL(successful, true);

	successful = false;
	Run02<UdpTestEvent>(2, true);
	BOOST_REQUIRE_EQUAL(successful, true);

	successful = false;
	Run02<UdpTestEvent>(20, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run02<UdpTestEvent>(20, true);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run02<UdpTestEvent>(70, false);
	BOOST_CHECK_EQUAL(successful, false);	// expect to fail
}

BOOST_FIXTURE_TEST_CASE(UdpTestEventP2PTest03, EventFixture_P2P) {
	successful = true;
	BOOST_CHECK_NO_THROW(Run03<UdpTestEvent>(false));
	BOOST_CHECK_EQUAL(successful, false);

	successful = true;
	BOOST_CHECK_NO_THROW(Run03<UdpTestEvent>(true));
	BOOST_CHECK_EQUAL(successful, false);
}

BOOST_FIXTURE_TEST_CASE(UdpTestEventWifiTest01, EventFixture_Wifi) {	// Wifi
	successful = false;
	Run01<UdpTestEvent>(2, 1, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(2, 1, true);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(31, 11, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(31, 11, true);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(97, 50, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run01<UdpTestEvent>(97, 50, true);
	BOOST_CHECK_EQUAL(successful, true);
}

BOOST_FIXTURE_TEST_CASE(UdpTestEventWifiTest02, EventFixture_Wifi) {
	successful = false;
	Run02<UdpTestEvent>(2, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run02<UdpTestEvent>(2, true);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run02<UdpTestEvent>(61, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run02<UdpTestEvent>(61, true);
	BOOST_CHECK_EQUAL(successful, true);
}

BOOST_FIXTURE_TEST_CASE(UdpTestEventMixedTest, EventFixture_Mixed) {
	successful = false;
	Run<UdpTestEvent>(2, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run<UdpTestEvent>(2, true);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run<UdpTestEvent>(61, false);
	BOOST_CHECK_EQUAL(successful, true);

	successful = false;
	Run<UdpTestEvent>(61, true);
	BOOST_CHECK_EQUAL(successful, true);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE(UdpTestEventSimpleTest) {	// test reschedule() XXX:

// topology
	NodePtr node1 = make_shared<Node>();
	NodePtr node2 = make_shared<Node>();
	EdgePtr edge = make_shared<P2PEdge>(node1, node2);

	Simulation si;
	si.AddEdge(edge);
	si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(node1, node2);

	Ipv4PacketInfoOutput out;
	out.EnableCbLocalDeliver();
	out.SetListType(DELIVERED_LIST);
	VectorPtr<PacketInfoPtr> list;

	uint64_t totalBytes_1 = 0, totalBytes_2 = 0, totalBytes_3 = 0, totalBytes_4 = 0;

	//
	// case 1
	//
	out.ClearPacketInfoList();

//	event.lock()->SetUp();
//	event.lock()->Schedule(1.1);
//	event.lock()->ReSetUpAndSchedule();
	event.lock()->SetStartTime(1.1);
	si.Run();

	list = out.GetPacketInfoList();
	for (auto pack : *list)
		totalBytes_1 += pack->GetSize();

	//
	// case 2
	//
	out.ClearPacketInfoList();

//	event.lock()->SetUp();
//	event.lock()->Schedule(2);
	event.lock()->SetStartTime(2);

	// test before run
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), false);

	si.Run();

	list = out.GetPacketInfoList();
	for (auto pack : *list)
		totalBytes_2 += pack->GetSize();

	// test after run
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), true);
	BOOST_REQUIRE_LT(event.lock()->GetTotalDataSize(), totalBytes_2);
	BOOST_REQUIRE_EQUAL(totalBytes_2, event.lock()->GetTotalDataSize() + 8);
	BOOST_REQUIRE_EQUAL(totalBytes_1, totalBytes_2);

	//
	// case 3 (continuation of 2)
	//
	out.ClearPacketInfoList();

//	event.lock()->SetUp();
//	event.lock()->ReSetUpAndSchedule();
	si.Run();

	list = out.GetPacketInfoList();
	for (auto pack : *list)
		totalBytes_3 += pack->GetSize();

	// test after run
	BOOST_REQUIRE_EQUAL(totalBytes_3, totalBytes_1);

	//
	// case 4 (continuation of 3)
	//
	out.ClearPacketInfoList();

//	event.lock()->Schedule(2);
	event.lock()->SetStartTime(2);
	si.Run();
//	for (uint i = 0; i < 20; i++)
//		event.lock()->ReSetUpAndSchedule();

	// test before run
	event.lock()->SetStartTime(0.2);
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), false);

	si.Run();

	list = out.GetPacketInfoList();
	for (auto pack : *list)
		totalBytes_4 += pack->GetSize();

	// test after run
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), true);
	BOOST_REQUIRE_EQUAL(totalBytes_4, 2 * totalBytes_1);
}

BOOST_AUTO_TEST_CASE(UdpTestEventNewPortNumberTest) {
	NodePtr n1 = make_shared<simul::Node>();
	NodePtr n2 = make_shared<simul::Node>();

	EdgePtr edge = make_shared<P2PEdge>(n1, n2);

	Simulation si;
	si.AddEdge(edge);
	si.SetUpNetworks();

	weak_ptr<Event> event1 = si.CreateEvent<UdpTestEvent>(n1, n2, false);
	weak_ptr<Event> event2 = si.CreateEvent<UdpTestEvent>(n1, n2, false);
	weak_ptr<Event> event3 = si.CreateEvent<UdpTestEvent>(n1, n2, false);

	event1.lock()->SetStartTime(1);
	event2.lock()->SetStartTime(1);
	event3.lock()->SetStartTime(1);

	//
	bool correct = !(event1.lock()->IsSuccessful() || event2.lock()->IsSuccessful()
			|| event3.lock()->IsSuccessful());
	BOOST_REQUIRE(correct);

	//
	si.Run();
	correct = event1.lock()->IsSuccessful() && event2.lock()->IsSuccessful()
			&& event3.lock()->IsSuccessful();
	BOOST_REQUIRE(correct);
}

