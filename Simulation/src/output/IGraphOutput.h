/*
 * IGraphOutput.h
 *
 *  Created on: May 9, 2017
 *      Author: Kiet Vo
 */

#ifndef OUTPUT_IGRAPHOUTPUT_H_
#define OUTPUT_IGRAPHOUTPUT_H_

#include "../Si_Type.h"

namespace simul {

/**
 * \defgroup output Output
 */

/**
 * \ingroup output
 */
class IGraphOutput {
public:
	virtual ~IGraphOutput() {
	}

	virtual bool WriteGraph(SetPtr<EdgePtr> edgeList, string filename) = 0;
};

} /* namespace simul */

#endif /* OUTPUT_IGRAPHOUTPUT_H_ */
