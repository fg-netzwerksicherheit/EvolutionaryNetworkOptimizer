/*
 * Network.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#include "../nets/Network.h"
#include "../nets/P2PNet.h"
#include "../nets/WifiNet.h"

#include "../components/Edge.h"
#include "../components/P2PEdge.h"
#include "../components/WifiEdge.h"

#include "../exceptions/NullPointerException.h"

namespace simul {

NS_LOG_COMPONENT_DEFINE("simul::Network");

Network::Network() {
	edgeList = make_shared<set<EdgePtr>>();
	nodeList = make_shared<set<NodePtr>>();
}

Network::~Network() {
}

void Network::AddEdge(EdgePtr edge) {

	if (edge == NULL) {
		string msg = "This edge is NULL!";
		NS_LOG_ERROR(msg);
		throw NullPointerException(msg);
	}

	edgeList->insert(edge);

	nodeList->insert(edge->GetSourceNode());
	nodeList->insert(edge->GetDestNode());
}

} /* namespace simul */
