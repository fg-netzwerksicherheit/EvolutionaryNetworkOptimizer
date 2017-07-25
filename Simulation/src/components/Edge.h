/*
 * Edge.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef COMPONENTS_EDGE_H_
#define COMPONENTS_EDGE_H_

#ifndef DATARATE_INFINITY
#define DATARATE_INFINITY	"0"
#endif

#include "../Si_Type.h"

namespace simul {

/**
 * \defgroup components Components
 */

/**
 * \ingroup components
 */
class Edge {

private:
	NodePtr sourceNode;
	NodePtr destNode;

	virtual void DoNothing() = 0;

public:
	Edge(NodePtr sourceNode, NodePtr destNode);
	virtual ~Edge();

	NodePtr GetSourceNode() const;
	NodePtr GetDestNode() const;
};

} /* namespace simul */

#endif /* COMPONENTS_EDGE_H_ */
