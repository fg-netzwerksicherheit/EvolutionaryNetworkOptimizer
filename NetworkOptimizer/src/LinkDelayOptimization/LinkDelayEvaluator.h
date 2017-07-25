/*
 * LinkDelayEvaluator.h
 *
 *  Created on: May 15, 2017
 *      Author: quang
 */

#ifndef LINKDELAYOPTIMIZATION_LINKDELAYEVALUATOR_H_
#define LINKDELAYOPTIMIZATION_LINKDELAYEVALUATOR_H_

#include "Simul.h"
#include <openea/EA.h>

#define DEFAULT_LINK_DELAY	4	// 4 seconds for each link

/**
 * \defgroup linkdelay	Link Delay Optimization
 *
 * \brief Only works with p2pnetml file.
 */

/**
 * \ingroup linkdelay
 *
 * \brief Evaluate network instances by running simulated events.
 *
 * Each link in a network has a default propagation delay value of 4 seconds.
 * Each second decrement will cost you 1$.
 * The budget value describes how much you can reduce the total link (propagation) delays in your network.
 *
 * \see simul::P2PEdge
 */
class LinkDelayEvaluator: public ea::ScalarEvaluator {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkDelayEvaluator (std::string graphFile, std::string eventFile, double budget);
	virtual ~LinkDelayEvaluator();

protected:
	virtual double DoScalarEvaluate(const ea::GenomePtr& pGenome) override;

private:
	simul::SimulationPtr si;
	simul::SetPtr<simul::EdgePtr> edgeList;
	double budget;
	uint64_t totalExpectedData;

	virtual inline bool IsMaximizer() {
		return false;
	}
};

REGISTER_ADDON(LinkDelayEvaluator)

#endif /* LINKDELAYOPTIMIZATION_LINKDELAYEVALUATOR_H_ */
