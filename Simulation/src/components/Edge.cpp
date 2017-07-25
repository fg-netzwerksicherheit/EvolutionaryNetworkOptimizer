/*
 * Edge.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#include "Edge.h"

#include "../exceptions/NullPointerException.h"
#include "../exceptions/UsageException.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::Edge");

Edge::Edge(NodePtr sourceNode, NodePtr destNode) {

	if (sourceNode == NULL || destNode == NULL) {
		string msg = "A Node is NULL!";
		NS_LOG_ERROR(msg);
		throw NullPointerException(msg);
	}
	if (sourceNode == destNode) {
		string msg = "Cannot connect a node to \"itself\"!";
		NS_LOG_ERROR(msg);
		throw invalid_argument(msg);
	}
	this->sourceNode = sourceNode;
	this->destNode = destNode;
}

Edge::~Edge() {
}

NodePtr Edge::GetSourceNode() const {
	return sourceNode;
}

NodePtr Edge::GetDestNode() const {
	return destNode;
}

} /* namespace simul */
