/*
 * DataRateMutator.h
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham
 */

#ifndef DATARATEOPTIMIZATION_DATARATEMUTATOR_H_
#define DATARATEOPTIMIZATION_DATARATEMUTATOR_H_

#include "DataRateGenome.h"

using namespace ea;

/**
 * \ingroup datarate
 *
 * \brief Random resetting mutation.
 *
 * If a genome is chosen to be mutated, each of its genes will be mutated by resetting a new value from the uniform distribution.
 * The distribution range and mutation rate for each gene is specified in the constructor.
 * Data-rate values should be of double type and in "Mbps" unit (e.g. 7.3 means "7.3 Mbps").
 *
 * \see DataRateGenome
 * \see DataRateInitializer
 */
class DataRateMutator: public ea::TypedMutator<DataRateGenome> {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	DataRateMutator(double geneMutationRate, double minDataRate, double maxDataRate);
	virtual ~DataRateMutator();

private:
	double geneMutationRate;
	double minDataRate, maxDataRate;

	virtual shared_ptr<DataRateGenome> DoApply(const shared_ptr<DataRateGenome>& pTarget)
	override final;
};

REGISTER_ADDON(DataRateMutator)

#endif /* DATARATEOPTIMIZATION_DATARATEMUTATOR_H_ */
