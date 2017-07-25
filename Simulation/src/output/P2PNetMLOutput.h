/*
 * P2PNetMLOutput.h
 *
 *  Created on: May 9, 2017
 *      Author: Kiet Vo
 */

#ifndef OUTPUT_P2PNETMLOUTPUT_H_
#define OUTPUT_P2PNETMLOUTPUT_H_

#include "IGraphOutput.h"

namespace simul {

/**
 * \ingroup output
 */
class P2PNetMLOutput: public IGraphOutput {
public:
	P2PNetMLOutput();
	virtual ~P2PNetMLOutput();

	virtual bool WriteGraph(SetPtr<EdgePtr>, string filename) override;
};

} /* namespace simul */

#endif /* OUTPUT_P2PNETMLOUTPUT_H_ */
