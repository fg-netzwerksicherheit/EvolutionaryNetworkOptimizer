/*
 * MyAppHelper.cpp
 *
 *  Created on: May 1, 2017
 *      Author: Quang Pham
 */

#include "OnOffAppHelper.h"

#include "OnOffApp.h"

namespace simul {

using namespace ns3;

OnOffAppHelper::OnOffAppHelper(std::string protocol, Address address) {
	m_factory.SetTypeId("simul::OnOffApp");
	m_factory.Set("Protocol", StringValue(protocol));
	m_factory.Set("Remote", AddressValue(address));
}

void OnOffAppHelper::SetAttribute(std::string name, const AttributeValue &value) {
	m_factory.Set(name, value);
}

ApplicationContainer OnOffAppHelper::Install(Ptr<ns3::Node> node) const {
	return ApplicationContainer(InstallPriv(node));
}

ApplicationContainer OnOffAppHelper::Install(std::string nodeName) const {
	Ptr<ns3::Node> node = Names::Find<ns3::Node>(nodeName);
	return ApplicationContainer(InstallPriv(node));
}

ApplicationContainer OnOffAppHelper::Install(NodeContainer c) const {
	ApplicationContainer apps;
	for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i) {
		apps.Add(InstallPriv(*i));
	}

	return apps;
}

Ptr<Application> OnOffAppHelper::InstallPriv(Ptr<ns3::Node> node) const {
	Ptr<Application> app = m_factory.Create<Application>();
	node->AddApplication(app);

	return app;
}

int64_t OnOffAppHelper::AssignStreams(NodeContainer c, int64_t stream) {
	int64_t currentStream = stream;
	Ptr<ns3::Node> node;
	for (NodeContainer::Iterator i = c.Begin(); i != c.End(); ++i) {
		node = (*i);
		for (uint32_t j = 0; j < node->GetNApplications(); j++) {
			Ptr<OnOffApp> onoff = DynamicCast<OnOffApp>(node->GetApplication(j));
			if (onoff) {
				currentStream += onoff->AssignStreams(currentStream);
			}
		}
	}
	return (currentStream - stream);
}

void OnOffAppHelper::SetConstantRate(DataRate dataRate, uint32_t packetSize) {
	m_factory.Set("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1000]"));
	m_factory.Set("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
	m_factory.Set("DataRate", DataRateValue(dataRate));
	m_factory.Set("PacketSize", UintegerValue(packetSize));
}

} /* namespace simul */
