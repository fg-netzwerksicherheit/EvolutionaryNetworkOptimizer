/*
 * AnyEdgeTest.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../../AnyGraph.h"

using namespace anygraph;

BOOST_AUTO_TEST_CASE(AnyEdgeTest) {
	AnyEdge edge;
	//constructor
	BOOST_CHECK(edge.get_From() == std::string("0"));
	BOOST_CHECK(edge.get_To() == std::string("0"));

	std::string from = "1";
	std::string to = "2";

	edge = AnyEdge(from, to);

	BOOST_CHECK(edge.get_From() == std::string("1"));
	BOOST_CHECK(edge.get_To() == std::string("2"));

	//else

	from = "3";
	to = "4";

	edge.set_from(from);
	edge.set_to(to);

	BOOST_CHECK(edge.get_From() == std::string("3"));
	BOOST_CHECK(edge.get_To() == std::string("4"));

	Attributes attr1, attr2;

	attr1.set_id("id1");
	attr2.set_id("id2");

	std::vector<Attributes>attrList;

	attrList.push_back(attr1);
	attrList.push_back(attr2);

	edge.add_attribute(attr1);
	edge.add_attribute(attr2);

	BOOST_CHECK(edge.get_Attribute().size() == attrList.size());
	BOOST_CHECK(edge.get_attribute_at(0).get_attribute_id() == attr1.get_attribute_id());
	BOOST_CHECK(edge.get_attribute_at(1).get_attribute_id() == attr2.get_attribute_id());

}


