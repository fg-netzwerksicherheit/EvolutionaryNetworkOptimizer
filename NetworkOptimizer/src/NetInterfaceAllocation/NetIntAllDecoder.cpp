/*
 * NetIntAllDecoder.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: Kiet Vo
 */

#include "NetIntAllDecoder.h"

using namespace std;

NetIntAllDecoder::NetIntAllDecoder(string graphFile) :
		NetIntAllDecoder(anygraph::P2PNetMLHandler().ReadGraph(graphFile)) {
}

NetIntAllDecoder::NetIntAllDecoder(std::shared_ptr<anygraph::Topology> p_topo) {
	topo = p_topo;

	//
	// these 2 lists are vital for defining a genotype (interface) array in "specific order"
	//
	vector<string> computerList;
	vector<vector<anygraph::AnyNode>> interfacesInComputer;

	for (auto device : topo->get_node()) {	// iterate the network-interface list in topology
		for (auto attribute : device.get_attribute_list()) {	// iterate the attribute list
			if (attribute.get_attribute_name() == "Computer") {	// find computer attribute

				bool foundComputer = false;
				string computerID = attribute.get_attribute_data();

				// existing computer?
				for (uint compIndex = 0; compIndex < computerList.size(); compIndex++) {
					if (computerID == computerList[compIndex]) {
						interfacesInComputer[compIndex].push_back(device);
						foundComputer = true;
						break;
					}
				}

				// newly discovered computer?
				if (foundComputer == false) {
					computerList.push_back(computerID);

					vector<anygraph::AnyNode> localInterfaceList;
					localInterfaceList.push_back(device);
					interfacesInComputer.push_back(localInterfaceList);
				}
				break;	// to the next network interface (or device)
			}
		}
	}

	//
	// store network interface's properties
	//
	interfaceList.clear();
	for (uint i = 0; i < interfacesInComputer.size(); i++)
		for (uint j = 0; j < interfacesInComputer[i].size(); j++)
			interfaceList.push_back(interfacesInComputer[i][j]);

	//
	// calculate the number of valid network interfaces (must reside in a computer)
	//
	int numTotalInterfaces = 0;
	for (vector<anygraph::AnyNode> localInterfaces : interfacesInComputer)
		numTotalInterfaces += localInterfaces.size();

	//TODO: remove after finishing test
	assert(static_cast<long>(interfaceList.size()) == numTotalInterfaces);

	//
	// compute possible values for each gene
	//
	possibleValues = vector<vector<int>>(numTotalInterfaces);

	for (int deviceIndex = 0; deviceIndex < numTotalInterfaces; deviceIndex++) {

		// calculate lower and upper of invalid range
		int lowerInvalid = 0, upperInvalid = -1;
		for (auto localInterfaces : interfacesInComputer) {
			uint numLocalInterfaces = localInterfaces.size();

			lowerInvalid = upperInvalid + 1;
			upperInvalid = lowerInvalid + numLocalInterfaces - 1;

			if (deviceIndex <= upperInvalid)
				break;
		}

		//TODO: remove after finishing test
		assert(lowerInvalid <= deviceIndex && deviceIndex <= upperInvalid);

		// generate possible values
		for (int i = 0; i < lowerInvalid; i++)
			possibleValues[deviceIndex].push_back(i);
		for (int i = upperInvalid + 1; i < numTotalInterfaces; i++)
			possibleValues[deviceIndex].push_back(i);
	}
}

NetIntAllDecoder::~NetIntAllDecoder() {
}

// interfaces with no "computer" attributes are ignored
size_t NetIntAllDecoder::GetNumInterfaces() {
	return possibleValues.size();
}

vector<vector<int>> NetIntAllDecoder::GetPossibleValues() {
	return possibleValues;
}

uint NetIntAllDecoder::GetNumViolatedInterfaces(std::vector<int> genotype) {
	uint numViolation = 0;

	for (uint geneIndex = 0; geneIndex < genotype.size(); geneIndex++) {
		if (genotype[geneIndex] < 0)
			continue;

		bool assumedViolated = true;
		for (int possibleValue : possibleValues[geneIndex]) {
			if (genotype[geneIndex] != possibleValue)	// is this connection in valid range?
				continue;

			//XXX: casting is only safe due to no-connection check earlier
			uint matchedIndex = static_cast<uint>(genotype[geneIndex]);

			if (genotype[matchedIndex] == static_cast<long>(geneIndex))
				assumedViolated = false;

			break;
		}

		if (assumedViolated)
			numViolation++;
	}

	return numViolation;
}

using namespace simul;

double NetIntAllDecoder::ComputeCost(vector<int> genotype) {
	TopologyPtr decodedTopo = DecodeImpl(genotype);

	unordered_map<string, double> costMap; // interface id -> cost
	for (auto interface : interfaceList)
		for (auto att : interface.get_attribute_list())
			if (att.get_attribute_name() == "cost") {
				costMap[interface.get_ID()] = stod(att.get_attribute_data());
				break;
			}

	double totalCost = 0;
	for (auto edge : decodedTopo->get_edge()) {
		totalCost += costMap[edge.get_From()];
		totalCost += costMap[edge.get_To()];
	}

	return totalCost;
}

SetPtr<EdgePtr> NetIntAllDecoder::Decode(vector<int> genotype) {

	return GraphMLInput().ParseGraph(DecodeImpl(genotype));
}

//
// private
//
TopologyPtr NetIntAllDecoder::DecodeImpl(vector<int> genotype) {
	// checking block
	if (genotype.size() != possibleValues.size())
		throw invalid_argument("[NetIntAllDecoder] Wrong genotype's size!");

	// create initial topology
	TopologyPtr decodedTopo = make_shared<anygraph::Topology>();
	for (auto att : topo->get_attributes())
		decodedTopo->add_attribute(att);
	for (auto an : topo->get_node())
		decodedTopo->add_node(an);

	// find valid edges to decode
	uint edgeCount = 0;
	vector<bool> alreadyChecked(genotype.size(), false);	// for each network interface

	for (uint geneIndex = 0; geneIndex < genotype.size(); geneIndex++) {
		if (alreadyChecked[geneIndex])
			continue;
		else
			alreadyChecked[geneIndex] = true;

		if (genotype[geneIndex] < 0)	// no connection
			continue;

		for (int possibleValue : possibleValues[geneIndex]) {
			if (genotype[geneIndex] != possibleValue)	// is this connection in valid range?
				continue;

			//XXX: casting is only safe due to no-connection check earlier
			uint matchedIndex = static_cast<uint>(genotype[geneIndex]);

			if (alreadyChecked[matchedIndex])
				break;

			// mismatched connection
			if (genotype[matchedIndex] != static_cast<long>(geneIndex))
				break;

			alreadyChecked[matchedIndex] = true;

			// decoding
			anygraph::AnyEdge ae;
			ae.set_ID("e" + to_string(edgeCount++));
			ae.set_from(interfaceList[geneIndex].get_ID());
			ae.set_to(interfaceList[matchedIndex].get_ID());
			decodedTopo->add_edge(ae);

			break;
		}
	}

	return decodedTopo;
}

