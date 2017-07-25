/*
 * TopologyTest.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../../AnyGraph.h"

using namespace anygraph;

BOOST_AUTO_TEST_CASE(TopologyTest) {
	Topology topology;

	AnyNode node1, node2, node3;
	node1.set_ID("n1");
	node2.set_ID("n2");
	node3.set_ID("n3");

	AnyEdge edge1, edge2;
	edge1.set_from("n1");
	edge1.set_to("n2");
	edge2.set_from("n1");
	edge2.set_to("n3");


	Attributes attr1, attr2;
	attr1.set_id("a1");
	attr2.set_id("a2");

	topology.add_node(node1);
	topology.add_node(node2);
	topology.add_node(node3);
	topology.add_edge(edge1);
	topology.add_edge(edge2);
	topology.add_attribute(attr1);
	topology.add_attribute(attr2);

	BOOST_CHECK(topology.get_attributes().size() == 2);
	BOOST_CHECK(topology.get_edge_size() == 2);
	BOOST_CHECK(topology.get_node_size() == 3);
	BOOST_CHECK(topology.get_node()[0].get_ID() == std::string("n1"));
	BOOST_CHECK(topology.get_node()[1].get_ID() == std::string("n2"));
	BOOST_CHECK(topology.get_node()[2].get_ID() == std::string("n3"));
	BOOST_CHECK(topology.get_edge()[0].get_From() == std::string("n1"));
	BOOST_CHECK(topology.get_edge()[0].get_To() == std::string("n2"));
	BOOST_CHECK(topology.get_edge()[1].get_From() == std::string("n1"));
	BOOST_CHECK(topology.get_edge()[1].get_To() == std::string("n3"));
	BOOST_CHECK(topology.get_attributes()[0].get_attribute_id() == std::string("a1"));
	BOOST_CHECK(topology.get_attributes()[1].get_attribute_id() == std::string("a2"));
}

