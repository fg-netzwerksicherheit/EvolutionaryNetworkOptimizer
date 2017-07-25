/*
 * WifiEdge.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: Kiet Vo
 */

#include "WifiEdge.h"

namespace simul {

//NS_LOG_COMPONENT_DEFINE("Simul::WifiEdge");

/**
 * \param sourceNode access point
 * \param destNode station node
 */
WifiEdge::WifiEdge(NodePtr sourceNode, NodePtr destNode) :
		Edge(sourceNode, destNode) {
}

WifiEdge::~WifiEdge() {
}

void WifiEdge::DoNothing() {

}

} /* namespace simul */
