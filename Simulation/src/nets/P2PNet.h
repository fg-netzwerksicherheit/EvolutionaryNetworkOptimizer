/*
 * P2PNet.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef NETS_P2PNET_H_
#define NETS_P2PNET_H_

#include "../nets/Network.h"

namespace simul {

/**
 * \ingroup nets
 */
class P2PNet: public Network {
	friend class Simulation;

private:
	static int p2pNetCount;
	int localNetID;                        // to calculate ip address
	ns3::NetDeviceContainer devices;

protected:
	ns3::NodeContainer p2pNodes;

	P2PNet();

	virtual void Configure() override;
	virtual ns3::NetDeviceContainer GetAllNetDevices() override;
	static void ResetNetCount();

public:
	virtual ~P2PNet();
};

} /* namespace simul */

#endif /* NETS_P2PNET_H_ */
