/*
 * P2PEdge.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#include "P2PEdge.h"

namespace simul {

//NS_LOG_COMPONENT_DEFINE("simul::P2PEdge");

P2PEdge::P2PEdge(NodePtr sourceNode, NodePtr destNode) :
		Edge(sourceNode, destNode) {

	// default values for attributes
	this->dataRateSrc = DATARATE_INFINITY;
	this->dataRateDst = DATARATE_INFINITY;
	this->mtuSrc = 1500;
	this->mtuDst = 1500;
	this->interframeGapSrc = "0ns";
	this->interframeGapDst = "0ns";
	this->delay = "0ms";
}

P2PEdge::~P2PEdge() {
}

void P2PEdge::DoNothing() {
}

/**
 * \brief "0bps" means an infinite data rate value.
 *
 * \attention According to ns3.26, an extremely small value (near "0bps") might be interpreted as infinite value.
 */
void P2PEdge::SetDataRateSrc(string dataRate) {
	this->dataRateSrc = dataRate;
}

/**
 * \brief "0bps" means an infinite data rate value.
 *
 * \attention According to ns3.26, an extremely small value (near "0bps") might be interpreted as infinite value.
 */
void P2PEdge::SetDataRateDst(string dataRate) {
	this->dataRateDst = dataRate;
}

void P2PEdge::SetMTUSrc(uint16_t mtu) {
	this->mtuSrc = mtu;
}

void P2PEdge::SetMTUDst(uint16_t mtu) {
	this->mtuDst = mtu;
}

void P2PEdge::SetInterframeGapSrc(string time) {
	this->interframeGapSrc = time;
}

void P2PEdge::SetInterframeGapDst(string time) {
	this->interframeGapDst = time;
}

// getters

/**
 * \brief Propagation delay of the link.
 */
void P2PEdge::SetDelay(string delay) {
	this->delay = delay;
}

string P2PEdge::GetDataRateSrc() const {
	return dataRateSrc;
}

string P2PEdge::GetDataRateDst() const {
	return dataRateDst;
}

uint16_t P2PEdge::GetMTUSrc() const {
	return mtuSrc;
}

uint16_t P2PEdge::GetMTUDst() const {
	return mtuDst;
}

string P2PEdge::GetInterframeGapSrc() const {
	return interframeGapSrc;
}

string P2PEdge::GetInterframeGapDst() const {
	return interframeGapDst;
}

/**
 * \brief Propagation delay of the link.
 */
string P2PEdge::GetDelay() const {
	return delay;
}

using namespace ns3;

/**
 * \brief Propagate the new configuration data to the current ns-3 simulator.
 *
 * This method saves users from the need to re-configure the ns-3 simulator.
 */
bool P2PEdge::UpdateConfiguration() {

	if (srcDevice == nullptr || dstDevice == nullptr)
		return false;

	Ptr<PointToPointChannel> channel = DynamicCast<PointToPointChannel>(srcDevice->GetChannel());
	Ptr<PointToPointChannel> channel_2 = DynamicCast<PointToPointChannel>(dstDevice->GetChannel());

	if (channel == nullptr || channel_2 == nullptr || channel != channel_2)
		return false;

	srcDevice->SetDataRate(DataRate(GetDataRateSrc()));
	srcDevice->SetMtu(GetMTUSrc());
	srcDevice->SetInterframeGap(Time(GetInterframeGapSrc()));

	dstDevice->SetDataRate(DataRate(GetDataRateDst()));
	dstDevice->SetMtu(GetMTUDst());
	dstDevice->SetInterframeGap(Time(GetInterframeGapDst()));

	channel->SetAttribute("Delay", StringValue(GetDelay()));

	return true;
}

} /* namespace simul */
