/*
 * AnyNodeTest.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../../AnyGraph.h"

using namespace anygraph;

BOOST_AUTO_TEST_CASE(AnyNodeTest) {
	AnyNode node;
	//constructor test
	BOOST_CHECK(node.get_ID() == std::string(""));

	std::string id = "id0";
	node.set_ID(id);

	BOOST_CHECK(node.get_ID() == id);

	Attributes attr1, attr2;

	attr1.set_id("id1");
	attr2.set_id("id2");

	std::vector<Attributes> attrList;

	attrList.push_back(attr1);
	attrList.push_back(attr2);

	node.add_attributes(attr1);
	node.add_attributes(attr2);

	BOOST_CHECK(node.get_attribute_list().size() == attrList.size());
	BOOST_CHECK(
			node.get_attribute_at(0).get_attribute_id()
					== attr1.get_attribute_id());
	BOOST_CHECK(
			node.get_attribute_at(1).get_attribute_id()
					== attr2.get_attribute_id());

}

