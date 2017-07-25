/*
 * MyAppHelper.h
 *
 *  Created on: May 1, 2017
 *      Author: Quang Pham
 */

#ifndef SIMULATION_EVENTS_MYAPPHELPER_H_
#define SIMULATION_EVENTS_MYAPPHELPER_H_

#include "../../../Si_Type.h"

namespace simul {

/**
 * \ingroup apps
 */
class OnOffAppHelper {
public:
	OnOffAppHelper(std::string protocol, ns3::Address address);

	void SetAttribute(std::string name, const ns3::AttributeValue &value);
	void SetConstantRate(ns3::DataRate dataRate, uint32_t packetSize = 512);

	ns3::ApplicationContainer Install(ns3::NodeContainer c) const;
	ns3::ApplicationContainer Install(ns3::Ptr<ns3::Node> node) const;
	ns3::ApplicationContainer Install(std::string nodeName) const;
	int64_t AssignStreams(ns3::NodeContainer c, int64_t stream);

private:
	ns3::Ptr<ns3::Application> InstallPriv(ns3::Ptr<ns3::Node> node) const;
	ns3::ObjectFactory m_factory;
};

} /* namespace simul */

#endif /* SIMULATION_EVENTS_MYAPPHELPER_H_ */
