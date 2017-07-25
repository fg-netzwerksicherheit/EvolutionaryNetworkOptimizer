/*
 * GraphMLInputTest.cpp
 *
 *  Created on: Jun 17, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"

using namespace simul;

string GetProjectSimulDirectory();

BOOST_AUTO_TEST_CASE(GraphMLInputwEventTest) {
	string path = GetProjectSimulDirectory() + "/src/test/input/graphfiles";

	GraphMLInput input;
	Simulation si;
	si.AddEdges(input.ReadGraph(path + "/use_case4a.graphml"));
	si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(si.GetNode("comp0"), si.GetNode("comp3"));
	event.lock()->SetStartTime(0);

	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), false);
	si.Run();
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), true);
}

BOOST_AUTO_TEST_CASE(GraphMLInputAttributesTest) {
	string path = GetProjectSimulDirectory() + "/src/test/input/graphfiles";

	GraphMLInput input;
	SetPtr<EdgePtr> edges = input.ReadGraph(path + "/use_case4a.graphml");
	for (auto it = edges->begin(); it != edges->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		if (edge->GetSourceNode()->GetID() == "comp3" && edge->GetDestNode()->GetID() == "comp0") {
			BOOST_REQUIRE_EQUAL(ns3::DataRate(edge->GetDataRateSrc()), ns3::DataRate("100Mbps"));
			BOOST_REQUIRE_EQUAL(ns3::DataRate(edge->GetDataRateDst()), ns3::DataRate("1000Mbps"));
			BOOST_REQUIRE_EQUAL(edge->GetMTUSrc(), 1122);
			BOOST_REQUIRE_EQUAL(edge->GetMTUDst(), 1567);
			BOOST_REQUIRE_EQUAL(ns3::Time(edge->GetInterframeGapSrc()), ns3::Time("96ns"));
			BOOST_REQUIRE_EQUAL(ns3::Time(edge->GetInterframeGapDst()), ns3::Time("69ns"));
			BOOST_REQUIRE_EQUAL(edge->GetDelay(), "213ms");
			break;
		}
	}
}

