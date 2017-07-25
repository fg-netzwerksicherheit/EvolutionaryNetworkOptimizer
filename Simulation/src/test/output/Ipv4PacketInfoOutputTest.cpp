/*
 * Ipv4PacketInfoOutputTest.cpp
 *
 *  Created on: May 4, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"

using namespace simul;

BOOST_AUTO_TEST_CASE(Ipv4PacketInfoOutputNormalTest) {
	NodePtr n1 = make_shared<simul::Node>();
	NodePtr n2 = make_shared<simul::Node>();
	EdgePtr edge = make_shared_base_in_simul(Edge, P2PEdge, n1, n2);

	Simulation si;
	si.AddEdge(edge);
	si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<TcpAppEvent>(n1, n2, 2000);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
		event.lock()->SetStartTime(0);

	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	si.Run();

	out.SetListType(OUTGOING_LIST);
	VectorPtr<PacketInfoPtr> outgoingList = out.GetPacketInfoList();
	out.SetListType(DELIVERED_LIST);
	VectorPtr<PacketInfoPtr> deliveredList = out.GetPacketInfoList();

	BOOST_REQUIRE_GE(outgoingList->size(), deliveredList->size());
	BOOST_REQUIRE_GT(deliveredList->size(), 0);

	for (auto pack : *outgoingList)
		BOOST_REQUIRE_EQUAL(pack->GetType(), PACKET_SENT);
	for (auto pack : *deliveredList) {
		BOOST_REQUIRE_EQUAL(pack->GetType(), PACKET_DELIVERED);
		BOOST_REQUIRE_GE((pack->GetDeliveredTime() - pack->GetSentTime()).GetSeconds(), 0);
	}

	// clean up
	out.ClearPacketInfoList();
	BOOST_REQUIRE_EQUAL(outgoingList->size(), 0);
	BOOST_REQUIRE_EQUAL(deliveredList->size(), 0);

	//
	// with UdpApp
	//
	event = si.CreateEvent<UdpAppEvent>(n1, n2, 2000);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
		event.lock()->SetStartTime(0);

	si.Run();

	BOOST_REQUIRE_GE(outgoingList->size(), deliveredList->size());
	BOOST_REQUIRE_GT(deliveredList->size(), 0);

	for (auto pack : *outgoingList)
		BOOST_REQUIRE_EQUAL(pack->GetType(), PACKET_SENT);
	for (auto pack : *deliveredList) {
		BOOST_REQUIRE_EQUAL(pack->GetType(), PACKET_DELIVERED);
		BOOST_REQUIRE_GE((pack->GetDeliveredTime() - pack->GetSentTime()).GetSeconds(), 0);
	}
}

BOOST_AUTO_TEST_CASE(Ipv4PacketInfoOutputExtremeTest) {
	NodePtr n1 = make_shared<simul::Node>();
	NodePtr n2 = make_shared<simul::Node>();
	EdgePtr edge = make_shared_base_in_simul(Edge, P2PEdge, n1, n2);

	Simulation si;
	si.AddEdge(edge);
	si.SetUpNetworks();

	uint bytes = 2 * 1000 * 1000;
	weak_ptr<Event> event = si.CreateEvent<TcpAppEvent>(n1, n2, bytes);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
		event.lock()->SetStartTime(0);

	Ipv4PacketInfoOutput out;
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	si.Run();

	out.SetListType(DELIVERED_LIST);
	VectorPtr<PacketInfoPtr> deliveredList = out.GetPacketInfoList();

	uint sum = 0;
	for (auto pack : *deliveredList) {
		sum += pack->GetSize();
	}
	BOOST_REQUIRE_GE(sum, bytes);

	// clean up
	out.ClearPacketInfoList();
	BOOST_REQUIRE_EQUAL(deliveredList->size(), 0);

	//
	// with Udp
	//
	event = si.CreateEvent<UdpAppEvent>(n1, n2, bytes);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
		event.lock()->SetStartTime(0);

	si.Run();

	sum = 0;
	for (auto pack : *deliveredList) {
		sum += pack->GetSize();
	}
	BOOST_REQUIRE_GE(sum, bytes);
}

