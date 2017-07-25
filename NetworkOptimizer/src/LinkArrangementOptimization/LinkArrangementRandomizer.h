/*
 * LinkArrangementRandomizer.h
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#ifndef LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTRANDOMIZER_H_
#define LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTRANDOMIZER_H_

#include <openea/EA.h>

/**
 * \ingroup linkarrangement
 *
 * \brief Randomise values (defined by LinkArrangementDecoder) for genomes.
 *
 * \see LinkArrangementDecoder
 */
class LinkArrangementRandomizer: public ea::IntRandomizer {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkArrangementRandomizer();
	virtual ~LinkArrangementRandomizer();
};

REGISTER_ADDON(LinkArrangementRandomizer)

#endif /* LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTRANDOMIZER_H_ */
