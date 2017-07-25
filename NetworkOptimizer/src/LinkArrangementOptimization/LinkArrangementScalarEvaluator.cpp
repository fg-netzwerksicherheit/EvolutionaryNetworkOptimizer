/*
 * LinkArrangementScalarEvaluator.cpp
 *
 *  Created on: May 24, 2017
 *      Author: kiet
 */

#include "Simul.h"
#include "LinkArrangementScalarEvaluator.h"

EA_TYPEINFO_CUSTOM_IMPL(LinkArrangementScalarEvaluator) {
	return *ea::TypeInfo("LinkArrangementScalarEvaluator").SetConstructor<LinkArrangementScalarEvaluator, std::string, double>("event-file", "budget")->SetConstructor<LinkArrangementScalarEvaluator, uint, double>("numComputers", "budget");
}

LinkArrangementScalarEvaluator::LinkArrangementScalarEvaluator(std::string p_eventFile,
		double p_budget) :
		eventFile(p_eventFile), budget(p_budget) {
}

LinkArrangementScalarEvaluator::LinkArrangementScalarEvaluator(uint p_numComputers, double p_budget) :
		numComputers(p_numComputers), budget(p_budget) {
}

LinkArrangementScalarEvaluator::~LinkArrangementScalarEvaluator() {
}

static void CreateEvents(simul::SimulationPtr si, uint numComputers);

using namespace ea;
using namespace std;

double LinkArrangementScalarEvaluator::DoScalarEvaluate(const GenomePtr& pGenome) {

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
	out.EnableCbSendOutgoing();
	out.EnableCbLocalDeliver();

	si->Run();

	out.SetListType(simul::DELIVERED_LIST);
	simul::VectorPtr<simul::PacketInfoPtr> delivered = out.GetPacketInfoList();

	uint32_t dataDelivered = 0;
	for (auto pack : *delivered)
		dataDelivered += pack->GetSize();

	// calculate fitness
	double fitness = dataDelivered;
	double cost = decoder.ComputeCost(genome->GetGenes());
	if (cost > budget)
		fitness /= (cost - budget + 1.0);	// penalty
	else if (cost > 0)
		fitness += 1.0 / cost;				// incentive

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

