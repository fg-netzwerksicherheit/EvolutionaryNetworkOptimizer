/*
 * NetworkCrossover.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham
 */

#include "DataRateRecombinator.h"

EA_TYPEINFO_CUSTOM_IMPL(DataRateRecombinator){
return *ea::TypeInfo("DataRateRecombinator")
.SetConstructor<DataRateRecombinator>();
}

using namespace ea;
using namespace std;

DataRateRecombinator::DataRateRecombinator() {
}

DataRateRecombinator::~DataRateRecombinator() {
}

shared_ptr<DataRateGenome> DataRateRecombinator::DoCombine(
		vector<shared_ptr<DataRateGenome>>& pParents) {

	shared_ptr<DataRateGenome> result = make_shared<DataRateGenome>();
	shared_ptr<DataRateGenome> parent0 = pParents[0];
	shared_ptr<DataRateGenome> parent1 = pParents[1];

	vector<double> DataRateSrc0 = parent0->getDataRateSrc();
	vector<double> DataRateSrc1 = parent1->getDataRateSrc();

	vector<double> DataRateDst0 = parent0->getDataRateDst();
	vector<double> DataRateDst1 = parent1->getDataRateDst();

	DoubleRandomizer alpha(0, 1);

	vector<vector<double>> DataRateSrc_children = WholeArithmeticCrossover(DataRateSrc0,
			DataRateSrc1, alpha.Get());
	vector<vector<double>> DataRateDst_children = WholeArithmeticCrossover(DataRateDst0,
			DataRateDst1, alpha.Get());

	result->setDataRateSrc(DataRateSrc_children[0]);
	result->setDataRateDst(DataRateDst_children[0]);

	return result;
}

uint DataRateRecombinator::GetParentCount() {
	return 2;
}
