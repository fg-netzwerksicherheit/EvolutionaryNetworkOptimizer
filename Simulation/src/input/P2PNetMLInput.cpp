/*
 * GraphMLInput.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: Kiet Vo
 */

#include "P2PNetMLInput.h"

#include "../components/Node.h"
#include "../components/Edge.h"
#include "../components/P2PEdge.h"
#include "../simulation/Simulation.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::P2PNetMLInput");

P2PNetMLInput::P2PNetMLInput() {
}

P2PNetMLInput::~P2PNetMLInput() {
}

static void SetAttributes(shared_ptr<P2PEdge> edge, vector<anygraph::Attributes> attributes);

SetPtr<EdgePtr> P2PNetMLInput::ReadGraph(string path) {
	return ParseGraph(anygraph::P2PNetMLHandler().ReadGraph(path));
}

SetPtr<EdgePtr> P2PNetMLInput::ParseGraph(TopologyPtr topology) {

	// create a node map
	unordered_map<string, NodePtr> nodeMap;
	for (anygraph::AnyNode an : topology->get_node()) {
		string id = an.get_ID();
		NodePtr aNode = make_shared<Node>();
		aNode->SetID(id);
		nodeMap[id] = aNode;
	}

	// create edges for Simulation
	SetPtr<EdgePtr> edges = make_shared<set<EdgePtr>>();
	for (anygraph::AnyEdge ae : topology->get_edge()) {
		NodePtr n1 = nodeMap[ae.get_From()];
		NodePtr n2 = nodeMap[ae.get_To()];

		shared_ptr<P2PEdge> edge = make_shared<P2PEdge>(n1, n2);
		SetAttributes(edge, ae.get_Attribute());

		edges->insert(edge);
	}

	return edges;
}

static void SetAttributes(shared_ptr<P2PEdge> edge, vector<anygraph::Attributes> attributes) {
	for (anygraph::Attributes att : attributes) {
		string name = att.get_attribute_name();
		string data = att.get_attribute_data();

		if (name == "DataRateSrc")
			edge->SetDataRateSrc(data);
		else if (name == "DataRateDst")
			edge->SetDataRateDst(data);

		else if (name == "MTUSrc") {
			unsigned long ul = stoul(data, nullptr, 0);
			if (SHRT_MAX < ul)
				throw range_error("Invalid value of MTUSrc, " + data);
			else
				edge->SetMTUSrc(static_cast<short>(ul));
		}

		else if (name == "MTUDst") {
			unsigned long ul = stoul(data, nullptr, 0);
			if (SHRT_MAX < ul)
				throw range_error("Invalid value of MTUDst, " + data);
			else
				edge->SetMTUDst(static_cast<short>(ul));
		}

		else if (name == "InterframeGapSrc")
			edge->SetInterframeGapSrc(data);
		else if (name == "InterframeGapDst")
			edge->SetInterframeGapDst(data);

		else if (name == "Delay")
			edge->SetDelay(data);

		else
			NS_LOG_WARN("Unknown attribute: \"" << name <<"\"");
	}
}

} /* namespace simul */
