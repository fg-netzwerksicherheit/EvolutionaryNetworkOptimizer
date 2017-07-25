/*
 * DataRateScalarEvaluator.h
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham, Kiet Vo
 */

#ifndef DATARATEOPTIMIZATION_DATARATESCALAREVALUATOR_H_
#define DATARATEOPTIMIZATION_DATARATESCALAREVALUATOR_H_

#include "Simul.h"

#include "DataRateGenome.h"

using namespace ea;

/**
 * \ingroup datarate
 *
 * \brief Evaluate a network instance by running simulation with events.
 *
 * The cost of a network corresponds to the total data-rate values (in "Mbps").
 * For instance, a network interface with data-rate value of 7.3 Mbps will have the cost of 7.3.
 * The cost of a good network instance should not exceed the budget.
 */
class DataRateScalarEvaluator: public ea::ScalarEvaluator {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	DataRateScalarEvaluator(std::string graphFile, std::string eventFile, double p_budget);
	virtual ~DataRateScalarEvaluator();

protected:
	virtual double DoScalarEvaluate(const GenomePtr& pGenome) override;

private:
	simul::SetPtr<simul::EdgePtr> edgeList;
	simul::SimulationPtr si;
	double budget;

	virtual inline bool IsMaximizer() {
		return true;
	}
};

REGISTER_ADDON(DataRateScalarEvaluator)

#endif /* DATARATEOPTIMIZATION_DATARATESCALAREVALUATOR_H_ */
