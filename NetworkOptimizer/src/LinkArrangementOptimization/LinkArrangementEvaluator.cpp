/*
 * LinkArrangementEvaluator.cpp
 *
 *  Created on: May 26, 2017
 *      Author: Kiet Vo
 */

#include "LinkArrangementEvaluator.h"

EA_TYPEINFO_CUSTOM_IMPL(LinkArrangementEvaluator){
return *ea::TypeInfo("LinkArrangementEvaluator")
.SetConstructor<LinkArrangementEvaluator, std::string, double>("event-file", "budget")->SetConstructor<LinkArrangementEvaluator, uint, double>("numComputers", "budget");
}

LinkArrangementEvaluator::LinkArrangementEvaluator(std::string p_eventFile, double p_budget) :
		eventFile(p_eventFile), budget(p_budget) {
}

LinkArrangementEvaluator::LinkArrangementEvaluator(uint p_numComputers, double p_budget) :
		numComputers(p_numComputers), budget(p_budget) {
}

LinkArrangementEvaluator::~LinkArrangementEvaluator() {
}

using namespace std;
using namespace ea;

static void CreateEvents(simul::SimulationPtr si, uint numComputers);

FitnessPtr LinkArrangementEvaluator::DoEvaluate(const GenomePtr& pGenome) {

	IntArrayGenomePtr genome = dynamic_pointer_cast<IntArrayGenome>(pGenome);

	simul::SetPtr<simul::EdgePtr> edgeList = decoder.DecodeToEdges(genome->GetGenes());

	simul::SimulationPtr si = make_shared<simul::Simulation>();
	si->AddEdges(edgeList);
	si->SetUpNetworks();

	if (numComputers <= 0)
		vector<weak_ptr<simul::Event>> eventList = simul::EventInput().ReadEvent(eventFile, si);
	else
		CreateEvents(si, numComputers);

	simul::Ipv4PacketInfoOutput out;
	out.EnableCbLocalDeliver();

	si->Run();

	out.SetListType(simul::DELIVERED_LIST);
	simul::VectorPtr<simul::PacketInfoPtr> delivered = out.GetPacketInfoList();

	double dataDelivered = 0;
	double sumLatencyxBytes = 0;
	for (auto pack : *delivered) {
		double size = static_cast<double>(pack->GetSize());
		dataDelivered += size;
		sumLatencyxBytes += (pack->GetDeliveredTime() - pack->GetSentTime()).GetSeconds() * size;
	}

	double cost = decoder.ComputeCost(genome->GetGenes());
	double latency = sumLatencyxBytes / dataDelivered;

	shared_ptr<LinkArrangementFitness> fitness = make_shared<LinkArrangementFitness>(budget, cost,
			dataDelivered, latency);

	return fitness;
}

using namespace simul;
static void CreateEvents(SimulationPtr si, uint numComputers) {
	for (uint i = 0; i < numComputers; i++) {
		string srcID = "n" + to_string(i);
		string dstID = "n";
		if (i == numComputers - 1)
			dstID = dstID + to_string(0);
		else
			dstID = dstID + to_string(i + 1);

		NodePtr src = si->GetNode(srcID);
		if (src == nullptr)
			src = make_shared<Node>(srcID);

		NodePtr dst = si->GetNode(dstID);
		if (dst == nullptr)
			dst = make_shared<Node>(dstID);

		weak_ptr<Event> event = si->CreateEvent<UdpTestEvent>(src, dst);
		event.lock()->SetStartTime(1);
	}
}

