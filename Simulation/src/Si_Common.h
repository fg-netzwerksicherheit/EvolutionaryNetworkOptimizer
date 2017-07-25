/*
 * Si_Common.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef SI_COMMON_H_
#define SI_COMMON_H_

#include <memory>
#include <set>
#include <queue>
#include <iostream>
#include <assert.h>
#include <ctime>
#include <string>
#include <exception>
#include <unordered_map>
#include <vector>
#include <limits>

#define NS3_LOG_ENABLE

#include "ns3/address.h"
#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/data-rate.h"
#include "ns3/traced-callback.h"

#include "ns3/ptr.h"
#include "ns3/vector.h"
#include "ns3/tag.h"

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-helper.h"
#include "ns3/ipv4-nix-vector-helper.h"

#include "AnyGraph.h"

namespace simul {

#define make_shared_base_in_simul(BASE, DERIVED, ...)\
std::static_pointer_cast<BASE> (std::make_shared<DERIVED>(__VA_ARGS__))

template<typename T>
using SetPtr = std::shared_ptr<std::set<T>>;
template<typename T>
using VectorPtr = std::shared_ptr<std::vector<T>>;

// anygraph (Quang)
using ITopologyPtr = std::shared_ptr<anygraph::ITopology>;
using TopologyPtr = std::shared_ptr<anygraph::Topology>;
using AnyEventListPtr = std:: shared_ptr<anygraph::AnyEventList>;
}

#endif /* SI_COMMON_H_ */
