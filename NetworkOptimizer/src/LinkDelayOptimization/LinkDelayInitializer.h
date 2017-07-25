/*
 * LinkDelayInitializer.h
 *
 *  Created on: May 23, 2017
 *      Author: kiet
 */

#ifndef LINKDELAYOPTIMIZATION_LINKDELAYINITIALIZER_H_
#define LINKDELAYOPTIMIZATION_LINKDELAYINITIALIZER_H_

#include <openea/EA.h>

/**
 * \ingroup linkdelay
 *
 * \brief Initialise a bunch of network instances with various link propagation delay values.
 *
 * \see LinkDelayEvaluator
 * \see simul::P2PEdge
 */
class LinkDelayInitializer: public ea::Initializer {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkDelayInitializer(std::string graphFile);
	virtual ~LinkDelayInitializer();

private:
	const size_t genomeLength;

	virtual ea::GenomePoolPtr DoInitialize(uint pSize) override;
};

REGISTER_ADDON(LinkDelayInitializer)

#endif /* LINKDELAYOPTIMIZATION_LINKDELAYINITIALIZER_H_ */
