/*
 * NetworkInitializer.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham
 */

#include "DataRateInitializer.h"
#include "Simul.h"

EA_TYPEINFO_CUSTOM_IMPL(DataRateInitializer){
return *ea::TypeInfo("DataRateInitializer")
.SetConstructor<DataRateInitializer, std::string, double, double>("graph-file","min-datarate", "max-datarate");
}

using namespace ea;
using namespace std;

DataRateInitializer::DataRateInitializer(std::string graphFile, double p_min, double p_max) :
		m_length(simul::P2PNetMLInput().ReadGraph(graphFile)->size()), minRate(p_min), maxRate(
				p_max) {
}

DataRateInitializer::~DataRateInitializer() {
}

GenomePoolPtr DataRateInitializer::DoInitialize(uint pSize) {

	DoubleRandomizer drRand(minRate, maxRate);
	vector<GenomePtr> genomeList(pSize, nullptr);

	for (uint i = 0; i < pSize; ++i) {
		vector<double> DataRateSrc(m_length);
		vector<double> DataRateDst(m_length);

		for (uint j = 0; j < m_length; ++j) {
			DataRateDst[j] = drRand.Get();
			DataRateSrc[j] = drRand.Get();
		}
		genomeList[i] = make_shared<DataRateGenome>(DataRateSrc, DataRateDst);
	}

	return make_shared<GenomePool>(genomeList.cbegin(), genomeList.cend());
}

