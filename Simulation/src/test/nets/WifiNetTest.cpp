/*
 * WifiNetTest.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"
#include "../SimpleFixtures.h"

using namespace simul;

BOOST_FIXTURE_TEST_CASE(NetWifiMembersTest, NetWifiFixture) {

	NodePtr node = std::make_shared<simul::Node>();
	EdgePtr edge;

	edge = std::make_shared<WifiEdge>(node, std::make_shared<simul::Node>());
	this->AddEdge(edge);

	edge = std::make_shared<WifiEdge>(node, std::make_shared<simul::Node>());
	this->AddEdge(edge);

	this->Configure();

	//
	BOOST_CHECK(this->GetAllNetDevices().GetN() == 3);
	for (uint i = 0; i < this->GetAllNetDevices().GetN(); i++) {
		ns3::Ptr<ns3::WifiNetDevice> wifi = ns3::DynamicCast<ns3::WifiNetDevice>(this->GetAllNetDevices().Get(i));
		BOOST_CHECK(wifi->GetPhy() != nullptr);
	}

	BOOST_CHECK(this->nodeList->size() == 3);
	BOOST_CHECK(this->edgeList->size() == 2);

	BOOST_CHECK(this->staNodes.GetN() == 2);
	BOOST_CHECK(this->apNode.GetN() == 1);
}

BOOST_FIXTURE_TEST_CASE(NetWifiExceptionTest, NetWifiFixture) {
	EdgePtr edge;
	BOOST_CHECK_THROW(this->AddEdge(edge), NullPointerException);
}

