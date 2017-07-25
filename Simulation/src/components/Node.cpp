/*
 * Node.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#include "Node.h"

#include "ns3/vector.h"

namespace simul {

//NS_LOG_COMPONENT_DEFINE("simul::Node");

Node::Node() {
}

Node::Node(string p_ID) :
		ID(p_ID) {
}

Node::Node(double x, double y, double z) {
	SetPosition(x, y, z);
}

Node::~Node() {
}

void Node::SetPosition(double x, double y, double z) {
	position = ns3::Vector3D(x, y, z);
}

void Node::SetID(string ID) {
	this->ID = ID;
}

string Node::GetID() {
	return ID;
}

ns3::Vector3D Node::GetPosition() {
	return position;
}

ns3::Ptr<ns3::Node> static CreateNs3Node();

ns3::Ptr<ns3::Node> Node::GetNs3Node() {
	if (ns3Node == nullptr)
		ns3Node = CreateNs3Node();
	return ns3Node;
}

//
// private
//
void Node::NotifySimulatorDestroyed() {
	ns3Node = nullptr;
}

using namespace ns3;

ns3::Ptr<ns3::Node> static CreateNs3Node() {
	Ptr<ns3::Node> ns3Node = CreateObject<ns3::Node>();

	// install internet stack & routing protocol
	Ipv4NixVectorHelper nixRouting;
	Ipv4StaticRoutingHelper staticRouting;

	Ipv4ListRoutingHelper list;
	list.Add(staticRouting, 0);
	list.Add(nixRouting, 10);

	InternetStackHelper stack;
	stack.SetRoutingHelper(list);
	stack.Install(ns3Node);

	return ns3Node;
}

} /* namespace simul */
