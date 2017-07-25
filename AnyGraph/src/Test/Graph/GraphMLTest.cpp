/*
 * GraphMLTest.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../AnyGraph.h"

std::string GetProjectAnyGraphDirectory();

using namespace anygraph;

void check_graphml_correct(std::string input, std::string output) {
	GraphMLHandler GraphMLIO;
	std::shared_ptr<Topology> t1 = make_shared<Topology>();
	std::shared_ptr<Topology> t2 = make_shared<Topology>();
	t1 = GraphMLIO.ReadGraph(input);
	GraphMLIO.WriteGraph(t1, output);
	t2 = GraphMLIO.ReadGraph(output);
	BOOST_CHECK(t1->get_attributes().size() == t2->get_attributes().size());
	BOOST_REQUIRE_EQUAL(t1->get_edge_size(), t2->get_edge_size());
	for (int i = 0; i < t1->get_edge_size(); ++i) {
		BOOST_CHECK(t1->get_edge()[i].get_From() == t2->get_edge()[i].get_From());
		BOOST_CHECK(t1->get_edge()[i].get_To() == t2->get_edge()[i].get_To());
		BOOST_REQUIRE_EQUAL(t1->get_edge()[i].get_Attribute().size(),
				t2->get_edge()[i].get_Attribute().size());
		bool edgeCorrect = true;
		for (unsigned int i1 = 0; i1 < t1->get_edge()[i].get_Attribute().size(); ++i1) {
			edgeCorrect = edgeCorrect
					&& (t1->get_edge()[i].get_attribute_at(i1).get_attribute_id()
							== t2->get_edge()[i].get_attribute_at(i1).get_attribute_id());
			edgeCorrect = edgeCorrect
					&& (t1->get_edge()[i].get_attribute_at(i1).get_attribute_name()
							== t2->get_edge()[i].get_attribute_at(i1).get_attribute_name());
			edgeCorrect = edgeCorrect
					&& (t1->get_edge()[i].get_attribute_at(i1).get_attribute_data()
							== t2->get_edge()[i].get_attribute_at(i1).get_attribute_data());
			edgeCorrect = edgeCorrect
					&& (t1->get_edge()[i].get_attribute_at(i1).get_attribute_type()
							== t2->get_edge()[i].get_attribute_at(i1).get_attribute_type());
		}
		BOOST_CHECK(edgeCorrect == true);
	}
	BOOST_REQUIRE_EQUAL(t1->get_node_size(), t2->get_node_size());
	for (int i = 0; i < t1->get_node_size(); ++i) {
		BOOST_CHECK(t1->get_node()[i].get_ID() == t2->get_node()[i].get_ID());
		BOOST_REQUIRE_EQUAL(t1->get_node()[i].get_attribute_list().size(),
				t2->get_node()[i].get_attribute_list().size());
		bool nodeCorrect = true;
		for (unsigned int i1 = 0; i1 < t1->get_node()[i].get_attribute_list().size(); ++i1) {
			nodeCorrect = nodeCorrect
					&& (t1->get_node()[i].get_attribute_at(i1).get_attribute_id()
							== t2->get_node()[i].get_attribute_at(i1).get_attribute_id());
			nodeCorrect = nodeCorrect
					&& (t1->get_node()[i].get_attribute_at(i1).get_attribute_name()
							== t2->get_node()[i].get_attribute_at(i1).get_attribute_name());
			nodeCorrect = nodeCorrect
					&& (t1->get_node()[i].get_attribute_at(i1).get_attribute_data()
							== t2->get_node()[i].get_attribute_at(i1).get_attribute_data());
			nodeCorrect = nodeCorrect
					&& (t1->get_node()[i].get_attribute_at(i1).get_attribute_type()
							== t2->get_node()[i].get_attribute_at(i1).get_attribute_type());

		}
		BOOST_CHECK(nodeCorrect == true);
	}

}

BOOST_AUTO_TEST_CASE(GraphMLTest) {
	//another test case................................................................
	std::string in_path = GetProjectAnyGraphDirectory() + "/Resources/UnitTestInput1.graphml";
	std::string out_path = GetProjectAnyGraphDirectory() + "/Resources/UnitTestOutput1.graphml";
	//another test case................................................................
	std::string in_path1 = GetProjectAnyGraphDirectory() + "/Resources/UnitTestInput2.graphml";
	std::string out_path1 = GetProjectAnyGraphDirectory() + "/Resources/UnitTestOutput2.graphml";
	//another test case................................................................
	std::string in_path2 = GetProjectAnyGraphDirectory() + "/Resources/UnitTestInput3.graphml";
	std::string out_path2 = GetProjectAnyGraphDirectory() + "/Resources/UnitTestOutput3.graphml";
	//another test case................................................................
	check_graphml_correct(in_path, out_path);
	check_graphml_correct(in_path1, out_path1);
	check_graphml_correct(in_path2, out_path2);
}

