/*
 * NetworkEvaluator.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham, Kiet Vo
 */

#include "DataRateScalarEvaluator.h"

using namespace std;
using namespace simul;

EA_TYPEINFO_CUSTOM_IMPL(DataRateScalarEvaluator){
return *ea::TypeInfo("DataRateScalarEvaluator")
.SetConstructor<DataRateScalarEvaluator, string, string, double>("graph-file","event-file","budget");
}

DataRateScalarEvaluator::DataRateScalarEvaluator(string graphFile, string eventFile,
		double p_budget) :
		edgeList(P2PNetMLInput().ReadGraph(graphFile)), budget(p_budget) {
	si = make_shared<Simulation>();
	si->AddEdges(edgeList);
	si->SetUpNetworks();

	EventInput ei;
	ei.ReadEvent(eventFile, si);
}

DataRateScalarEvaluator::~DataRateScalarEvaluator() {

}

/**
 *
 */
double DataRateScalarEvaluator::DoScalarEvaluate(const GenomePtr& pGenome) {
	shared_ptr<DataRateGenome> network = dynamic_pointer_cast<DataRateGenome>(pGenome);

	uint counter = 0;
	double totalDataRate = 0;
	for (auto it = edgeList->begin(); it != edgeList->end(); it++) {
		P2PEdgePtr edge = dynamic_pointer_cast<P2PEdge>(*it);

		double threshold = 1.0 / (1000 * 1000);	// 1 bit per second
		assert(0 < threshold);	//TODO: remove

		double rate1 = (network->getDataRateSrc())[counter];
		if (rate1 <= threshold)
			rate1 = threshold * 2;

		double rate2 = (network->getDataRateDst())[counter];
		if (rate2 <= threshold)
			rate2 = threshold * 2;

		//TODO: remove
		assert(threshold < rate1 && threshold < rate2);

		totalDataRate += (rate1 + rate2);

		edge->SetDataRateSrc(to_string(static_cast<uint64_t>(rate1 * 1000 * 1000)) + "bps");
		edge->SetDataRateDst(to_string(static_cast<uint64_t>(rate2 * 1000 * 1000)) + "bps");

		edge->UpdateConfiguration();
		counter++;
	}

	Ipv4PacketInfoOutput out;
	out.EnableCbLocalDeliver();

	//XXX: memory grows over time if tcp-app and udp-app are used
	si->Run();

	out.SetListType(simul::DELIVERED_LIST);
	shared_ptr<vector<PacketInfoPtr>> deliveredList = out.GetPacketInfoList();

	double dataDelivered = 0;
	for (auto pack : *deliveredList)
		dataDelivered += pack->GetSize();

	// calculate fitness
	double fitness = dataDelivered;
	double cost = totalDataRate;	// cost
	if (cost > budget)
		fitness /= (cost - budget + 1.0);	// penalty
	else
		fitness += 1.0 / (cost * 1000 * 1000);				// incentive

	return fitness;
}

