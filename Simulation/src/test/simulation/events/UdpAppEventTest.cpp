/*
 * UdpAppEventTest.cpp
 *
 *  Created on: May 10, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../../Simul.h"

using namespace simul;

BOOST_AUTO_TEST_CASE(UdpAppEventTest) {
	NodePtr node1 = make_shared<simul::Node>();
	NodePtr node2 = make_shared<simul::Node>();
	NodePtr node3 = make_shared<simul::Node>();
	NodePtr node4 = make_shared<simul::Node>();

	EdgePtr edge1 = make_shared_base_in_simul(Edge, P2PEdge, node1, node2);
	EdgePtr edge2 = make_shared_base_in_simul(Edge, P2PEdge, node2, node3);
	EdgePtr edge3 = make_shared_base_in_simul(Edge, P2PEdge, node3, node4);

	Simulation si;
	si.AddEdge(edge1);
	si.AddEdge(edge2);
	si.AddEdge(edge3);
	si.SetUpNetworks();

	uint bytes = 20 * 1024 * 1024;
	weak_ptr<Event> event = si.CreateEvent<UdpAppEvent>(node1, node4, bytes);
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
}


