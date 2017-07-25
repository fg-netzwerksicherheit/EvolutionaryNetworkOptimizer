/*
 * P2PNetMLOutput.cpp
 *
 *  Created on: May 9, 2017
 *      Author: Kiet Vo
 */

#include "P2PNetMLOutput.h"

#include "../components/Node.h"
#include "../components/Edge.h"
#include "../components/P2PEdge.h"

namespace simul {

P2PNetMLOutput::P2PNetMLOutput() {
}

P2PNetMLOutput::~P2PNetMLOutput() {
}

using namespace anygraph;

static void GenerateKeys(TopologyPtr topo);
static void GenerateNodesEdges(TopologyPtr topo, SetPtr<EdgePtr> edgeList);

bool P2PNetMLOutput::WriteGraph(SetPtr<EdgePtr> edgeList, string filename) {

	TopologyPtr topo = make_shared<anygraph::Topology>();

	GenerateKeys(topo);
	GenerateNodesEdges(topo, edgeList);

	anygraph::P2PNetMLHandler().WriteGraph(topo, filename);

	return true;
}

static void GenerateKeys(TopologyPtr topo) {
	{
		Attributes drs;
		drs.set_id("e_DataRateSrc");
		drs.set_name("DataRateSrc");
		drs.set_type("string");
		topo->add_attribute(drs);
	}
	{
		Attributes drd;
		drd.set_id("e_DataRateDst");
		drd.set_name("DataRateDst");
		drd.set_type("string");
		topo->add_attribute(drd);
	}
	{
		Attributes mtus;
		mtus.set_id("e_MTUSrc");
		mtus.set_name("MTUSrc");
		mtus.set_type("double");
		topo->add_attribute(mtus);
	}
	{
		Attributes mtud;
		mtud.set_id("e_MTUDst");
		mtud.set_name("MTUDst");
		mtud.set_type("double");
		topo->add_attribute(mtud);
	}
	{
		Attributes igs;
		igs.set_id("e_InterframeGapSrc");
		igs.set_name("InterframeGapSrc");
		igs.set_type("string");
		topo->add_attribute(igs);
	}
	{
		Attributes igd;
		igd.set_id("e_InterframeGapDst");
		igd.set_name("InterframeGapDst");
		igd.set_type("string");
		topo->add_attribute(igd);

	}
	{
		Attributes delay;
		delay.set_id("e_Delay");
		delay.set_name("Delay");
		delay.set_type("string");
		topo->add_attribute(delay);
	}
}

static void GenerateNodesEdges(TopologyPtr topo, SetPtr<EdgePtr> edgeList) {
	set<NodePtr> nodeList;
	for (auto it = edgeList->begin(); it != edgeList->end(); it++) {
		shared_ptr<P2PEdge> edge = dynamic_pointer_cast<P2PEdge>(*it);
		NodePtr src = edge->GetSourceNode();
		NodePtr dst = edge->GetDestNode();
		nodeList.insert(src);
		nodeList.insert(dst);

		AnyEdge ae;
		ae.set_from(src->GetID());
		ae.set_to(dst->GetID());
		{
			Attributes drs;
			drs.set_id("e_DataRateSrc");
			drs.set_data(edge->GetDataRateSrc());
			ae.add_attribute(drs);
		}
		{
			Attributes drd;
			drd.set_id("e_DataRateDst");
			drd.set_data(edge->GetDataRateDst());
			ae.add_attribute(drd);
		}
		{
			Attributes mtus;
			mtus.set_id("e_MTUSrc");
			mtus.set_data(to_string(edge->GetMTUSrc()));
			ae.add_attribute(mtus);
		}
		{
			Attributes mtud;
			mtud.set_id("e_MTUDst");
			mtud.set_data(to_string(edge->GetMTUDst()));
			ae.add_attribute(mtud);
		}
		{
			Attributes igs;
			igs.set_id("e_InterframeGapSrc");
			igs.set_data(edge->GetInterframeGapSrc());
			ae.add_attribute(igs);
		}
		{
			Attributes igd;
			igd.set_id("e_InterframeGapDst");
			igd.set_data(edge->GetInterframeGapDst());
			ae.add_attribute(igd);
		}
		{
			Attributes delay;
			delay.set_id("e_Delay");
			delay.set_data(edge->GetDelay());
			ae.add_attribute(delay);
		}
		topo->add_edge(ae);
	}

	for (auto it = nodeList.begin(); it != nodeList.end(); it++) {
		AnyNode node;
		node.set_ID((*it)->GetID());
		topo->add_node(node);
	}
}

} /* namespace simul */
