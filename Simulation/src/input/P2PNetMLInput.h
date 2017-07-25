/*
 * P2PNetMLInput.h
 *
 *  Created on: Apr 3, 2017
 *      Author: Kiet Vo
 */

#ifndef INPUT_P2PNETMLINPUT_H_
#define INPUT_P2PNETMLINPUT_H_

#include "../input/IGraphInput.h"

namespace simul {

/**
 * \ingroup input
 */
class P2PNetMLInput: public IGraphInput {
public:
	P2PNetMLInput();
	virtual ~P2PNetMLInput();

	virtual SetPtr<EdgePtr> ReadGraph(string path) override;
	virtual SetPtr<EdgePtr> ParseGraph(TopologyPtr topology) override;
};

} /* namespace simul */

#endif /* INPUT_P2PNETMLINPUT_H_ */
