/*
 * LinkArrangementInitializer.h
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#ifndef LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTINITIALIZER_H_
#define LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTINITIALIZER_H_

#include <openea/EA.h>

/**
 * \ingroup linkarrangement
 *
 * \brief Initialise a bunch of genomes (representations of network instances) using
 * LinkArrangementRandomizer.
 *
 * Each genome contains a vector of N(N-1)/2 elements where N is the number of computers.
 *
 * \see LinkArrangementDecoder
 */
class LinkArrangementInitializer: public ea::IntRandomArrayInitializer {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkArrangementInitializer(uint numComputers);
	virtual ~LinkArrangementInitializer();
};

REGISTER_ADDON(LinkArrangementInitializer)

#endif /* LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTINITIALIZER_H_ */
