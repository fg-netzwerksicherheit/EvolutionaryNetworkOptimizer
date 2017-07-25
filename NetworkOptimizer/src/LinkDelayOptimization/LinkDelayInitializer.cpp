/*
 * LinkDelayInitializer.cpp
 *
 *  Created on: May 23, 2017
 *      Author: kiet
 */

#include "Simul.h"
#include "LinkDelayInitializer.h"
#include "LinkDelayEvaluator.h"

EA_TYPEINFO_CUSTOM_IMPL(LinkDelayInitializer){
return *ea::TypeInfo("LinkDelayInitializer")
.SetConstructor<LinkDelayInitializer, std::string>("graph-file");
}

LinkDelayInitializer::LinkDelayInitializer(std::string p_graphFile) :
		genomeLength(simul::P2PNetMLInput().ReadGraph(p_graphFile)->size()) {
}

LinkDelayInitializer::~LinkDelayInitializer() {
}

using namespace ea;

GenomePoolPtr LinkDelayInitializer::DoInitialize(uint pSize) {
	VectorPtr<GenomePtr> genomes = std::make_shared<std::vector<GenomePtr>>(pSize);
	std::uniform_real_distribution<double> dis(0, DEFAULT_LINK_DELAY * 2);

	for (uint i = 0; i < pSize; i++) {
		std::vector<double> genes(genomeLength);

		for (uint j = 0; j < genomeLength; j++)
			genes[j] = dis(ea::Random::generator);

		(*genomes)[i] = std::make_shared<DoubleArrayGenome>(genes);
	}

	return std::make_shared<GenomePool>(genomes->begin(), genomes->end());
}

