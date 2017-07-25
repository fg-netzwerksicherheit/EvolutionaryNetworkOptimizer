/*
 * GraphMLInput.h
 *
 *  Created on: Jun 17, 2017
 *      Author: Kiet Vo
 */

#ifndef INPUT_GRAPHMLINPUT_H_
#define INPUT_GRAPHMLINPUT_H_

#include "IGraphInput.h"

namespace simul {

/**
 * \ingroup input
 */
class GraphMLInput: public IGraphInput {
public:
	GraphMLInput();
	virtual ~GraphMLInput();

	virtual SetPtr<EdgePtr> ReadGraph(string path) override;
	virtual SetPtr<EdgePtr> ParseGraph(TopologyPtr topology) override;
};

} /* namespace simul */

#endif /* INPUT_GRAPHMLINPUT_H_ */
