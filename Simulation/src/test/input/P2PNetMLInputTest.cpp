/*
 * P2PNetMLInputTest.cpp
 *
 *  Created on: May 5, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"
using namespace simul;

string GetProjectSimulDirectory();

BOOST_AUTO_TEST_CASE(P2PNetMLInputwEventTest) {
	string path = GetProjectSimulDirectory() + "/src/test/input/graphfiles";

	P2PNetMLInput input;
	Simulation si;
	si.AddEdges(input.ReadGraph(path + "/use_case1.p2pnetml"));
	si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(si.GetNode("n1"), si.GetNode("n5"));
	event.lock()->SetStartTime(0);

	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), false);
	si.Run();
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), true);

	//
	// new file
	//
	Simulation si2;
	si2.AddEdges(input.ReadGraph(path + "/use_case1b.p2pnetml"));
	si2.SetUpNetworks();

	event = si2.CreateEvent<UdpTestEvent>(si2.GetNode("n1"), si2.GetNode("n4"));
	event.lock()->SetStartTime(0);

	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), false);
	si2.Run();
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), true);
}

BOOST_AUTO_TEST_CASE(P2PNetMLInputAttributesTest) {
	string path = GetProjectSimulDirectory() + "/src/test/input/graphfiles";

	P2PNetMLInput input;
	SetPtr<EdgePtr> edges = input.ReadGraph(path + "/use_case1.p2pnetml");
	for (auto it = edges->begin(); it != edges->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		if (edge->GetSourceNode()->GetID() == "n0" && edge->GetDestNode()->GetID() == "n4") {
			BOOST_REQUIRE_EQUAL(ns3::DataRate(edge->GetDataRateSrc()), ns3::DataRate("100Mbps"));
			BOOST_REQUIRE_EQUAL(ns3::DataRate(edge->GetDataRateDst()), ns3::DataRate("1000Mbps"));
			BOOST_REQUIRE_EQUAL(edge->GetMTUSrc(), 1122);
			BOOST_REQUIRE_EQUAL(edge->GetMTUDst(), 1567);
			BOOST_REQUIRE_EQUAL(ns3::Time(edge->GetInterframeGapSrc()), ns3::Time("96ns"));
			BOOST_REQUIRE_EQUAL(ns3::Time(edge->GetInterframeGapDst()), ns3::Time("69ns"));
			BOOST_REQUIRE_EQUAL(edge->GetDelay(), "2ms");
			break;
		}
	}

	//
	// new file
	//
	edges = input.ReadGraph(path + "/use_case1b.p2pnetml");
	for (auto it = edges->begin(); it != edges->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		if (edge->GetSourceNode()->GetID() == "n4" && edge->GetDestNode()->GetID() == "n5") {
			BOOST_REQUIRE_EQUAL(ns3::DataRate(edge->GetDataRateSrc()), ns3::DataRate("1000Mbps"));
			BOOST_REQUIRE_EQUAL(ns3::DataRate(edge->GetDataRateDst()), ns3::DataRate("100kbps"));
			BOOST_REQUIRE_EQUAL(edge->GetMTUSrc(), 100);
			BOOST_REQUIRE_EQUAL(edge->GetMTUDst(), 500);
			BOOST_REQUIRE_EQUAL(ns3::Time(edge->GetInterframeGapSrc()), ns3::Time("96ns"));
			BOOST_REQUIRE_EQUAL(ns3::Time(edge->GetInterframeGapDst()), ns3::Time("96ns"));
			BOOST_REQUIRE_EQUAL(edge->GetDelay(), "56ms");
			break;
		}
	}
}

