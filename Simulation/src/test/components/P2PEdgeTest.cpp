/*
 * P2PEdgeTest.cpp
 *
 *  Created on: May 11, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"

using namespace simul;

BOOST_AUTO_TEST_CASE(P2PEdgeSimpleTest) {
	shared_ptr<P2PEdge> edge = make_shared<P2PEdge>(make_shared<simul::Node>(),
			make_shared<simul::Node>());
	Simulation si;
	si.AddEdge(edge);

	BOOST_REQUIRE_EQUAL(edge->UpdateConfiguration(), false);

	si.SetUpNetworks();
	BOOST_REQUIRE_EQUAL(edge->UpdateConfiguration(), true);

	ns3::Simulator::Destroy();
	BOOST_REQUIRE_EQUAL(edge->UpdateConfiguration(), false);
}

BOOST_AUTO_TEST_CASE(P2PEdgeAttributeTest) {
	shared_ptr<P2PEdge> edge = make_shared<P2PEdge>(make_shared<simul::Node>(),
			make_shared<simul::Node>());

	// dataRate (start with default values)
	ns3::DataRate dr;
	BOOST_CHECK_EQUAL(ns3::DataRate(edge->GetDataRateSrc()), dr);
	BOOST_CHECK_EQUAL(ns3::DataRate(edge->GetDataRateDst()), dr);

	string drs = "12kbps", drd = "77Mbps";
	edge->SetDataRateSrc(drs);
	edge->SetDataRateDst(drd);
	BOOST_CHECK_EQUAL(edge->GetDataRateSrc(), drs);
	BOOST_CHECK_EQUAL(edge->GetDataRateDst(), drd);
	BOOST_CHECK_NE(edge->GetDataRateSrc(), drd);
	BOOST_CHECK_NE(edge->GetDataRateDst(), drs);

	// mtu
	uint16_t mtus = 1500, mtud = 1500;	// start with default values
	BOOST_CHECK_EQUAL(edge->GetMTUSrc(), mtus);
	BOOST_CHECK_EQUAL(edge->GetMTUDst(), mtud);

	mtus = 12, mtud = 2000;
	edge->SetMTUSrc(mtus);
	edge->SetMTUDst(mtud);
	BOOST_CHECK_EQUAL(edge->GetMTUSrc(), mtus);
	BOOST_CHECK_EQUAL(edge->GetMTUDst(), mtud);

	// interframe gap
	string times = "0ns", timed = "0ns";	// start with default values
	BOOST_CHECK_EQUAL(ns3::Time(edge->GetInterframeGapSrc()), ns3::Time(times));
	BOOST_CHECK_EQUAL(ns3::Time(edge->GetInterframeGapDst()), ns3::Time(timed));

	times = "96ns", timed = "11s";
	edge->SetInterframeGapSrc(times);
	edge->SetInterframeGapDst(timed);
	BOOST_CHECK_EQUAL(ns3::Time(edge->GetInterframeGapSrc()), ns3::Time(times));
	BOOST_CHECK_EQUAL(ns3::Time(edge->GetInterframeGapDst()), ns3::Time(timed));
	BOOST_CHECK_NE(ns3::Time(edge->GetInterframeGapSrc()), ns3::Time(timed));
	BOOST_CHECK_NE(ns3::Time(edge->GetInterframeGapDst()), ns3::Time(times));

	// delay
	string delay = "0ms";	// start with default values
	BOOST_CHECK_EQUAL(edge->GetDelay(), delay);

	delay = "2s";
	edge->SetDelay(delay);
	BOOST_CHECK_EQUAL(edge->GetDelay(), delay);
}

BOOST_AUTO_TEST_CASE(P2PEdgewRunTest) {
	NodePtr node1 = make_shared<simul::Node>();
	NodePtr node2 = make_shared<simul::Node>();
	shared_ptr<P2PEdge> edge = make_shared<P2PEdge>(node1, node2);
	Simulation si;
	si.AddEdge(edge);
	si.SetUpNetworks();

	Ipv4PacketInfoOutput out;
	out.EnableCbLocalDeliver();

	weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(node1, node2);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
		event.lock()->SetStartTime(0);

	// scenario 1
	string delay = "17ns";
	edge->SetDelay(delay);
	edge->UpdateConfiguration();
	si.Run();

	VectorPtr<PacketInfoPtr> list = out.GetPacketInfoList();

	for (auto pack : *list) {
		BOOST_REQUIRE_EQUAL(pack->GetDeliveredTime() - pack->GetSentTime(), ns3::Time(delay));
	}

	// scenario 2
	delay = "0s";
	string dataRate = "1kbps";

	edge->SetDelay(delay);
	edge->SetDataRateSrc(dataRate);
	edge->SetDataRateDst(dataRate);
	edge->UpdateConfiguration();

	out.ClearPacketInfoList();

	//	event.lock()->SetUp();
	//	event.lock()->Schedule(0);
		event.lock()->SetStartTime(0);
	si.Run();

	list = out.GetPacketInfoList();
	BOOST_REQUIRE_EQUAL(list->size(), 1);

	for (auto pack : *list) {
		uint32_t bytes = pack->GetSize();
		BOOST_REQUIRE_GT(bytes, 1024);

		double error = (pack->GetDeliveredTime() - pack->GetSentTime()
				- ns3::DataRate(dataRate).CalculateBytesTxTime(bytes)).GetSeconds();
		BOOST_REQUIRE_LT(error, 1);
	}
}

