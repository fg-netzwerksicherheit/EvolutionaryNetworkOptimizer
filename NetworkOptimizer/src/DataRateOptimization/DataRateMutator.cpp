/*
 * NetworkMutator.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham
 */

#include "DataRateMutator.h"

EA_TYPEINFO_CUSTOM_IMPL(DataRateMutator){
return *ea::TypeInfo("DataRateMutator")
.SetConstructor<DataRateMutator, double, double, double>("gene-mutation-rate", "min-datarate", "max-datarate");
}

using namespace ea;
using namespace std;

DataRateMutator::DataRateMutator(double gmr, double mindr, double maxdr) :
		geneMutationRate(gmr), minDataRate(mindr), maxDataRate(maxdr) {

	assert(0 <= geneMutationRate && geneMutationRate <= 1);
	assert(0 <= minDataRate && minDataRate <= maxDataRate);
}

DataRateMutator::~DataRateMutator() {
}

shared_ptr<DataRateGenome> DataRateMutator::DoApply(const shared_ptr<DataRateGenome>& pTarget) {

	shared_ptr<DataRateGenome> result = make_shared<DataRateGenome>();

	vector<double> DataRateSrc = pTarget->getDataRateSrc();
	vector<double> DataRateDst = pTarget->getDataRateDst();

	DoubleRandomizer drRand(minDataRate, maxDataRate);	// data rate values

	for (uint i = 0; i < DataRateSrc.size(); ++i)
		if (DoubleRandomizer(0, 1).Get() < geneMutationRate) {
			DataRateSrc[i] = drRand.Get();
			DataRateDst[i] = drRand.Get();
		}

	result->setDataRateSrc(DataRateSrc);
	result->setDataRateDst(DataRateDst);

	return result;
}

