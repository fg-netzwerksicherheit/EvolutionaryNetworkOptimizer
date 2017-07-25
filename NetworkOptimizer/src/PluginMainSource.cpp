/*
 * PluginMainSource.h
 *
 *  Created on: May 23, 2017
 *      Author: kiet
 */

#ifndef PLUGINMAINSOURCE_CC_
#define PLUGINMAINSOURCE_CC_

#define EA_PLUGIN_MAIN_SOURCE

#include "Simul.h" // because of log conflict

#include "DataRateOptimization/DataRateGenome.h"
#include "DataRateOptimization/DataRateInitializer.h"
#include "DataRateOptimization/DataRateMutator.h"
#include "DataRateOptimization/DataRateRecombinator.h"
#include "DataRateOptimization/DataRateScalarEvaluator.h"

#include "LinkDelayOptimization/LinkDelayEvaluator.h"
#include "LinkDelayOptimization/LinkDelayInitializer.h"

#include "LinkArrangementOptimization/LinkArrangementRandomizer.h"
#include "LinkArrangementOptimization/LinkArrangementInitializer.h"
#include "LinkArrangementOptimization/LinkArrangementExportHook.h"
#include "LinkArrangementOptimization/LinkArrangementEvaluator.h"
#include "LinkArrangementOptimization/LinkArrangementScalarEvaluator.h"
#include "LinkArrangementOptimization/LinkArrangementFitness.h"
#include "LinkArrangementOptimization/LinkArrangementOutputHook.h"

#endif /* PLUGINMAINSOURCE_CC_ */
