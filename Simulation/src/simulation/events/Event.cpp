/*
 * Event.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Kiet Vo
 */

#include "Event.h"

#include "../../exceptions/NullPointerException.h"
#include "../../exceptions/UsageException.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::Event");

double Event::defaultEventLength = 10.0;
double Event::recommendedDuration = 0.0;

/**
 * \throw NullPointerException when a node is null.
 * \throw invalid_argument when 2 arguments are of the same node.
 */
Event::Event(NodePtr sourceNode, NodePtr destNode) {

	if (sourceNode == NULL || destNode == NULL) {
		string msg = "A Node is NULL!";
		NS_LOG_ERROR(msg);
		throw NullPointerException(msg);
	}

	if (sourceNode == destNode) {
		string msg = "Cannot set an event from a node to \"itself\"!";
		NS_LOG_ERROR(msg);
		throw invalid_argument(msg);
	}

	this->sourceNode = sourceNode;
	this->destNode = destNode;
}

Event::~Event() {
}

/**
 * This is required after constructing an event otherwise it will not run.
 * However, each event only need to call this once as this has nothing to do with the ns-3
 * simulator's configuration.
 *
 * \param startTime in seconds
 *
 * \see Simulation::Run()
 */
void Event::SetStartTime(double startTime) {
	if (IsValidToSetStartTime(startTime) == false)
		return;

	DoSetStartTime(startTime);

	// set duration based on start-time
	if (recommendedDuration < startTime + defaultEventLength)
		recommendedDuration += (startTime + defaultEventLength);
}

/**
 * \brief The "supposed" duration of an event.
 *
 * This value is only used to calculate the stop time for a simulation and has nothing to do with an event's real duration.
 * The duration of an event is always unknown until it has finished running.
 *
 * \see GetRecommendedDuration()
 * \see Simulation::Run(double)
 */
void Event::SetDefaultEventLength(double time) {
	if (time < 1) {
		string msg = "Default event length is too small!";
		NS_LOG_ERROR(msg);
		throw invalid_argument(msg);
	}

	defaultEventLength = time;
}

/**
 * \brief The "supposed" duration of an event.
 *
 * This value is only used to calculate the stop time for a simulation and has nothing to do with an event's real duration.
 * The duration of an event is always unknown until it has finished running.
 *
 * \see GetRecommendedDuration()
 * \see Simulation::Run(double)
 */
double Event::GetDefaultEventLength() {
	return defaultEventLength;
}

/**
 * \brief Recommended duration for this simulation.
 *
 * This value is computed from SetStartTime(double) and the event's length,
 * which can be accessed via GetDefaultEventLength() and SetDefaultEventLength(double).
 *
 * \return duration in seconds
 *
 * \see GetDefaultEventLength()
 * \see SetDefaultEventLength(double)
 */
double Event::GetRecommendedDuration() {
	return recommendedDuration;
}

//
// private
//
void Event::SetUp() {
	if (IsValidToSetUp() == false)
		return;

	DoSetUp();
}

} /* namespace simul */
