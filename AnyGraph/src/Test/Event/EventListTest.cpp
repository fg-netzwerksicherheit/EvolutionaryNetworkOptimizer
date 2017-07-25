/*
 * EventListTest.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: quang
 */
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../AnyGraph.h"

std::string GetProjectAnyGraphDirectory();

using namespace anygraph;

bool check_attributes(AnyEventList event_list, int event_index, int attribute_index,
		std::string name, std::string data) {
	if (event_list.get_event_at(event_index).get_attribute_name_at(attribute_index) == name
			&& event_list.get_event_at(event_index).get_attribute_data_at(attribute_index)
					== data) {
		return true;
	} else
		return false;
}

BOOST_AUTO_TEST_CASE(EventListTest) {
	AnyEventList json_event_list;
	AnyEventList xml_event_list;
	std::string xml_path = GetProjectAnyGraphDirectory() + "/Resources/use_case1_events.xml";
	std::string json_path = GetProjectAnyGraphDirectory() + "/Resources/use_case1_events.json";

	json_event_list.read_json_event(json_path);
	BOOST_REQUIRE_EQUAL(json_event_list.get_number_of_event(), 3);
	bool json_correct = true;

	json_correct = json_correct && (check_attributes(json_event_list, 0, 0, "protocol", "tcp"));
	json_correct = json_correct && (check_attributes(json_event_list, 0, 1, "start", "6343"));
	json_correct = json_correct && (check_attributes(json_event_list, 0, 2, "src", "n1"));
	json_correct = json_correct && (check_attributes(json_event_list, 0, 3, "dst", "n2"));
	json_correct = json_correct
			&& (check_attributes(json_event_list, 0, 4, "datatransfer", "796286676"));

	json_correct = json_correct && (check_attributes(json_event_list, 1, 0, "protocol", "udp"));
	json_correct = json_correct && (check_attributes(json_event_list, 1, 1, "start", "2727"));
	json_correct = json_correct && (check_attributes(json_event_list, 1, 2, "src", "n0"));
	json_correct = json_correct && (check_attributes(json_event_list, 1, 3, "dst", "n4"));
	json_correct = json_correct
			&& (check_attributes(json_event_list, 1, 4, "datatransfer", "885678137"));

	json_correct = json_correct && (check_attributes(json_event_list, 2, 0, "protocol", "tcp"));
	json_correct = json_correct && (check_attributes(json_event_list, 2, 1, "start", "3671"));
	json_correct = json_correct && (check_attributes(json_event_list, 2, 2, "src", "n1"));
	json_correct = json_correct && (check_attributes(json_event_list, 2, 3, "dst", "n3"));
	json_correct = json_correct
			&& (check_attributes(json_event_list, 2, 4, "datatransfer", "397113450"));

	BOOST_CHECK(json_correct == true);

	xml_event_list.read_xml_event(xml_path);
	BOOST_REQUIRE_EQUAL(xml_event_list.get_number_of_event(), 4);

	bool xml_correct = true;

	xml_correct = xml_correct && (check_attributes(xml_event_list, 0, 0, "protocol", "tcp"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 0, 1, "start", "6343"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 0, 2, "src", "1"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 0, 3, "dst", "2"));
	xml_correct = xml_correct
			&& (check_attributes(xml_event_list, 0, 4, "datatransfer", "796286676"));

	xml_correct = xml_correct && (check_attributes(xml_event_list, 1, 0, "protocol", "tcp"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 1, 1, "start", "2727"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 1, 2, "src", "0"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 1, 3, "dst", "4"));
	xml_correct = xml_correct
			&& (check_attributes(xml_event_list, 1, 4, "datatransfer", "885678137"));

	xml_correct = xml_correct && (check_attributes(xml_event_list, 2, 0, "protocol", "tcp"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 2, 1, "start", "3671"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 2, 2, "src", "1"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 2, 3, "dst", "3"));
	xml_correct = xml_correct
			&& (check_attributes(xml_event_list, 2, 4, "datatransfer", "397113450"));

	xml_correct = xml_correct && (check_attributes(xml_event_list, 3, 0, "protocol", "udp"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 3, 1, "start", "2702"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 3, 2, "src", "2"));
	xml_correct = xml_correct && (check_attributes(xml_event_list, 3, 3, "dst", "0"));
	xml_correct = xml_correct
			&& (check_attributes(xml_event_list, 3, 4, "datatransfer", "667440230"));

	BOOST_CHECK(xml_correct == true);

}

