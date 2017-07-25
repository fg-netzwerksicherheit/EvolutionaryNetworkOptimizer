/*
 * GraphMLInput.cpp
 *
 *  Created on: Jun 17, 2017
 *      Author: Kiet Vo
 */

#include "GraphMLInput.h"
#include "../components/Node.h"
#include "../components/P2PEdge.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::GraphMLInput");

GraphMLInput::GraphMLInput() {
}

GraphMLInput::~GraphMLInput() {
}

static void SetAttributes(P2PEdgePtr edge, anygraph::AnyEdge ae, anygraph::AnyNode srcDevice,
		anygraph::AnyNode dstDevice);

SetPtr<EdgePtr> GraphMLInput::ReadGraph(string path) {
	return ParseGraph(anygraph::GraphMLHandler().ReadGraph(path));
}

SetPtr<EdgePtr> GraphMLInput::ParseGraph(TopologyPtr topology) {

	//
	// create a node map (device -> computer)
	// and a node map (deviceID -> device)
	//
	unordered_map<string, NodePtr> nodeMap;
	unordered_map<string, anygraph::AnyNode> deviceMap;

	for (auto device : topology->get_node())
		for (auto attribute : device.get_attribute_list()) {
			if (attribute.get_attribute_name() != "Computer")
				continue;

			string computerID = attribute.get_attribute_data();
			NodePtr computerOfThisDevice = nullptr;

			// existing computer?
			for (auto it = nodeMap.cbegin(); it != nodeMap.cend(); it++) {
				NodePtr comp = (*it).second;
				if (comp->GetID() == computerID) {
					computerOfThisDevice = comp;
					break;
				}
			}

			// new computer?
			if (computerOfThisDevice == nullptr)
				computerOfThisDevice = make_shared<Node>(computerID);

			nodeMap[device.get_ID()] = computerOfThisDevice;
			deviceMap[device.get_ID()] = device;
			break;		// go to next device
		}

	//
	// create edges for Simulation
	//
	SetPtr<EdgePtr> edges = make_shared<set<EdgePtr>>();
	for (anygraph::AnyEdge ae : topology->get_edge()) {
		NodePtr n1 = nodeMap[ae.get_From()];
		NodePtr n2 = nodeMap[ae.get_To()];

		if (n1 == n2)	// cannot connect a node to itself
			continue;

		P2PEdgePtr edge = make_shared<P2PEdge>(n1, n2);
		SetAttributes(edge, ae, deviceMap[ae.get_From()], deviceMap[ae.get_To()]);

		edges->insert(edge);
	}

	return edges;
}

using namespace anygraph;
static void SetAttributes(P2PEdgePtr edge, AnyEdge ae, AnyNode srcDevice, AnyNode dstDevice) {

	// edge's attributes
	for (auto att : ae.get_Attribute()) {
		string name = att.get_attribute_name();
		string data = att.get_attribute_data();

		if (name == "Delay")
			edge->SetDelay(data);
		else
			NS_LOG_WARN("Unknown attribute: \"" << name <<"\"");
	}

	// source device's attributes
	for (auto att : srcDevice.get_attribute_list()) {
		string name = att.get_attribute_name();
		string data = att.get_attribute_data();

		if (name == "DataRate")
			edge->SetDataRateSrc(data);

		else if (name == "MTU") {
			unsigned long ul = stoul(data, nullptr, 0);
			if (SHRT_MAX < ul)
				throw range_error("Invalid value of MTUSrc, " + data);
			else
				edge->SetMTUSrc(static_cast<short>(ul));
		}

		else if (name == "InterframeGap")
			edge->SetInterframeGapSrc(data);

		else
			NS_LOG_WARN("Unknown attribute: \"" << name <<"\"");
	}

	// dest device's attributes
	for (auto att : dstDevice.get_attribute_list()) {
		string name = att.get_attribute_name();
		string data = att.get_attribute_data();

		if (name == "DataRate")
			edge->SetDataRateDst(data);

		else if (name == "MTU") {
			unsigned long ul = stoul(data, nullptr, 0);
			if (SHRT_MAX < ul)
				throw range_error("Invalid value of MTUSrc, " + data);
			else
				edge->SetMTUDst(static_cast<short>(ul));
		}

		else if (name == "InterframeGap")
			edge->SetInterframeGapDst(data);

		else
			NS_LOG_WARN("Unknown attribute: \"" << name <<"\"");
	}
}

} /* namespace simul */

