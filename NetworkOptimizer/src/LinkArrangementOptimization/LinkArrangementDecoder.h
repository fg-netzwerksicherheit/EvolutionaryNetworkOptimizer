/*
 * LinkArrangementDecoder.h
 *
 *  Created on: Jun 26, 2017
 *      Author: Kiet Vo
 */

#ifndef LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTDECODER_H_
#define LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTDECODER_H_

#include "Simul.h"

#define LA_DEC_CB_LENGTH	9

/**
 * \defgroup linkarrangement	Link Arrangement Optimization
 *
 * \brief Only works with p2pnetml file.
 */

/**
 * \ingroup linkarrangement
 *
 * \brief Mapping from genotype (integer array) to phenotype (edge list).
 *
 * If a network has N computers then it will be represented by an array of N(N-1)/2 elements.
 * Each element represents the connection type between a pair of computers
 * and there are N(N-1)/2 such pairs.
 *
 * The following table gives a list of possible values for the genotypes and their meanings.
 *
 \verbatim
 |	genotype	|	phenotype			|	cost
 ------------------------------------------------
 |		0		|	no connections		|	0
 |		1		|	Ethernet 10Mb/s		|	1
 |		2		|	Ethernet 100Mb/s	|	2
 |		3		|	Ethernet Gb/s		|	3
 |		4		|	Ethernet 2.5Gb/s	|	4
 |		5		|	Ethernet 5Gb/s		|	5
 |		6		|	Ethernet 10Gb/s		|	6
 |		7		|	Ethernet 40Gb/s		|	7
 |		8		|	Ethernet 100Gb/s	|	8
 \endverbatim
 */
class LinkArrangementDecoder {
public:
	LinkArrangementDecoder();
	virtual ~LinkArrangementDecoder();

	double ComputeCost(std::vector<int> genes);
	simul::SetPtr<simul::EdgePtr> DecodeToEdges(std::vector<int> genes);
	std::shared_ptr<anygraph::Topology> DecodeToTopology(std::vector<int> genes);

private:
	std::vector<std::shared_ptr<anygraph::AnyEdge>> codebook;

	std::shared_ptr<anygraph::Topology> DecodeImpl(std::vector<uint> genes);
};

#endif /* LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTDECODER_H_ */
