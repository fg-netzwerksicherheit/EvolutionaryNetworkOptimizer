/*
 * Network.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef NETS_NETWORK_H_
#define NETS_NETWORK_H_

#include "../Si_Type.h"

namespace simul {

/**
 * \defgroup nets Networks
 */

/**
 * \ingroup nets
 */
class Network {
	friend class Simulation;

protected:
	SetPtr<EdgePtr> edgeList;
	SetPtr<NodePtr> nodeList;

	Network();
	void AddEdge(EdgePtr edge);		// bulk adding is not allowed

	virtual void Configure() = 0;
	virtual ns3::NetDeviceContainer GetAllNetDevices() = 0;

public:
	virtual ~Network();
};

}
/* namespace simul */

#endif /* NETS_NETWORK_H_ */
