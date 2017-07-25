/*
 * P2PNetMLOutputTest.cpp
 *
 *  Created on: May 10, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"

using namespace simul;

string GetProjectSimulDirectory();

BOOST_AUTO_TEST_CASE(P2PNetMLOutputAttributesTest01) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	// 1st pair of lists
	P2PNetMLInput input;
	SetPtr<EdgePtr> eList1 = input.ReadGraph(path + "/graphfiles/use_case1.p2pnetml");
	set<NodePtr> nList1;
	for (auto it = eList1->begin(); it != eList1->end(); it++) {
		nList1.insert((*it)->GetSourceNode());
		nList1.insert((*it)->GetDestNode());
	}

	// 2nd pair of lists
	string filename = "P2PNetMLOutputAttributesTest.xml";
	P2PNetMLOutput out;
	out.WriteGraph(eList1, filename);

	SetPtr<EdgePtr> eList2 = input.ReadGraph(get_current_dir_name() + string("/") + filename);
	set<NodePtr> nList2;
	for (auto it = eList2->begin(); it != eList2->end(); it++) {
		nList2.insert((*it)->GetSourceNode());
		nList2.insert((*it)->GetDestNode());
	}

	// 1st pair == 2nd pair ?
	BOOST_REQUIRE_EQUAL(eList1->size(), eList2->size());
	BOOST_REQUIRE_EQUAL(nList1.size(), nList2.size());

	uint16_t sumMtu1 = 0, sumMtu2 = 0;
	ns3::Time sumDelay1, sumDelay2, sumIg1, sumIg2;
	uint64_t sumDr1 = 0, sumDr2 = 0;
	for (auto it = eList1->begin(); it != eList1->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		sumMtu1 += edge->GetMTUSrc() + edge->GetMTUDst();
		sumDelay1 += ns3::Time(edge->GetDelay());
		sumIg1 += ns3::Time(edge->GetInterframeGapSrc()) + ns3::Time(edge->GetInterframeGapDst());
		sumDr1 += ns3::DataRate(edge->GetDataRateSrc()).GetBitRate()
				+ ns3::DataRate(edge->GetDataRateDst()).GetBitRate();
	}
	for (auto it = eList2->begin(); it != eList2->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		sumMtu2 += edge->GetMTUSrc() + edge->GetMTUDst();
		sumDelay2 += ns3::Time(edge->GetDelay());
		sumIg2 += ns3::Time(edge->GetInterframeGapSrc()) + ns3::Time(edge->GetInterframeGapDst());
		sumDr2 += ns3::DataRate(edge->GetDataRateSrc()).GetBitRate()
				+ ns3::DataRate(edge->GetDataRateDst()).GetBitRate();
	}

	BOOST_REQUIRE_EQUAL(sumMtu1, sumMtu2);
	BOOST_REQUIRE_EQUAL(sumDelay1, sumDelay2);
	BOOST_REQUIRE_EQUAL(sumIg1, sumIg2);
	BOOST_REQUIRE_EQUAL(sumDr1, sumDr2);
}

BOOST_AUTO_TEST_CASE(P2PNetMLOutputAttributesTest02) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	// 1st pair of lists
	P2PNetMLInput input;
	SetPtr<EdgePtr> eList1 = input.ReadGraph(path + "/graphfiles/use_case1b.p2pnetml");
	set<NodePtr> nList1;
	for (auto it = eList1->begin(); it != eList1->end(); it++) {
		nList1.insert((*it)->GetSourceNode());
		nList1.insert((*it)->GetDestNode());
	}

	// 2nd pair of lists
	string filename = "P2PNetMLOutputAttributesTest.xml";
	P2PNetMLOutput out;
	out.WriteGraph(eList1, filename);

	SetPtr<EdgePtr> eList2 = input.ReadGraph(get_current_dir_name() + string("/") + filename);
	set<NodePtr> nList2;
	for (auto it = eList2->begin(); it != eList2->end(); it++) {
		nList2.insert((*it)->GetSourceNode());
		nList2.insert((*it)->GetDestNode());
	}

	// 1st pair == 2nd pair ?
	BOOST_REQUIRE_EQUAL(eList1->size(), eList2->size());
	BOOST_REQUIRE_EQUAL(nList1.size(), nList2.size());

	uint16_t sumMtu1 = 0, sumMtu2 = 0;
	ns3::Time sumDelay1, sumDelay2, sumIg1, sumIg2;
	uint64_t sumDr1 = 0, sumDr2 = 0;
	for (auto it = eList1->begin(); it != eList1->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		sumMtu1 += edge->GetMTUSrc() + edge->GetMTUDst();
		sumDelay1 += ns3::Time(edge->GetDelay());
		sumIg1 += ns3::Time(edge->GetInterframeGapSrc()) + ns3::Time(edge->GetInterframeGapDst());
		sumDr1 += ns3::DataRate(edge->GetDataRateSrc()).GetBitRate()
				+ ns3::DataRate(edge->GetDataRateDst()).GetBitRate();
	}
	for (auto it = eList2->begin(); it != eList2->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		sumMtu2 += edge->GetMTUSrc() + edge->GetMTUDst();
		sumDelay2 += ns3::Time(edge->GetDelay());
		sumIg2 += ns3::Time(edge->GetInterframeGapSrc()) + ns3::Time(edge->GetInterframeGapDst());
		sumDr2 += ns3::DataRate(edge->GetDataRateSrc()).GetBitRate()
				+ ns3::DataRate(edge->GetDataRateDst()).GetBitRate();
	}

	BOOST_REQUIRE_EQUAL(sumMtu1, sumMtu2);
	BOOST_REQUIRE_EQUAL(sumDelay1, sumDelay2);
	BOOST_REQUIRE_EQUAL(sumIg1, sumIg2);
	BOOST_REQUIRE_EQUAL(sumDr1, sumDr2);
}

BOOST_AUTO_TEST_CASE(P2PNetMLOutputwRunTest01) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SetPtr<EdgePtr> eList1 = input.ReadGraph(path + "/graphfiles/use_case1.p2pnetml");

	// write graph file
	string filename = "P2PNetMLOutputAttributesTest.xml";
	P2PNetMLOutput out;
	out.WriteGraph(eList1, filename);

	// use newly created graph file
	Simulation si;
	si.AddEdges(input.ReadGraph(get_current_dir_name() + string("/") + filename));
	si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(si.GetNode("n1"), si.GetNode("n5"));
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
	event.lock()->SetStartTime(0);

	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), false);
	si.Run();
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), true);
}

BOOST_AUTO_TEST_CASE(P2PNetMLOutputwRunTest02) {
	string path = GetProjectSimulDirectory() + "/src/test/input";

	P2PNetMLInput input;
	SetPtr<EdgePtr> eList1 = input.ReadGraph(path + "/graphfiles/use_case1b.p2pnetml");

	// write graph file
	string filename = "P2PNetMLOutputAttributesTest.xml";
	P2PNetMLOutput out;
	out.WriteGraph(eList1, filename);

	// use newly created graph file
	Simulation si;
	si.AddEdges(input.ReadGraph(get_current_dir_name() + string("/") + filename));
	si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(si.GetNode("n1"), si.GetNode("n4"));
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
	event.lock()->SetStartTime(0);

	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), false);
	si.Run();
	BOOST_REQUIRE_EQUAL(event.lock()->IsSuccessful(), true);
}

