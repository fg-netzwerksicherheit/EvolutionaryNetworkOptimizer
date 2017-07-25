/*
 * Topology.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */

#include <tinyxml2.h>
#include <iostream>

#include "Topology.h"

namespace anygraph {

Topology::Topology() {
}

Topology::~Topology() {
}

void Topology::add_node(AnyNode node) {
	node_list.push_back(node);
}

void Topology::add_edge(AnyEdge edge) {
	edge_list.push_back(edge);
}

void Topology::add_attribute(Attributes t) {
	topology_attributes.push_back(t);
}

void Topology::set_ID(string s) {
	id = s;
}

vector<AnyNode> Topology::get_node() {
	return node_list;
}

vector<AnyEdge> Topology::get_edge() {
	return edge_list;
}

vector<Attributes> Topology::get_attributes() {
	return topology_attributes;
}

int Topology::get_node_size() {
	return node_list.size();
}

int Topology::get_edge_size() {
	return edge_list.size();
}

} /* namespace std */

