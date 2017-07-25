/*
 * SimulationTest.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../TestSuiteSwitches.h"
#ifdef SimulationTestSuite_ENABLE
#define SimulationTestSuite_SWITCH	*boost::unit_test::enabled()
#else
#define SimulationTestSuite_SWITCH	*boost::unit_test::disabled()
#endif

#include "../../Simul.h"
#include "../SimpleFixtures.h"

using namespace simul;

BOOST_AUTO_TEST_SUITE(SimulationTestSuite, SimulationTestSuite_SWITCH)

BOOST_AUTO_TEST_CASE(SimulationMembersTest) {

	NodePtr node1 = std::make_shared<simul::Node>();
	NodePtr node2 = std::make_shared<simul::Node>();

	Simulation si;

	node1->SetID("n01"), node2->SetID("n02");
	si.AddEdge(std::make_shared<P2PEdge>(node1, node2));

	// getNode()
	BOOST_CHECK(si.GetNode("n01") != NULL);
	BOOST_CHECK(si.GetNode("n02") != NULL);
	BOOST_CHECK(si.GetNode("n1") == NULL);

	// getNode() again
	node1->SetID("n001"), node2->SetID("n002");

	BOOST_CHECK(si.GetNode("n01") == NULL);
	BOOST_CHECK(si.GetNode("n02") == NULL);
	BOOST_CHECK(si.GetNode("n001") != NULL);
	BOOST_CHECK(si.GetNode("n002") != NULL);

	//
	// new scenario
	//
	node1 = std::make_shared<simul::Node>();
	node2 = std::make_shared<simul::Node>();

	// add an edge
	node1->SetID("n03"), node2->SetID("n04");
	si.AddEdge(std::make_shared<WifiEdge>(node1, node2));

	// add a bunch of edges
	SetPtr<EdgePtr> edges = std::make_shared<std::set<EdgePtr>>();

	for (int i = 0; i < 10; i++)
		edges->insert(
				std::make_shared<WifiEdge>(std::make_shared<simul::Node>(),
						std::make_shared<simul::Node>()));

	si.AddEdges(edges);

	for (int i = 0; i < 100; i++)
		si.SetUpNetworks();

	// getNode() again
	BOOST_CHECK(si.GetNode("n01") == NULL);
	BOOST_CHECK(si.GetNode("n02") == NULL);
	BOOST_CHECK(si.GetNode("n001") != NULL);
	BOOST_CHECK(si.GetNode("n002") != NULL);
	BOOST_CHECK(si.GetNode("n03") != NULL);
	BOOST_CHECK(si.GetNode("n03") != NULL);

	//
	// new scenario
	// FindDevicesFromNode()
	//
	ns3::NetDeviceContainer devices1 = si.FindDevicesFromNode(node1);
	ns3::NetDeviceContainer devices2 = si.FindDevicesFromNode(node2);
	ns3::Ptr<ns3::WifiNetDevice> wifi;

	wifi = ns3::DynamicCast<ns3::WifiNetDevice>(devices1.Get(0));
	BOOST_CHECK(wifi->GetPhy() != nullptr);

	wifi = ns3::DynamicCast<ns3::WifiNetDevice>(devices2.Get(0));
	BOOST_CHECK(wifi->GetPhy() != nullptr);
}

BOOST_AUTO_TEST_CASE(SimulationMembersTest_2) {
	weak_ptr<Event> event;
	{
		Simulation si;
		event = si.CreateEvent<UdpTestEvent>(make_shared<simul::Node>(),
				make_shared<simul::Node>());
		BOOST_REQUIRE(event.lock() != nullptr);
	}
	BOOST_REQUIRE(event.lock() == nullptr);
}

// Flush() functions test
BOOST_AUTO_TEST_CASE(SimpleFlushTest) {
	Simulation si;

	// scenario 1 (normal)
	NodePtr n1 = make_shared<simul::Node>();
	NodePtr n2 = make_shared<simul::Node>();
	EdgePtr e1 = make_shared_base_in_simul(Edge, P2PEdge, n1, n2);
	si.AddEdge(e1);

	for (int i = 0; i < 100; i++)
		si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(n1, n2);
//	event.lock()->SetUp();
//	event.lock()->Schedule(1.0);
	event.lock()->SetStartTime(1.0);

	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);
	si.Run(Event::GetRecommendedDuration());
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), true);

	// scenario 2 (new nodes and edges but not connected)
	NodePtr n3 = make_shared<simul::Node>();
	NodePtr n4 = make_shared<simul::Node>();
	EdgePtr e2 = make_shared_base_in_simul(Edge, P2PEdge, n3, n4);
	si.AddEdge(e2);

	for (int i = 0; i < 100; i++)
		si.SetUpNetworks();

	event = si.CreateEvent<UdpTestEvent>(n1, n4);
//	event.lock()->SetUp();
//	event.lock()->Schedule(1.0);
	event.lock()->SetStartTime(1.0);
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);
	si.Run(Event::GetRecommendedDuration());
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);

	// scenario 3 (connect n2 and n3)
	EdgePtr e3 = make_shared_base_in_simul(Edge, P2PEdge, n2, n3);
	si.AddEdge(e3);

	for (int i = 0; i < 100; i++)
		si.SetUpNetworks();

//	event.lock()->SetUp();
//	event.lock()->Schedule(1.0);
	event.lock()->SetStartTime(1.0);
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);
	si.Run(Event::GetRecommendedDuration());
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), true);
}

BOOST_FIXTURE_TEST_CASE(IPAssignmentTest, SimulationFixture) {
	uint nump2p = 254 * 64;
	for (uint i = 0; i < nump2p; i++)
		this->AddEdge(make_shared<P2PEdge>(make_shared<simul::Node>(), make_shared<simul::Node>()));

	// test ResetNetCount()
	for (int i = 0; i < 2; i++)
		BOOST_CHECK_NO_THROW(this->SetUpNetworks());

	this->AddEdge(make_shared<P2PEdge>(make_shared<simul::Node>(), make_shared<simul::Node>()));
	BOOST_CHECK_THROW(this->SetUpNetworks(), LimitationException);

	//
	// remove an edge to make this valid again
	this->globalEdgeList.erase(this->globalEdgeList.begin());

	uint numwifi = 254;
	for (uint i = 0; i < numwifi; i++)
		this->AddEdge(
				make_shared<WifiEdge>(make_shared<simul::Node>(), make_shared<simul::Node>()));

	// test ResetNetCount()
	for (int i = 0; i < 2; i++)
		BOOST_CHECK_NO_THROW(this->SetUpNetworks());

	this->AddEdge(make_shared<WifiEdge>(make_shared<simul::Node>(), make_shared<simul::Node>()));
	BOOST_CHECK_THROW(this->SetUpNetworks(), LimitationException);
}

BOOST_FIXTURE_TEST_CASE(TopologyAttributeTest, SimulationFixture) {
	shared_ptr<P2PEdge> edge = make_shared<P2PEdge>(make_shared<simul::Node>(),
			make_shared<simul::Node>());

	this->AddEdge(edge);

	for (int i = 0; i < 100; i++)
		this->SetUpNetworks();

	/**
	 *  set attributes for edge
	 */
	// data rate
	string drs = "23kbps", drd = "71Mbps";
	edge->SetDataRateSrc(drs);
	edge->SetDataRateDst(drd);

	// mtu
	uint16_t mtus = 12, mtud = 2000;
	edge->SetMTUSrc(mtus);
	edge->SetMTUDst(mtud);

	// interframe gap
	string times = "11ns", timed = "3s";
	edge->SetInterframeGapSrc(times);
	edge->SetInterframeGapDst(timed);

	// delay
	string delay = "7s";
	edge->SetDelay(delay);

	for (int i = 0; i < 100; i++)
		this->SetUpNetworks();

	/**
	 * test if attributes are configured successfully
	 */
	ns3::NetDeviceContainer deviceSrc = this->FindDevicesFromNode(edge->GetSourceNode());
	ns3::NetDeviceContainer deviceDst = this->FindDevicesFromNode(edge->GetDestNode());
	BOOST_CHECK_EQUAL(deviceSrc.GetN(), 1);
	BOOST_CHECK_EQUAL(deviceDst.GetN(), 1);
	BOOST_CHECK_EQUAL(deviceSrc.Get(0)->GetChannel(), deviceDst.Get(0)->GetChannel());

	// data rate
	ns3::DataRateValue vSrc;
	ns3::DataRateValue vDst;
	deviceSrc.Get(0)->GetAttribute("DataRate", vSrc);
	deviceDst.Get(0)->GetAttribute("DataRate", vDst);
	BOOST_CHECK_EQUAL(ns3::DataRate(drs), vSrc.Get());
	BOOST_CHECK_EQUAL(ns3::DataRate(drd), vDst.Get());
	BOOST_CHECK_NE(ns3::DataRate(drs), vDst.Get());
	BOOST_CHECK_NE(ns3::DataRate(drd), vSrc.Get());

	// mtu
	BOOST_CHECK_EQUAL(deviceSrc.Get(0)->GetMtu(), mtus);
	BOOST_CHECK_EQUAL(deviceDst.Get(0)->GetMtu(), mtud);
	BOOST_CHECK_NE(deviceSrc.Get(0)->GetMtu(), mtud);
	BOOST_CHECK_NE(deviceDst.Get(0)->GetMtu(), mtus);

	// interframe gap
	ns3::TimeValue timeSrc;
	ns3::TimeValue timeDst;
	deviceSrc.Get(0)->GetAttribute("InterframeGap", timeSrc);
	deviceDst.Get(0)->GetAttribute("InterframeGap", timeDst);
	BOOST_CHECK_EQUAL(timeSrc.Get(), ns3::Time(times));
	BOOST_CHECK_EQUAL(timeDst.Get(), ns3::Time(timed));
	BOOST_CHECK_NE(timeSrc.Get(), ns3::Time(timed));
	BOOST_CHECK_NE(timeDst.Get(), ns3::Time(times));

	// delay
	ns3::TimeValue delayValue;
	deviceSrc.Get(0)->GetChannel()->GetAttribute("Delay", delayValue);
	BOOST_CHECK_EQUAL(delayValue.Get(), ns3::Time(delay));
}

BOOST_AUTO_TEST_SUITE_END()
/**
 *  end heavy test suite
 */

//
BOOST_AUTO_TEST_CASE(SimulationExceptionTest) {
	Simulation si;
	EdgePtr edge;
	SetPtr<EdgePtr> edgeList;

	// null-pointer-exception
	BOOST_CHECK_THROW(si.AddEdge(edge), NullPointerException);
	BOOST_CHECK_THROW(si.AddEdges(edgeList), NullPointerException);

	// no-edge set-up "no" exception
	BOOST_CHECK_NO_THROW(si.SetUpNetworks());

	// re-set-up "no" exception
	edge = make_shared_base_in_simul(Edge, P2PEdge, make_shared<simul::Node>(),
			make_shared<simul::Node>());
	si.AddEdge(edge);
	si.SetUpNetworks();
	BOOST_CHECK_NO_THROW(si.SetUpNetworks());
}

BOOST_AUTO_TEST_CASE(SimulationRunTest) {
	EdgePtr edge = make_shared<P2PEdge>(make_shared<simul::Node>(), make_shared<simul::Node>());

	// same mem address (test destructor & run)
	Simulation *address1, *address2;
	{
		Simulation si;
		si.AddEdge(edge);
		si.SetUpNetworks();

		address1 = &si;
	}
	{
		Simulation si;
		si.AddEdge(edge);
		BOOST_REQUIRE_THROW(si.Run(), UsageException);

		address2 = &si;
	}
	BOOST_REQUIRE_EQUAL(address1, address2);

	// run without set up
	Simulation si1;
	si1.SetUpNetworks();
	{
		Simulation si2;
		BOOST_REQUIRE_THROW(si2.Run(), UsageException);
	}
	BOOST_REQUIRE_NO_THROW(si1.Run());	// si2's destructor should not affect si1;
}

BOOST_AUTO_TEST_CASE(SimulationLogComponentTest) {
	Simulation si;

	BOOST_CHECK_NO_THROW(Simulation::DisableAllLogComponents());
	BOOST_CHECK_NO_THROW(Simulation::DisableAllLogComponents(ns3::LOG_LEVEL_FUNCTION));

	BOOST_CHECK_NO_THROW(Simulation::EnableAllLogComponents(ns3::LOG_PREFIX_NODE));
	BOOST_CHECK_NO_THROW(Simulation::DisableAllLogComponents(ns3::LOG_LOGIC));

	BOOST_CHECK_NO_THROW(Simulation::EnableAllLogComponents());
	BOOST_CHECK_NO_THROW(Simulation::DisableAllLogComponents());
}

