/*
 * LinkArrangementExportHook.h
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#ifndef LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTEXPORTHOOK_H_
#define LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTEXPORTHOOK_H_

#include <openea/EA.h>

/**
 * \ingroup linkarrangement
 *
 * \brief Hook to export the graph file of the best network instance after each generation.
 *
 * Depending on the types of fitness used, the output files' names can have different meanings:
 \verbatim
 |	fitness types	|		file name formats				|
 -------------------------------------------------------------
 |	non-scalar		|	la-<generation>-<data>-<cost>		|
 |	scalar			|	la-<generation>-<fitness>-<cost>	|
 \endverbatim
 *
 * \see LinkArrangementEvaluator
 * \see LinkArrangementScalarEvaluator
 */
class LinkArrangementExportHook: public ea::Hook {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkArrangementExportHook(bool exportGraph = false);
	virtual ~LinkArrangementExportHook();

protected:
	virtual void DoGenerational() override;

private:
	bool exportGraph;
};

REGISTER_ADDON(LinkArrangementExportHook)

#endif /* LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTEXPORTHOOK_H_ */
