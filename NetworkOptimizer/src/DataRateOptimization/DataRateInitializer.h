/*
 * DataRateInitializer.h
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham, Kiet Vo
 */

#ifndef DATARATEOPTIMIZATION_DATARATEINITIALIZER_H_
#define DATARATEOPTIMIZATION_DATARATEINITIALIZER_H_

#include "DataRateGenome.h"

/**
 * \ingroup datarate
 *
 * \brief Initialise a bunch of DataRateGenome with uniformly random data-rate values.
 *
 * Network topological information is read from the graph file.
 * Data-rate values should be of double type and in "Mbps" unit (e.g. 7.3 means "7.3 Mbps").
 *
 * \see DataRateGenome
 */
class DataRateInitializer: public ea::Initializer {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	DataRateInitializer(std::string graphFile, double minRate, double maxRate);
	virtual ~DataRateInitializer();

private:
	const uint m_length;
	const double minRate, maxRate;

	virtual GenomePoolPtr DoInitialize(uint pSize) override;
};

REGISTER_ADDON(DataRateInitializer)

#endif /* DATARATEOPTIMIZATION_DATARATEINITIALIZER_H_ */
