/*
 * P2PNetTest.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"
#include "../SimpleFixtures.h"

using namespace simul;

BOOST_FIXTURE_TEST_CASE(NetP2PMembersTest, NetP2PFixture) {

	EdgePtr edge = std::make_shared<P2PEdge>(std::make_shared<simul::Node>(),
			std::make_shared<simul::Node>());

	this->AddEdge(edge);
	this->Configure();

	//
	BOOST_CHECK(this->GetAllNetDevices().GetN() == 2);
	for (uint i = 0; i < this->GetAllNetDevices().GetN(); i++)
		BOOST_CHECK_EQUAL(this->GetAllNetDevices().Get(i)->IsPointToPoint(), true);

	BOOST_CHECK(this->nodeList->size() == 2);
	BOOST_CHECK(this->edgeList->size() == 1);

	BOOST_CHECK(this->p2pNodes.GetN() == 2);
	BOOST_CHECK(this->p2pNodes.Get(0) != this->p2pNodes.Get(1));
}

BOOST_FIXTURE_TEST_CASE(NetP2PExceptionTest, NetP2PFixture) {
	EdgePtr edge;
	BOOST_CHECK_THROW(this->AddEdge(edge), NullPointerException);
}

BOOST_FIXTURE_TEST_CASE(NetP2PAttributesTest, NetP2PFixture) {
	shared_ptr<P2PEdge> edge = std::make_shared<P2PEdge>(std::make_shared<simul::Node>(),
			std::make_shared<simul::Node>());

	string dataRateSrc("1kbps");
	string dataRateDst("9Mbps");
	edge->SetDataRateSrc(dataRateSrc);
	edge->SetDataRateDst(dataRateDst);

	uint16_t MTUSrc = 2121;
	uint16_t MTUDst = 512;
	edge->SetMTUSrc(MTUSrc);
	edge->SetMTUDst(MTUDst);

	string interframeGapSrc("1ns");
	string interframeGapDst("3s");
	edge->SetInterframeGapSrc(interframeGapSrc);
	edge->SetInterframeGapDst(interframeGapDst);

	string delay("7ms");
	edge->SetDelay(delay);

	this->AddEdge(edge);
	this->Configure();

	ns3::Ptr<ns3::PointToPointNetDevice> dsrc = ns3::DynamicCast<ns3::PointToPointNetDevice>(
			this->GetAllNetDevices().Get(0));
	ns3::Ptr<ns3::PointToPointNetDevice> ddst = ns3::DynamicCast<ns3::PointToPointNetDevice>(
			this->GetAllNetDevices().Get(1));

	ns3::DataRateValue dv;
	dsrc->GetAttribute("DataRate", dv);
	BOOST_REQUIRE_EQUAL(dv.Get(), ns3::DataRate(dataRateSrc));
	ddst->GetAttribute("DataRate", dv);
	BOOST_REQUIRE_EQUAL(dv.Get(), ns3::DataRate(dataRateDst));

	BOOST_REQUIRE_EQUAL(dsrc->GetMtu(), MTUSrc);
	BOOST_REQUIRE_EQUAL(ddst->GetMtu(), MTUDst);

	ns3::TimeValue tv;
	dsrc->GetAttribute("InterframeGap", tv);
	BOOST_REQUIRE_EQUAL(tv.Get(), ns3::Time(interframeGapSrc));
	ddst->GetAttribute("InterframeGap", tv);
	BOOST_REQUIRE_EQUAL(tv.Get(), ns3::Time(interframeGapDst));

	dsrc->GetChannel()->GetAttribute("Delay", tv);
	BOOST_REQUIRE_EQUAL(tv.Get(), ns3::Time(delay));
}
