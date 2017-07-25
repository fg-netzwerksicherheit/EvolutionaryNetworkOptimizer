/*
 * Simulation.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef SIMULATION_SIMULATION_H_
#define SIMULATION_SIMULATION_H_

#define SI_LOWER_RSS	1024 * 1024 * 100

#include "../Si_Type.h"

namespace simul {

/**
 * \defgroup simulation Simulation
 */

/**
 * \ingroup simulation
 * \brief A partial wrapper of the ns-3's simulator and acts as a controller for the simulations.
 *
 * Only one object of this type can be valid at a time and so are its events.
 *
 * \see Simulation::SetUpNetworks()
 */
class Simulation {
private:
	set<NodePtr> parsedNodeList;

	vector<NetworkPtr> networkList;
	ns3::NetDeviceContainer globalDevices;

	vector<EventPtr> eventList;

	void Flush();

	vector<NetworkPtr> ConstructNetwork(set<EdgePtr> edgeList);
	NetworkPtr ConstructP2P(EdgePtr initialEdge, set<EdgePtr>& edgeList);
	NetworkPtr ConstructWifi(EdgePtr initialEdge, set<EdgePtr>& edgeList);

	// valid checker group
	static Simulation * configuredObjectID;
	void SetValidToRunBit(bool isValid);
	bool IsValidToRun();

protected:
	set<EdgePtr> globalEdgeList;

public:
	Simulation();
	virtual ~Simulation();

	void AddEdge(EdgePtr edge);
	void AddEdges(SetPtr<EdgePtr> edges);

	NodePtr GetNode(string ID);
	ns3::NetDeviceContainer FindDevicesFromNode(NodePtr node);

	void SetUpNetworks();

	template<class DerivedEvent, class ...Args>
	weak_ptr<Event> CreateEvent(Args&&...args);

	void Run(double duration);
	void Run();

	static void EnableAllLogComponents();
	static void EnableAllLogComponents(enum ns3::LogLevel level);
	static void DisableAllLogComponents();
	static void DisableAllLogComponents(enum ns3::LogLevel level);
};

/**
 * \brief Create an event and associate it with this simulation object.
 *
 * \param args list of parameters for a specific event's constructor.
 */
template<class DerivedEvent, class ...Args>
weak_ptr<Event> Simulation::CreateEvent(Args&&...args) {
	EventPtr event = shared_ptr<Event>(new DerivedEvent(forward<Args>(args)...));
	eventList.push_back(event);
	return weak_ptr<Event>(event);
}

} /* namespace simul */

#endif /* SIMULATION_SIMULATION_H_ */
