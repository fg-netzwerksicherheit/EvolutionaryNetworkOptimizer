/*
 * LinkDelayMain.cpp
 *
 *  Created on: May 16, 2017
 *      Author: Kiet Vo
 */

#include "LinkDelayEvaluator.h"
#include "LinkDelayInitializer.h"
#include "../tools/tools.h"

using namespace ea;
using namespace simul;

int LinkDelayMain(bool cluster) {

	Log::Redirect(Log::DEBUG, "stdout");
	//Simulation::EnableAllLogComponents();

	string graphFile = GetProjectNetOptDirectory() + "/resources/input/use_case1a.p2pnetml";
	string eventFile = GetProjectNetOptDirectory() + "/resources/input/use_case1a_events.json";

	SetPtr<EdgePtr> edgelist = P2PNetMLInput().ReadGraph(graphFile);

	double budget = 8;

	EvolutionStrategyPtr es = make_shared<EvolutionStrategy>(50);

	es->initializer.Create<LinkDelayInitializer>(graphFile);
	auto evaluator = es->evaluator.Create<LinkDelayEvaluator>(graphFile, eventFile, budget);
	es->recombinators.CreateBase<DoubleUniformCrossover>()->Parent<UniformSelection>(false)->Ratio(
			1);
	es->mutators.CreateBase<DoublePointResetMutation>(0.3,
			make_shared<DoubleRandomizer>(0, DEFAULT_LINK_DELAY * 2))->Rate(0.4);
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

