/*
 * LinkArrangementMain.cpp
 *
 *  Created on: May 24, 2017
 *      Author: kiet
 */

#include "LinkArrangementInitializer.h"
#include "LinkArrangementRandomizer.h"
#include "LinkArrangementExportHook.h"
#include "LinkArrangementScalarEvaluator.h"
#include "LinkArrangementEvaluator.h"
#include "LinkArrangementOutputHook.h"
#include "../tools/tools.h"

using namespace ea;
using namespace simul;

int LinkArrangementMain(bool cluster) {

	//string eventFile = getPathToEventFile();
	double budget = 10;
	uint numComputers = 10;
	uint popSize = 20;

	Log::Redirect(Log::DEBUG, "stdout");

	std::shared_ptr<LinkArrangementRandomizer> randomizer = std::make_shared<
			LinkArrangementRandomizer>();

	EvolutionStrategyPtr es = make_shared<EvolutionStrategy>(popSize);

	es->initializer.Create<LinkArrangementInitializer>(numComputers);
	auto evaluator = es->evaluator.Create<LinkArrangementEvaluator>(numComputers, budget);
	es->recombinators.CreateBase<IntUniformCrossover>()->Parent<UniformSelection>()->Ratio(1);
	es->mutators.CreateBase<IntPointResetMutation>(0.2, randomizer)->Rate(0.3);
	es->survivalSelector.Create<GreedySelection>();

	es->hooks.Create<LinkArrangementOutputHook>(false);
	es->hooks.Create<LinkArrangementExportHook>(false);
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

