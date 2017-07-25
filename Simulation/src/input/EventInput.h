/*
 * EventInput.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Quang Pham
 */

#ifndef INPUT_EVENTINPUT_H_
#define INPUT_EVENTINPUT_H_

#include "../input/IEventInput.h"

namespace simul {

/**
 * \ingroup input
 */
class EventInput: public IEventInput {

public:
	EventInput();
	virtual ~EventInput();
	virtual vector<weak_ptr<Event>> ReadEvent(string path, SimulationPtr si);
};

} /* namespace std */

#endif /* INPUT_EVENTINPUT_H_ */
