/*
 * Node.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */

#include "AnyNode.h"

namespace anygraph {

AnyNode::AnyNode() {
	id = "";
}

AnyNode::~AnyNode() {
}

AnyNode::AnyNode(string id_key) {
	id = id_key;
}

void AnyNode::set_ID(string id_data) {
	id = id_data;
}

vector<Attributes> AnyNode::get_attribute_list() {
	return attributes;
}

void AnyNode::add_attributes(Attributes attr) {
	attributes.push_back(attr);
}

string AnyNode::get_ID() {
	return id;
}

Attributes AnyNode::get_attribute_at(int index) {
	return attributes[index];
}

} /* namespace std */
