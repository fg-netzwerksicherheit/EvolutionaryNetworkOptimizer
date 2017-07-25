/*
 * EventTest.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: quang
 */
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../AnyGraph.h"

using namespace anygraph;

BOOST_AUTO_TEST_CASE(EventTest) {
	AnyEvent event;
	Attributes attr, attr1;
	attr.set_id("aid1");
	attr.set_data("adata1");
	attr.set_name("aname1");
	attr.set_type("string");
	attr1.set_id("aid2");
	attr1.set_data("adata2");
	attr1.set_name("aname2");
	attr1.set_type("string");
	event.add_attribute(attr);
	event.add_attribute(attr1);
	std::vector<Attributes>attr_list;
	attr_list.push_back(attr);
	attr_list.push_back(attr1);
	BOOST_REQUIRE_EQUAL(event.get_attribute_size(), attr_list.size());
	bool correct = true;
	for(uint i = 0; i < attr_list.size(); ++i)
	{
		correct = correct && (event.get_attributes_list()[i].get_attribute_data() == attr_list[i].get_attribute_data());
		correct = correct && (event.get_attributes_list()[i].get_attribute_name() == attr_list[i].get_attribute_name());
		correct = correct && (event.get_attributes_list()[i].get_attribute_id() == attr_list[i].get_attribute_id());
		correct = correct && (event.get_attributes_list()[i].get_attribute_type() == attr_list[i].get_attribute_type());
		correct = correct && (event.get_attribute_data_at(i) == attr_list[i].get_attribute_data());
		correct = correct && (event.get_attribute_name_at(i) == attr_list[i].get_attribute_name());
	}
	BOOST_CHECK (correct == true);

}



