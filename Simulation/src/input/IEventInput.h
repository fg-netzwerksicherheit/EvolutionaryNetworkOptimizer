/*
 * IEventInput.h
 *
 *  Created on: Apr 11, 2017
 *      Author: Quang Pham
 */

#ifndef INPUT_IEVENTINPUT_H_
#define INPUT_IEVENTINPUT_H_

#include "../Si_Type.h"

namespace simul {

/**
 * \ingroup input
 */
class IEventInput {
public:
	virtual ~IEventInput() {
	}

	virtual vector<weak_ptr<Event>> ReadEvent(string path, SimulationPtr si) = 0;
};

} /* namespace simul */

#endif /* INPUT_IEVENTINPUT_H_ */
