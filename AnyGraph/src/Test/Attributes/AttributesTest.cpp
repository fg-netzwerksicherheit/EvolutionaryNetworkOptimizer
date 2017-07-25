/*
 * AttributesTest.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../AnyGraph.h"

using namespace anygraph;

BOOST_AUTO_TEST_CASE(AttributesTest) {
	Attributes attribute;

	BOOST_CHECK(attribute.get_attribute_id() == std::string(""));
	BOOST_CHECK(attribute.get_attribute_name() == std::string(""));
	BOOST_CHECK(attribute.get_attribute_type() == std::string(""));
	BOOST_CHECK(attribute.get_attribute_data() == std::string(""));

	std::string id = "ID1";
	attribute.set_id(id);
	std::string name = "name1";
	attribute.set_name(name);
	std::string data = "data1";
	attribute.set_data(data);
	std::string type = "type1";
	attribute.set_type(type);

	BOOST_REQUIRE_EQUAL(attribute.get_attribute_id().c_str(), id.c_str());
	BOOST_REQUIRE_EQUAL(attribute.get_attribute_name().c_str(), name.c_str());
	BOOST_REQUIRE_EQUAL(attribute.get_attribute_data().c_str(), data.c_str());
	BOOST_REQUIRE_EQUAL(attribute.get_attribute_type().c_str(), type.c_str());
}

