/*
 * SimpleFixtures.h
 *
 *  Created on: Mar 29, 2017
 *      Author: Kiet Vo
 */

#ifndef TEST_SIMPLEFIXTURES_H_
#define TEST_SIMPLEFIXTURES_H_

#include "../Simul.h"

struct NetP2PFixture: public simul::P2PNet {
};

struct NetWifiFixture: public simul::WifiNet {
};

struct SimulationFixture: public simul::Simulation {
};

#endif /* TEST_SIMPLEFIXTURES_H_ */
