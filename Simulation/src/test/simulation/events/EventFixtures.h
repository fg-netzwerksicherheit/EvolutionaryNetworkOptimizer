/*
 * EventFixtures.h
 *
 *  Created on: Apr 19, 2017
 *      Author: Kiet Vo
 */

#ifndef TEST_SIMULATION_EVENTS_EVENTFIXTURES_H_
#define TEST_SIMULATION_EVENTS_EVENTFIXTURES_H_

#include "../../../Simul.h"
#include <chrono>

using namespace simul;

struct EventFixture_P2P {
	bool successful = false;

	template<class DerivedEvent, class ...Args>
	void Run01(int numNodes, Args&&... args);

	template<class DerivedEvent, class ...Args>
	void Run02(int numNodes, Args&&... args);

	template<class DerivedEvent, class ...Args>
	void Run03(Args&&... args);
};

struct EventFixture_Wifi {
	bool successful = false;

	template<class DerivedEvent, class ...Args>
	void Run01(int numNodes, int numApNodes, Args&&... args);

	template<class DerivedEvent, class ...Args>
	void Run02(int numApNodes, Args&&... args);
};

struct EventFixture_Mixed {
	bool successful = false;

	template<class DerivedEvent, class ...Args>
	void Run(int numWifiNets, Args&&... args);
};

//
//	implementation
//
/************************************
 *									*
 *			Point-to-Point			*
 *									*
 ************************************/

/**
 * Normal scenario with successful communication
 *
 * Every node is connected to the others
 * Select randomly 2 nodes to set up event
 */
template<class DerivedEvent, class ...Args>
void EventFixture_P2P::Run01(int numNodes, Args&&... args) {
	// important checkpoint
	assert(numNodes > 1);

	NodePtr node[numNodes];
	for (int i = 0; i < numNodes; i++)
		node[i] = make_shared<simul::Node>();

	EdgePtr edge;
	Simulation si;

	for (int i = 0; i < numNodes - 1; i++)
		for (int j = i + 1; j < numNodes; j++) {
			edge = make_shared<P2PEdge>(node[i], node[j]);
			si.AddEdge(edge);
		}

	for (int i = 0; i < 10; i++)
		si.SetUpNetworks();

	// random generator
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, numNodes - 1);

	int index1 = distribution(generator);
	int index2 = distribution(generator);

	while (index1 == index2)
		index2 = distribution(generator);

	// set up event
	weak_ptr<Event> event = si.CreateEvent<DerivedEvent>(node[index1], node[index2],
			forward<Args>(args)...);
//	event.lock()->SetUp();
//	event.lock()->Schedule(1.0);
	event.lock()->SetStartTime(1.0);
	si.Run(Event::GetRecommendedDuration());

	successful = event.lock()->IsSuccessful();
}

/**
 * Each node is connected to only "one" other node
 * Data is sent from one end to the other
 *
 * =========Illustration========
 * n0---n1---n2---n3-- ... ---nk
 * =============================
 *
 */
template<class DerivedEvent, class ...Args>
void EventFixture_P2P::Run02(int numNodes, Args&&... args) {
	// important checkpoint
	assert(numNodes > 1);

	NodePtr node[numNodes];
	for (int i = 0; i < numNodes; i++)
		node[i] = make_shared<simul::Node>();

	EdgePtr edge;
	Simulation si;

	for (int i = 0; i < numNodes - 1; i++) {
		edge = make_shared<P2PEdge>(node[i], node[i + 1]);
		si.AddEdge(edge);
	}

	for (int i = 0; i < 10; i++)
		si.SetUpNetworks();

	// set up event
	weak_ptr<Event> event = si.CreateEvent<DerivedEvent>(node[0], node[numNodes - 1],
			forward<Args>(args)...);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(1.0);
	event.lock()->SetStartTime(1.0);
	si.Run(Event::GetRecommendedDuration());

	successful = event.lock()->IsSuccessful();
}

/**
 * Disconnected nodes, event fails but program does not crash!
 */
template<class DerivedEvent, class ...Args>
void EventFixture_P2P::Run03(Args&&... args) {
	NodePtr n1 = make_shared<simul::Node>();
	NodePtr n2 = make_shared<simul::Node>();
	EdgePtr e1 = make_shared_base_in_simul(Edge, P2PEdge, n1, n2);
	NodePtr n3 = make_shared<simul::Node>();
	NodePtr n4 = make_shared<simul::Node>();
	EdgePtr e2 = make_shared_base_in_simul(Edge, P2PEdge, n3, n4);

	Simulation si;
	si.AddEdge(e1);
	si.AddEdge(e2);
	si.SetUpNetworks();

	weak_ptr<Event> event = si.CreateEvent<DerivedEvent>(n1, n4, forward<Args>(args)...);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(1.0);
	event.lock()->SetStartTime(1.0);
	si.Run(Event::GetRecommendedDuration());

	successful = event.lock()->IsSuccessful();
}

/************************************
 *									*
 *				Wifi				*
 *									*
 ************************************/

/**
 * Wifi connections only
 * Select randomly 2 nodes to set up event
 *
 * ========Illustration=======
 *		     *ap*  *ap*
 *		 *      \  /     *
 *		*ap*----*ap*----*ap*
 *		 *      /  \     *
 *		     *ap*  *ap*
 * ===========================
 *
 */
template<class DerivedEvent, class ...Args>
void EventFixture_Wifi::Run01(int numNodes, int numApNodes, Args&&... args) {
	// important checkpoint
	assert(numNodes > 1);
	assert(numApNodes > 0);
	assert(numNodes >= numApNodes);

	int numStaNodes = numNodes - numApNodes;
	NodePtr staNodes[numStaNodes];
	NodePtr apNodes[numApNodes];

	for (int i = 0; i < numStaNodes; i++)
		staNodes[i] = make_shared<simul::Node>();
	for (int i = 0; i < numApNodes; i++)
		apNodes[i] = make_shared<simul::Node>();

	EdgePtr edge;
	Simulation si;

	// create a number of wifi networks based on number of access points
	for (int i = 0; i < numApNodes; i++)
		for (int j = 0; j < numStaNodes; j++) {
			edge = make_shared<WifiEdge>(apNodes[i], staNodes[j]);
			si.AddEdge(edge);
		}

	// also let those access points connect together
	for (int i = 1; i < numApNodes; i++) {
		edge = make_shared<WifiEdge>(apNodes[0], apNodes[i]);
		si.AddEdge(edge);
	}

	for (int i = 0; i < 10; i++)
		si.SetUpNetworks();

	// set up event
	NodePtr allNodes[numNodes];
	for (int i = 0; i < numStaNodes; i++)
		allNodes[i] = staNodes[i];
	for (int i = 0; i < numApNodes; i++)
		allNodes[numStaNodes + i] = apNodes[i];

	// random generator
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, numNodes - 1);

	int index1 = distribution(generator);
	int index2 = distribution(generator);

	while (index1 == index2)
		index2 = distribution(generator);

	// event 1
	weak_ptr<Event> event = si.CreateEvent<DerivedEvent>(allNodes[index1], allNodes[index2],
			forward<Args>(args)...);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(1.0);
		event.lock()->SetStartTime(1.0);
	si.Run(Event::GetRecommendedDuration());

	successful = event.lock()->IsSuccessful();
}

/**
 * Wifi connections only
 * Select randomly 2 nodes to set up event
 *
 * ========Illustration=======
 *		    -ap  ap-
 *		      \  /
 *		-ap----ap----ap-
 *		      /  \
 *		    -ap  ap-
 * ===========================
 */
template<class DerivedEvent, class ...Args>
void EventFixture_Wifi::Run02(int numApNodes, Args&&... args) {
	// important checkpoint
	assert(numApNodes > 1);

	int numStaNodes = numApNodes;
	NodePtr staNodes[numStaNodes];
	NodePtr apNodes[numApNodes];

	for (int i = 0; i < numStaNodes; i++)
		staNodes[i] = make_shared<simul::Node>();
	for (int i = 0; i < numApNodes; i++)
		apNodes[i] = make_shared<simul::Node>();

	EdgePtr edge;
	Simulation si;

	// create a number of wifi networks based on number of access points
	for (int i = 0; i < numApNodes; i++) {
		edge = make_shared<WifiEdge>(apNodes[i], staNodes[i]);
		si.AddEdge(edge);
	}

	// let all access points connect to a station nodes "by wifi"
	for (int i = 0; i < numApNodes; i++) {
		edge = make_shared<WifiEdge>(staNodes[0], apNodes[i]);
		si.AddEdge(edge);
	}

	for (int i = 0; i < 10; i++)
		si.SetUpNetworks();

	// set up event
	int numNodes = numStaNodes + numApNodes;
	NodePtr allNodes[numNodes];

	for (int i = 0; i < numStaNodes; i++)
		allNodes[i] = staNodes[i];
	for (int i = 0; i < numApNodes; i++)
		allNodes[numStaNodes + i] = apNodes[i];

	// random generator
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, numNodes - 1);

	int index1 = distribution(generator);
	int index2 = distribution(generator);

	while (index1 == index2)
		index2 = distribution(generator);

	// event 1
	weak_ptr<Event> event = si.CreateEvent<DerivedEvent>(allNodes[index1], allNodes[index2],
			forward<Args>(args)...);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(1.0);
		event.lock()->SetStartTime(1.0);
	si.Run(Event::GetRecommendedDuration());

	successful = event.lock()->IsSuccessful();
}

/************************************
 *									*
 *				Mixed				*
 *									*
 ************************************/

/**
 * Mixed net of P2P and Wifi
 * Random event
 *
 * ========Illustration=======
 *	n  n  n  n         n  n
 *	\  /  \  /         \  /
 *	 ap----ap----...----ap
 *	/  \  /  \         /  \
 *	n  n  n  n         n  n
 * ===========================
 *
 */
template<class DerivedEvent, class ...Args>
void EventFixture_Mixed::Run(int numWifiNets, Args&&... args) {
	// important checkpoint
	assert(numWifiNets > 1);

	NodePtr apNodes[numWifiNets];
	NodePtr staNodes[numWifiNets * 2];

	for (int i = 0; i < numWifiNets; i++) {
		apNodes[i] = make_shared<simul::Node>();
		staNodes[2 * i] = make_shared<simul::Node>();
		staNodes[2 * i + 1] = make_shared<simul::Node>();
	}

	EdgePtr edge;
	Simulation si;

	// create a number of wifi networks
	for (int i = 0; i < numWifiNets; i++) {
		edge = make_shared<WifiEdge>(apNodes[i], staNodes[2 * i]);
		si.AddEdge(edge);

		edge = make_shared<WifiEdge>(apNodes[i], staNodes[2 * i + 1]);
		si.AddEdge(edge);

		// connect access points together through P2P
		if (i < numWifiNets - 1) {
			edge = make_shared<P2PEdge>(apNodes[i], apNodes[i + 1]);
			si.AddEdge(edge);
		}
	}

	for (int i = 0; i < 10; i++)
		si.SetUpNetworks();

	// random generator
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<int> distribution(0, numWifiNets * 2 - 1);

	int index1 = distribution(generator);
	int index2 = distribution(generator);

	while (index1 == index2)
		index2 = distribution(generator);

	// event 1
	weak_ptr<Event> event = si.CreateEvent<DerivedEvent>(staNodes[index1], staNodes[index2],
			forward<Args>(args)...);
	//	event.lock()->SetUp();
	//	event.lock()->Schedule(1.0);
		event.lock()->SetStartTime(1.0);
	si.Run(Event::GetRecommendedDuration());

	successful = event.lock()->IsSuccessful();
}

#endif /* TEST_SIMULATION_EVENTS_EVENTFIXTURES_H_ */
