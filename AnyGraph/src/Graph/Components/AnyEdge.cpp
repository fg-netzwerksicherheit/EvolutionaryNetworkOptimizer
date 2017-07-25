/*
 * Edge.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */

#include "AnyEdge.h"

namespace anygraph {

AnyEdge::AnyEdge() {
	from = ("0");
	to = ("0");

}

AnyEdge::~AnyEdge() {
	// TODO Auto-generated destructor stub
}
AnyEdge::AnyEdge(string fromNode, string toNode) {
	from = (fromNode);
	to = (toNode);
}
void AnyEdge::set_from(string fromNode) {
	from = (fromNode);
}
void AnyEdge::set_to(string toNode) {
	to = (toNode);
}
void AnyEdge::set_ID(string i) {
	id = i;
}
void AnyEdge::add_attribute(Attributes a) {
	attributes.push_back(a);
}
string AnyEdge::get_From() {
	return from;
}
string AnyEdge::get_To() {
	return to;
}
vector<Attributes> AnyEdge::get_Attribute() {
	return attributes;
}
Attributes AnyEdge::get_attribute_at(int index) {
	return attributes[index];
}

} /* namespace std */
