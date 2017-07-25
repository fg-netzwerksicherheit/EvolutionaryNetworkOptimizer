/*
 * Simulation.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#include "Simulation.h"
#include "events/Event.h"

#include "../components/Node.h"
#include "../components/Edge.h"
#include "../components/P2PEdge.h"
#include "../components/WifiEdge.h"

#include "../nets/Network.h"
#include "../nets/P2PNet.h"
#include "../nets/WifiNet.h"

#include "../exceptions/NullPointerException.h"
#include "../exceptions/UsageException.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::Simulation");

static shared_ptr<vector<string>> logComponentNames = nullptr;

static shared_ptr<vector<string>> GetLogComponentNames() {
	if (logComponentNames == nullptr) {
		logComponentNames = make_shared<vector<string>>();

		logComponentNames->push_back("simul::Edge");

		logComponentNames->push_back("simul::GraphMLInput");
		logComponentNames->push_back("simul::P2PNetMLInput");

		logComponentNames->push_back("simul::Network");
		logComponentNames->push_back("simul::P2PNet");
		logComponentNames->push_back("simul::WifiNet");

		logComponentNames->push_back("simul::Ipv4PacketInfoOutput");

		logComponentNames->push_back("simul::Simulation");

		logComponentNames->push_back("simul::OnOffApp");
		logComponentNames->push_back("simul::Event");
		logComponentNames->push_back("simul::TcpAppEvent");
		logComponentNames->push_back("simul::UdpAppEvent");
		logComponentNames->push_back("simul::UdpTestEvent");
	}
	return logComponentNames;
}

/**
 * Enable all log components of this Automated Simulations with ns3::LOG_LEVEL_ALL and ns3::LOG_PREFIX_ALL.
 *
 * \see ns3::LogLevel
 */
void Simulation::EnableAllLogComponents() {
	EnableAllLogComponents(ns3::LogLevel(ns3::LOG_LEVEL_ALL | ns3::LOG_PREFIX_ALL));
}

/**
 * Enable all log components of this Automated Simulations.
 *
 * \see ns3::LogLevel
 */
void Simulation::EnableAllLogComponents(enum ns3::LogLevel level) {
	for (string componentName : *GetLogComponentNames())
		ns3::LogComponentEnable(componentName.c_str(), level);
}

void Simulation::DisableAllLogComponents() {
	DisableAllLogComponents(ns3::LogLevel(ns3::LOG_LEVEL_ALL | ns3::LOG_PREFIX_ALL));
}

void Simulation::DisableAllLogComponents(enum ns3::LogLevel level) {
	for (string componentName : *GetLogComponentNames())
		ns3::LogComponentDisable(componentName.c_str(), level);
}

Simulation::Simulation() {
}

Simulation::~Simulation() {
	SetValidToRunBit(false);
}

void Simulation::AddEdge(EdgePtr edge) {
	if (edge == NULL) {
		string msg = "This edge is NULL!";
		NS_LOG_ERROR(msg);
		throw NullPointerException(msg);
	}

	globalEdgeList.insert(edge);
	parsedNodeList.insert(edge->GetSourceNode());
	parsedNodeList.insert(edge->GetDestNode());
}

void Simulation::AddEdges(SetPtr<EdgePtr> edges) {

	if (edges == NULL) {
		string msg = "This set is NULL!";
		NS_LOG_ERROR(msg);
		throw NullPointerException(msg);
	}

	for (auto it = edges->begin(); it != edges->end(); it++)
		this->AddEdge(*it);
}

/**
 * \brief Find a node from all the added edges.
 *
 * \param ID the id of the node to find
 *
 * \return a pointer to the found node
 * \return null pointer if not found
 *
 * \see simul::Node
 */
NodePtr Simulation::GetNode(string ID) {
	for (auto it = parsedNodeList.begin(); it != parsedNodeList.end(); it++)
		if ((*it)->GetID() == ID)
			return *it;

	NS_LOG_DEBUG("No Node with ID " << ID << " is found.");
	return NULL;
}

ns3::NetDeviceContainer Simulation::FindDevicesFromNode(NodePtr node) {
	ns3::NetDeviceContainer devicesOfNode;
	for (unsigned int i = 0; i < globalDevices.GetN(); i++)
		if (globalDevices.Get(i)->GetNode() == node->GetNs3Node())
			devicesOfNode.Add(globalDevices.Get(i));
	return devicesOfNode;
}

/**
 * \brief A configuration method for ns-3's simulator.
 *
 * It processes all the added edges in order to construct appropriate networks.
 * However, before doing necessary network construction and configuration, it first must
 * flush the previous configuration by destroying the current ns-3's simulator.
 * Therefore, all other simulation objects will become invalid and thus it complies with the rule "only one at a time".
 * Moreover, any class that uses the ns-3 simulator's configuration may become invalid and
 * need to re-set up as well.
 *
 * \attention Using ns3-logging-statement output directly above this method may cause
 * the simulator unable to reset, for example:
 *
 * \code
 * Simulation si;
 *
 * NS_LOG_DEBUG("Attempt to reset the simulator");
 * si.SetUpNetworks();
 * \endcode
 *
 * \see Ipv4PacketInfoOutput
 */
void Simulation::SetUpNetworks() {
// flush previous Configuration
	Flush();

// start configuration
	networkList = ConstructNetwork(globalEdgeList);

	for (auto it = networkList.begin(); it != networkList.end(); it++) {
		(*it)->Configure();
		globalDevices.Add((*it)->GetAllNetDevices());
	}

// set valid bit after configuration
	SetValidToRunBit(true);
}

/**
 * \brief Run with a specified duration.
 *
 * This one uses the same implementation as Run().
 *
 * \param duration in seconds
 *
 * \throw UsageException if this object has not been configured.
 *
 * \see SetUpNetworks()
 * \see Event::GetRecommendedDuration()
 */
void Simulation::Run(double duration) {
	if (IsValidToRun() == false) {
		string msg = "Not yet configured!";
		NS_LOG_ERROR(msg);
		throw UsageException(msg);
	}

	if (duration >= 0)
		ns3::Simulator::Stop(ns3::Seconds(duration));

	for (EventPtr event : eventList)
		event->SetUp();

	ns3::Simulator::Run();
}

/**
 * \brief Run all events associated with this simulation objects.
 *
 * Depending on specific cases, a simulation may run forever as one sub-event generates another.
 * This method simply invokes Run(double) with infinite duration.
 *
 * \see Run(double)
 */
void Simulation::Run() {
	Run(-51);
}

//
// private
//

//
// This should only be called before SetUpNetworks()
// because Simulator::Destroy() cannot be invoked recklessly
//
void Simulation::Flush() {

// remove all networks
	P2PNet::ResetNetCount();
	WifiNet::ResetNetCount();

	networkList.clear();

	globalDevices = ns3::NetDeviceContainer();

// clean nodes
	for (auto node = parsedNodeList.begin(); node != parsedNodeList.end(); node++)
		(*node)->NotifySimulatorDestroyed();

// clean events
	for (auto event : eventList)
		event->NotifySimulatorDestroyed();

	ns3::Simulator::Destroy();
}

vector<NetworkPtr> Simulation::ConstructNetwork(set<EdgePtr> edgeList) {
	vector<NetworkPtr> networkList;

	while (edgeList.size() > 0) {
		EdgePtr edge = *edgeList.begin();

		if (dynamic_pointer_cast<P2PEdge>(edge))
			networkList.push_back(ConstructP2P(edge, edgeList));

		else if (dynamic_pointer_cast<WifiEdge>(edge))
			networkList.push_back(ConstructWifi(edge, edgeList));
	}

	return networkList;
}

NetworkPtr Simulation::ConstructP2P(EdgePtr initialEdge, set<EdgePtr>& edgeList) {
	NetworkPtr p2pNet = shared_ptr<Network>(new P2PNet());
	p2pNet->AddEdge(initialEdge);
	edgeList.erase(initialEdge);

	return p2pNet;
}

NetworkPtr Simulation::ConstructWifi(EdgePtr initialEdge, set<EdgePtr>& edgeList) {
	NetworkPtr wifiNet = shared_ptr<Network>(new WifiNet());

	NodePtr apNode = initialEdge->GetSourceNode();	// get access point of wifi network

// add wifi edges to wifi network
	set<EdgePtr> wifiEdges;

	for (auto it = edgeList.begin(); it != edgeList.end(); it++)
		if ((*it)->GetSourceNode() == apNode && dynamic_pointer_cast<WifiEdge>(*it))
			wifiEdges.insert(*it);

	for (auto it = wifiEdges.begin(); it != wifiEdges.end(); it++)
		wifiNet->AddEdge(*it);

// remove added wifi edges from the edge list
	for (auto it = wifiEdges.begin(); it != wifiEdges.end(); it++)
		edgeList.erase(*it);

	return wifiNet;
}

Simulation * Simulation::configuredObjectID = nullptr;

void Simulation::SetValidToRunBit(bool valid) {
	if (valid)
		configuredObjectID = this;

	else if (configuredObjectID == this)
		configuredObjectID = nullptr;
}

bool Simulation::IsValidToRun() {
	if (this == configuredObjectID)
		return true;
	else
		return false;
}

} /* namespace simul */

