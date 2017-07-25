/*
 * WifiNet.h
 *
 *  Created on: Mar 24, 2017
 *      Author: Kiet Vo
 */

#ifndef NETS_WIFINET_H_
#define NETS_WIFINET_H_

#include "../nets/Network.h"

namespace simul {

/**
 * \ingroup nets
 */
class WifiNet: public Network {
	friend class Simulation;

private:
	static int wifiNetCount;
	int localNetID;
	ns3::NetDeviceContainer devices;

protected:
	ns3::NodeContainer staNodes;
	ns3::NodeContainer apNode;

	WifiNet();

	virtual void Configure() override;
	virtual ns3::NetDeviceContainer GetAllNetDevices() override;
	static void ResetNetCount();

public:
	virtual ~WifiNet();
};

} /* namespace simul */

#endif /* NETS_WIFINET_H_ */
