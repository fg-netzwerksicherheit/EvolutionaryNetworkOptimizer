/*
 * LinkArrangementFitness.cpp
 *
 *  Created on: May 26, 2017
 *      Author: Kiet Vo
 */

#include "LinkArrangementFitness.h"
#include <iomanip>

EA_TYPEINFO_CUSTOM_IMPL(LinkArrangementFitness) {
	return *ea::TypeInfo("LinkArrangementFitness")
	.SetConstructor<LinkArrangementFitness, double, double, double, double>("budget","cost", "data","latency")->SetConstructor<LinkArrangementFitness>();
}

LinkArrangementFitness::LinkArrangementFitness() {
}

LinkArrangementFitness::LinkArrangementFitness(double p_budget, double p_cost, double p_data,
		double p_latency) :
		budget(p_budget), cost(p_cost), data(p_data), latency(p_latency) {
}

LinkArrangementFitness::~LinkArrangementFitness() {
}

double LinkArrangementFitness::GetData() {
	return data;
}

double LinkArrangementFitness::GetCost() {
	return cost;
}

using namespace std;

ostream& LinkArrangementFitness::Print(ostream& os) const {
	os << std::setprecision(0) << std::fixed << "Data = " << data << "; Cost = " << cost;
	os << std::setprecision(6) << std::fixed << "; Average latency = " << latency;
	return os;
}

int LinkArrangementFitness::Compare(const Fitness& p_other) const {
	const LinkArrangementFitness& other = static_cast<const LinkArrangementFitness&>(p_other);

	bool error1 = budget <= 0 || cost < 0 || data < 0 || latency < 0;
	bool error2 = other.budget != budget || other.cost < 0 || other.data < 0 || other.latency < 0;

	if (error1 || error2) {
		string msg("[Link Arrangement Fitness] Exception");
		cout << msg << endl;
		throw msg;
	}

	//
	// comparison
	//
	// general order of priority: cost -> data -> latency
	// criteria:
	// * cost: lower is better
	// * data: higher is better
	// * latency: lower is better
	//
	if (cost > budget) {

		if (other.cost > other.budget) {		// both costs exceed budget
			if (data != other.data)
				return data - other.data;			// data
			else if (cost != other.cost)
				return -(cost - other.cost);		// cost
			else
				return -(latency - other.latency);	// latency
		}

		else
			return -(cost - other.cost);
	}

	else { // cost <= budget

		if (other.cost > other.budget)
			return -(cost - other.cost);

		else {										// both costs are within budget
			if (data != other.data)					// data
				return data - other.data;
			else if (latency != other.latency)		// latency
				return -(latency - other.latency);
			else
				return -(cost - other.cost);		// cost
		}
	}
}

// protected
void LinkArrangementFitness::DoSerialize(std::ostream& pStream) const {
	Write(pStream, budget);
	Write(pStream, cost);
	Write(pStream, data);
	Write(pStream, latency);
}

void LinkArrangementFitness::DoDeserialize(std::istream& pStream) {
	budget = Read<double>(pStream);
	cost = Read<double>(pStream);
	data = Read<double>(pStream);
	latency = Read<double>(pStream);
}

