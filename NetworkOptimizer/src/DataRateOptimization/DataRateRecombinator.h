/*
 * DataRateRecombinator.h
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham
 */

#ifndef DATARATEOPTIMIZATION_DATARATERECOMBINATOR_H_
#define DATARATEOPTIMIZATION_DATARATERECOMBINATOR_H_

#include "../tools/tools.h"
#include "DataRateGenome.h"

/**
 * \ingroup datarate
 *
 * \brief Whole Arithmetic crossover between 2 parents.
 */
class DataRateRecombinator: public ea::TypedRecombinator<DataRateGenome> {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	DataRateRecombinator();
	virtual ~DataRateRecombinator();

	virtual uint GetParentCount();

private:

	virtual std::shared_ptr<DataRateGenome> DoCombine(
			std::vector<std::shared_ptr<DataRateGenome>>& pParents) override;
};

REGISTER_ADDON(DataRateRecombinator)

#endif /* DATARATEOPTIMIZATION_DATARATERECOMBINATOR_H_ */
