/*
 * LinkArrangementOutputHook.h
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#ifndef LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTOUTPUTHOOK_H_
#define LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTOUTPUTHOOK_H_

#include <openea/EA.h>

/**
 * \ingroup linkarrangement
 *
 * \brief Print non-scalar fitness of the best candidate solution after each generation.
 *
 * \attention
 * This class is meant only for LinkArrangementFitness, do not use for other types of fitness.
 *
 * \see LinkArrangementFitness
 */
class LinkArrangementOutputHook: public ea::Hook {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkArrangementOutputHook(bool printGenome = false);
	virtual ~LinkArrangementOutputHook();

protected:
	virtual void DoGenerational() override;

private:
	bool printGenome;
};

REGISTER_ADDON(LinkArrangementOutputHook)

#endif /* LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTOUTPUTHOOK_H_ */
