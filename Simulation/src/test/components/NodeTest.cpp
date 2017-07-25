/*
 * NodeTest.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"

using namespace simul;
using namespace ns3;

BOOST_AUTO_TEST_CASE(NodeMembersTest) {

	// no-argument constructor
	NodePtr node = std::make_shared<simul::Node>();
	Vector3D null, position = node->GetPosition();
	BOOST_CHECK(null.x == position.x && null.y == position.y && null.z == position.z);
	BOOST_CHECK_EQUAL(node->GetID(), "");
	BOOST_CHECK(node->GetNs3Node() != nullptr);

	// constructor
	double x = 1.1, y = 2.2, z = 3.3;
	node = std::make_shared<simul::Node>(x, y, z);
	position = node->GetPosition();
	BOOST_CHECK_EQUAL(position.x, x);
	BOOST_CHECK_EQUAL(position.y, y);
	BOOST_CHECK_EQUAL(position.z, z);

	// setID()
	std::string id = "my ID";
	node->SetID(id);

	// setPosition() & check position
	x = -x, y = -y, z = -z;
	node->SetPosition(x, y, z);
	position = node->GetPosition();
	BOOST_CHECK_EQUAL(position.x, x);
	BOOST_CHECK_EQUAL(position.y, y);
	BOOST_CHECK_EQUAL(position.z, z);

	// check ID
	BOOST_CHECK_EQUAL(node->GetID(), id);
	node = std::make_shared<simul::Node>(id + " new");
	BOOST_CHECK_EQUAL(node->GetID(), id + " new");
}

