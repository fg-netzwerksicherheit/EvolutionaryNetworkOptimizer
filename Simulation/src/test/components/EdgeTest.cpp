/*
 * EdgeTest.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"

using namespace simul;

BOOST_AUTO_TEST_CASE(EdgeExceptionTest) {
	NodePtr node;
	BOOST_CHECK_THROW(P2PEdge(std::make_shared<simul::Node>(), node), NullPointerException);
	BOOST_CHECK_THROW(P2PEdge(node, std::make_shared<simul::Node>()), NullPointerException);
	BOOST_CHECK_THROW(P2PEdge(node, node), NullPointerException);
	BOOST_CHECK_THROW(WifiEdge(std::make_shared<simul::Node>(), node), NullPointerException);
	BOOST_CHECK_THROW(WifiEdge(node, std::make_shared<simul::Node>()), NullPointerException);
	BOOST_CHECK_THROW(WifiEdge(node, node), NullPointerException);

	node = std::make_shared<simul::Node>();
	BOOST_CHECK_THROW(P2PEdge(node, node), invalid_argument);
	BOOST_CHECK_THROW(WifiEdge(node, node), invalid_argument);
}

BOOST_AUTO_TEST_CASE(EdgeMembersTest) {
	NodePtr source = std::make_shared<simul::Node>();
	NodePtr dest = std::make_shared<simul::Node>();
	EdgePtr edge;

	edge = std::make_shared<P2PEdge>(source, dest);
	BOOST_CHECK(edge->GetSourceNode() == source);
	BOOST_CHECK(edge->GetDestNode() == dest);

	edge = std::make_shared<WifiEdge>(source, dest);
	BOOST_CHECK(edge->GetSourceNode() == source);
	BOOST_CHECK(edge->GetDestNode() == dest);
}



