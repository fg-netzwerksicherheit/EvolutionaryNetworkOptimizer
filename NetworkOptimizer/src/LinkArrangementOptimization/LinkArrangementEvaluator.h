/*
 * LinkArrangementEvaluator.h
 *
 *  Created on: May 26, 2017
 *      Author: Kiet Vo
 */

#ifndef LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTEVALUATOR_H_
#define LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTEVALUATOR_H_

#include "LinkArrangementDecoder.h"
#include "LinkArrangementFitness.h"

/**
 * \ingroup linkarrangement
 *
 * \brief Evaluate network instances by running pre-defined events.
 *
 * Events can be read from file (user-defined) or generated by this class (number of computers is required).
 * There are two constructors for these two options.
 *
 * This class employed LinkArrangementFitness, which is a non-scalar type, to evaluate a network instance.
 *
 * \see LinkArrangementFitness
 */
class LinkArrangementEvaluator: public ea::IndividualEvaluator {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkArrangementEvaluator(std::string eventFile, double budget);
	LinkArrangementEvaluator(uint numComputers, double budget);
	virtual ~LinkArrangementEvaluator();

protected:
	virtual ea::FitnessPtr DoEvaluate(const ea::GenomePtr& pGenome) override;

private:
	std::string eventFile;
	long numComputers = -1;
	double budget = -1;

	LinkArrangementDecoder decoder;
	uint filenameCounter = 0;
};

REGISTER_ADDON(LinkArrangementEvaluator)

#endif /* LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTEVALUATOR_H_ */
