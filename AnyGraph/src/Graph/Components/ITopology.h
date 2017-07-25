/*
 * ITopology.h
 *
 *  Created on: Apr 8, 2017
 *      Author: Kiet Vo
 */

#ifndef GRAPH_ITOPOLOGY_H_
#define GRAPH_ITOPOLOGY_H_

#include "AnyEdge.h"
#include "AnyNode.h"
#include <vector>

namespace anygraph {

class ITopology {
public:
	virtual ~ITopology() {
	}

	virtual std::vector<AnyNode> get_node() = 0;
	virtual std::vector<AnyEdge> get_edge() = 0;
	virtual vector<Attributes> get_attributes() = 0;
};

} /* namespace anygrapha */

#endif /* GRAPH_ITOPOLOGY_H_ */
