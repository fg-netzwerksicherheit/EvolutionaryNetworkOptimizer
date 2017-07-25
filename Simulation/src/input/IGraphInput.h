/*
 * IGraphInput.h
 *
 *  Created on: Apr 9, 2017
 *      Author: Kiet Vo
 */

#ifndef INPUT_IGRAPHINPUT_H_
#define INPUT_IGRAPHINPUT_H_

#include "../Si_Type.h"

namespace simul {

/**
 * \defgroup input Input
 */

/**
 * \ingroup input
 */
class IGraphInput {
public:
	virtual ~IGraphInput() {
	}

	virtual SetPtr<EdgePtr> ReadGraph(string path) = 0;
	virtual SetPtr<EdgePtr> ParseGraph(TopologyPtr topology) = 0;
};

} /* namespace simul */

#endif /* INPUT_IGRAPHINPUT_H_ */
