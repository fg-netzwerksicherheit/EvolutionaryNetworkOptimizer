/*
 * LinkDelayEvaluator.cpp
 *
 *  Created on: May 15, 2017
 *      Author: quang
 */

#include "LinkDelayEvaluator.h"

EA_TYPEINFO_CUSTOM_IMPL(LinkDelayEvaluator){
return *ea::TypeInfo("LinkDelayEvaluator")
.SetConstructor<LinkDelayEvaluator, std::string, std::string, double>("graph-file","event-file","budget");
}

using namespace simul;

LinkDelayEvaluator::LinkDelayEvaluator(std::string graphFile, std::string eventFile,
		double p_budget) :
		edgeList(P2PNetMLInput().ReadGraph(graphFile)), budget(p_budget) {

	si = std::make_shared<Simulation>();
	si->AddEdges(edgeList);
	si->SetUpNetworks();

	vector<weak_ptr<Event>> eventList = EventInput().ReadEvent(eventFile, si);

	totalExpectedData = 0;
	for (auto event : eventList)
		totalExpectedData += (event.lock()->GetTotalDataSize() + 50);
	// plus spare size for headers
}

LinkDelayEvaluator::~LinkDelayEvaluator() {
}

using namespace ea;

double LinkDelayEvaluator::DoScalarEvaluate(const GenomePtr& pGenome) {
	std::vector<double> genes = std::dynamic_pointer_cast<DoubleArrayGenome>(pGenome)->GetGenes();

	uint counter = 0;
	for (auto it = edgeList->begin(); it != edgeList->end(); it++) {
		P2PEdgePtr edge = std::dynamic_pointer_cast<P2PEdge>(*it);
		edge->SetDelay(std::to_string(genes[counter++]) + "s");
		edge->UpdateConfiguration();
	}

	Ipv4PacketInfoOutput out;
	out.EnableCbLocalDeliver();

	//XXX: memory grows over time if tcp-app and udp-app are used
	si->Run();

	out.SetListType(simul::DELIVERED_LIST);
	std::shared_ptr<std::vector<PacketInfoPtr>> deliveredList = out.GetPacketInfoList();

	double totalLatency = 0;
	double dataDelivered = 0;
	for (auto pack : *deliveredList) {
		totalLatency += (pack->GetDeliveredTime() - pack->GetSentTime()).GetSeconds();
		dataDelivered += pack->GetSize();
	}
	double fitness = totalExpectedData - dataDelivered;
	assert(fitness > 0);	// please check headers' size in totalExpectedData

	double cost = 0;
	for (double gene : genes)
		if (gene < DEFAULT_LINK_DELAY)
			cost += (DEFAULT_LINK_DELAY - gene);

	if (cost > budget)
		fitness *= (cost - budget + 1.0);

	fitness += totalLatency / totalExpectedData;

	return fitness;
}

