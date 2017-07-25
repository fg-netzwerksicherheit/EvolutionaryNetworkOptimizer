/*
 * LinkArrangementDecoder.cpp
 *
 *  Created on: Jun 26, 2017
 *      Author: Kiet Vo
 */

#include "LinkArrangementDecoder.h"

using namespace std;
using namespace anygraph;

static shared_ptr<Topology> GenerateKeys(shared_ptr<Topology> topo);
vector<shared_ptr<AnyEdge>> static GetCodeBook();

LinkArrangementDecoder::LinkArrangementDecoder() {
	codebook = GetCodeBook();
}

LinkArrangementDecoder::~LinkArrangementDecoder() {
}

double LinkArrangementDecoder::ComputeCost(vector<int> genes) {
	double cost = 0;
	for (int gene : genes) {
		assert(gene >= 0);	//TODO: remove after adding exception
		cost += gene;
	}
	return cost;
}

/**
 * \brief Genotype to phenotype
 */
simul::SetPtr<simul::EdgePtr> LinkArrangementDecoder::DecodeToEdges(vector<int> genes) {
	return simul::P2PNetMLInput().ParseGraph(DecodeToTopology(genes));
}

/**
 * \brief To export graph file in p2pnetml format.
 * \see anygraph::P2PNetMLHandler::WriteGraph()
 */
shared_ptr<Topology> LinkArrangementDecoder::DecodeToTopology(vector<int> genes) {
	uint length = genes.size();
	uint NNodes = static_cast<uint>(sqrt(2 * length)) + 1;

	//TODO: remove after adding exceptions
	assert(length > 0);
	assert(length * 2 == NNodes * (NNodes - 1));

	shared_ptr<Topology> topo = make_shared<Topology>();

	// create nodes
	for (uint id = 0; id < NNodes; id++)
		topo->add_node(AnyNode("n" + to_string(id)));

	// create edges
	uint lowerGeneIndex = 0, upperGeneIndex = -1;
	for (uint srcID = 0; srcID < NNodes - 1; srcID++) {
		uint nNeighbours = NNodes - srcID - 1;
		lowerGeneIndex = upperGeneIndex + 1;
		upperGeneIndex = lowerGeneIndex + nNeighbours - 1;

		uint geneIndex = lowerGeneIndex;
		for (uint dstID = srcID + 1; dstID < NNodes; dstID++) {

			shared_ptr<AnyEdge> edge_ptr = codebook[genes[geneIndex]];
			if (edge_ptr != nullptr) {
				AnyEdge edge = *edge_ptr;
				edge.set_from("n" + to_string(srcID));
				edge.set_to("n" + to_string(dstID));
				topo->add_edge(edge);
			}

			geneIndex++;
		}
	}

	return GenerateKeys(topo);
}

// private
static shared_ptr<Topology> GenerateKeys(shared_ptr<Topology> topo) {
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

	return topo;
}

vector<shared_ptr<AnyEdge>> static GetCodeBook() {
	vector<shared_ptr<AnyEdge>> codebook(LA_DEC_CB_LENGTH, nullptr);

	for (uint i = 1; i < LA_DEC_CB_LENGTH; i++)
		codebook[i] = make_shared<AnyEdge>();

	Attributes drs, drd, igs, igd;
	drs.set_id("e_DataRateSrc"), drs.set_name("DataRateSrc");
	drd.set_id("e_DataRateDst"), drd.set_name("DataRateDst");
	igs.set_id("e_InterframeGapSrc"), igs.set_name("InterframeGapSrc");
	igd.set_id("e_InterframeGapDst"), igd.set_name("InterframeGapDst");

	// 1
	uint index = 1;
	drs.set_data("10Mbps");
	drd.set_data("10Mbps");
	igs.set_data("9.6us");
	igd.set_data("9.6us");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// 2
	index++;
	drs.set_data("100Mbps");
	drd.set_data("100Mbps");
	igs.set_data("0.96us");
	igd.set_data("0.96us");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// 3
	index++;
	drs.set_data("1Gbps");
	drd.set_data("1Gbps");
	igs.set_data("96ns");
	igd.set_data("96ns");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// 4
	index++;
	drs.set_data("2.5Gbps");
	drd.set_data("2.5Gbps");
	igs.set_data("38.4ns");
	igd.set_data("38.4ns");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// 5
	index++;
	drs.set_data("5Gbps");
	drd.set_data("5Gbps");
	igs.set_data("19.2ns");
	igd.set_data("19.2ns");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// 6
	index++;
	drs.set_data("10Gbps");
	drd.set_data("10Gbps");
	igs.set_data("9.6ns");
	igd.set_data("9.6ns");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// 7
	index++;
	drs.set_data("40Gbps");
	drd.set_data("40Gbps");
	igs.set_data("2.4ns");
	igd.set_data("2.4ns");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// 8
	index++;
	drs.set_data("100Gbps");
	drd.set_data("100Gbps");
	igs.set_data("0.96ns");
	igd.set_data("0.96ns");
	codebook[index]->add_attribute(Attributes(drs));
	codebook[index]->add_attribute(Attributes(drd));
	codebook[index]->add_attribute(Attributes(igs));
	codebook[index]->add_attribute(Attributes(igd));

	// TODO: remove later
	assert(index == 8);

	return codebook;
}

