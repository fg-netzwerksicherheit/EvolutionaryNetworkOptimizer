/*
 * Si_Type.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef SI_TYPE_H_
#define SI_TYPE_H_

#include "Si_Common.h"

namespace simul {

using namespace std;

// Pointer alias macro
#define DEFINE_PTR_TYPE_IN_SIMUL(CLASS)\
	class CLASS;\
	using CLASS ## Ptr = std::shared_ptr<CLASS>;

//
DEFINE_PTR_TYPE_IN_SIMUL(Simulation);

// Pointer to base classes
DEFINE_PTR_TYPE_IN_SIMUL(Edge)
DEFINE_PTR_TYPE_IN_SIMUL(Node)
DEFINE_PTR_TYPE_IN_SIMUL(IPacketInfoOutput)
DEFINE_PTR_TYPE_IN_SIMUL(Ipv4PacketInfoOutput)
DEFINE_PTR_TYPE_IN_SIMUL(PacketInfo)
DEFINE_PTR_TYPE_IN_SIMUL(Graph)
DEFINE_PTR_TYPE_IN_SIMUL(Event)
DEFINE_PTR_TYPE_IN_SIMUL(Network)

// Pointer to derived classes
DEFINE_PTR_TYPE_IN_SIMUL(P2PEdge)
}

#endif /* SI_TYPE_H_ */

