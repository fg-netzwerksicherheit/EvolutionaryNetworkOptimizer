/*
 * NetIntAllDecoder.h
 *
 *  Created on: Jun 14, 2017
 *      Author: Kiet Vo
 */

#ifndef NETINTERFACEALLOCATION_NETINTALLDECODER_H_
#define NETINTERFACEALLOCATION_NETINTALLDECODER_H_

#include "Simul.h"

class NetIntAllDecoder {
public:
	NetIntAllDecoder(std::string graphFile);
	NetIntAllDecoder(std::shared_ptr<anygraph::Topology> topo);
	virtual ~NetIntAllDecoder();

	// graph-file dependent
	// only process nodes (interfaces), ignore edges
	size_t GetNumInterfaces();
	std::vector<std::vector<int>> GetPossibleValues();

	// genotype dependent
	uint GetNumViolatedInterfaces(std::vector<int> genotype);
	double ComputeCost(std::vector<int> genotype);
	simul::SetPtr<simul::EdgePtr> Decode(std::vector<int> genotype);

private:
	std::shared_ptr<anygraph::Topology> topo;

	// vectors have size = number of interfaces = genome's size
	std::vector<anygraph::AnyNode> interfaceList;
	std::vector<std::vector<int>> possibleValues;	// for each gene

	std::shared_ptr<anygraph::Topology> DecodeImpl(std::vector<int> genotype);
};

#endif /* NETINTERFACEALLOCATION_NETINTALLDECODER_H_ */
