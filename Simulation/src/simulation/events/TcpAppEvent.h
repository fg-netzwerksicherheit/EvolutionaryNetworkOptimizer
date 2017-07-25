/*
 * TcpSocketEvent.h
 *
 *  Created on: Apr 7, 2017
 *      Author: Quang Pham, Kiet Vo
 */

#ifndef SIMULATION_EVENTS_TCPAPPEVENT_H_
#define SIMULATION_EVENTS_TCPAPPEVENT_H_

#include "Event.h"

namespace simul {

/**
 * \ingroup events
 *
 * \brief Event implementation that uses applications and communicates via tcp protocol.
 *
 * Even though this class is re-usable, the underlying OnOffApp it employed is not.
 * Each instance of OnOffApp can only be used once but only be disposed when the simulator is destroyed.
 * That means each time it is reused, a new application needs to be installed over the old ones
 * whereas the old ones will only be destroyed when the simulator is configured again.
 *
 * The following code sample can cause memory leak:
 * \code
 * Simulation si;
 * ...;
 * si.SetUpNetworks();
 *
 * while (true)
 *   si.Run();
 * \endcode
 *
 * The solution is not relying too much on its partial re-usability:
 * \code
 * Simulation si;
 * ...;
 * while (true) {
 *   si.SetUpNetworks();
 *   si.Run();
 * }
 * \endcode
 * or
 * \code
 * Simulation si;
 * ...;
 * for (uint i=0; i<10000; i++) {
 *   if (i%100 == 0)
 *     si.SetUpNetworks();
 *   si.Run();
 * }
 * \endcode
 *
 * Consider using getCurrentRSS() to check for memory growth if re-using this event/app/simulator
 * is unavoidable.
 *
 * \see Simulation::SetUpNetworks()
 * \see getCurrentRSS()
 */
class TcpAppEvent: public Event {
	friend class Simulation;

private:
	uint32_t maxBytes;

	double startTime = -1;
	virtual bool IsValidToSetStartTime(double startTime) override;
	virtual void DoSetStartTime(double startTime) override;

	bool hasStartTime = false;
	virtual bool IsValidToSetUp() override;
	virtual void DoSetUp() override;

	virtual void NotifySimulatorDestroyed() override;

protected:
	TcpAppEvent(NodePtr source, NodePtr dest, uint32_t bytes);

public:
	virtual ~TcpAppEvent();

	virtual uint64_t GetTotalDataSize() override;
	virtual bool IsSuccessful() override;
};

} /* namespace simul */

#endif /* SIMULATION_EVENTS_TCPAPPEVENT_H_ */
