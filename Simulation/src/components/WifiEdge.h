/*
 * WifiEdge.h
 *
 *  Created on: Mar 24, 2017
 *      Author: Kiet Vo
 */

#ifndef COMPONENTS_WIFIEDGE_H_
#define COMPONENTS_WIFIEDGE_H_

#include "Edge.h"

namespace simul {

/**
 * \ingroup components
 */
class WifiEdge: public Edge {
private:
	void DoNothing();

public:
	WifiEdge(NodePtr sourceNode, NodePtr destNode);
	virtual ~WifiEdge();
};

} /* namespace simul */

#endif /* COMPONENTS_WIFIEDGE_H_ */
