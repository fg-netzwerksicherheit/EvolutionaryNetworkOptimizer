/*
 * DataRateMain.cpp
 *
 *  Created on: Apr 24, 2017
 *      Author: Quang Pham
 */

#include "Simul.h"

#include "../tools/tools.h"
#include "DataRateGenome.h"
#include "DataRateInitializer.h"
#include "DataRateMutator.h"
#include "DataRateRecombinator.h"
#include "DataRateScalarEvaluator.h"

using namespace ea;
using namespace simul;

int DataRateMain(bool cluster) {

	Log::Redirect(Log::DEBUG, "stdout");

	//Simulation::EnableAllLogComponents();

	string graphFile = GetProjectNetOptDirectory() + "/resources/input/use_case1a.p2pnetml";
	string eventFile = GetProjectNetOptDirectory() + "/resources/input/use_case1a_events.json";

	double budget = 40;

	EvolutionStrategyPtr es = make_shared<EvolutionStrategy>(20);

	es->initializer.Create<DataRateInitializer>(graphFile, 1, 200);
	auto evaluator = es->evaluator.Create<DataRateScalarEvaluator>(graphFile, eventFile, budget);
	es->recombinators.CreateBase<DataRateRecombinator>()->Parent<UniformSelection>(false)->Ratio(1);
	es->mutators.CreateBase<DataRateMutator>(0.3, 1, 200)->Rate(0.2);
	es->survivalSelector.Create<GreedySelection>();

	es->hooks.Create<StandardOutputHook>(true);
	es->hooks.Create<GenerationTerminationHook>(100000);

	if (cluster)
		Cluster::AddOperator(evaluator);
	else
		MultiThreading::SetNumThreads(1);

	es->Evolve();

	if (cluster)
		Cluster::Destroy();

	return 0;
}

