/*
 * LinkArrangementExportHook.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#include "Simul.h"
#include "LinkArrangementFitness.h"
#include "LinkArrangementExportHook.h"
#include "LinkArrangementDecoder.h"

EA_TYPEINFO_CUSTOM_IMPL(LinkArrangementExportHook) {
	return *ea::TypeInfo("LinkArrangementExportHook").SetConstructor<LinkArrangementExportHook, bool>("export-graph");
}

LinkArrangementExportHook::LinkArrangementExportHook(bool p_exportGraph) :
		exportGraph(p_exportGraph) {
}

LinkArrangementExportHook::~LinkArrangementExportHook() {
}

using namespace ea;
using namespace std;

void LinkArrangementExportHook::DoGenerational() {
	if (exportGraph == false)
		return;

	OrganismPtr organism = GetBestOrganism();
	IntArrayGenomePtr genome = dynamic_pointer_cast<IntArrayGenome>(organism->GetGenome());

	LinkArrangementDecoder decoder;
	simul::TopologyPtr topo = decoder.DecodeToTopology(genome->GetGenes());

	FitnessPtr fitness = organism->GetFitness();
	ScalarFitnessPtr scalar = dynamic_pointer_cast<ScalarFitness>(fitness);

	string filename;
	// "la-<generation>-<fitness>-<cost>
	if (scalar)
		filename = "la-" + to_string(GetGeneration()) + "-"
				+ to_string(static_cast<uint>(scalar->GetValue())) + "-"
				+ to_string(static_cast<uint>(decoder.ComputeCost(genome->GetGenes()))) + ".xml";
	// "la-<generation>-<data>-<cost>
	else {
		shared_ptr<LinkArrangementFitness> laFitness = dynamic_pointer_cast<LinkArrangementFitness>(
				fitness);
		filename = "la-" + to_string(GetGeneration()) + "-"
				+ to_string(static_cast<uint>(laFitness->GetData())) + "-"
				+ to_string(static_cast<uint>(laFitness->GetCost())) + ".xml";
	}

	anygraph::P2PNetMLHandler().WriteGraph(topo, filename);
}
