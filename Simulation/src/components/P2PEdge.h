/*
 * P2PEdge.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef COMPONENTS_P2PEDGE_H_
#define COMPONENTS_P2PEDGE_H_

#include "Edge.h"

namespace simul {

/**
 * \ingroup components
 *
 * \brief Implementation of a point-to-point link.
 */
class P2PEdge: public Edge {
	friend class P2PNet;

private:
	string dataRateSrc;
	string dataRateDst;
	uint16_t mtuSrc;
	uint16_t mtuDst;
	string interframeGapSrc;
	string interframeGapDst;
	string delay;

	ns3::Ptr<ns3::PointToPointNetDevice> srcDevice;
	ns3::Ptr<ns3::PointToPointNetDevice> dstDevice;

	void DoNothing();

public:
	P2PEdge(NodePtr sourceNode, NodePtr destNode);
	virtual ~P2PEdge();

	void SetDataRateSrc(string dataRate);
	void SetDataRateDst(string dataRate);
	void SetMTUSrc(uint16_t mtu);
	void SetMTUDst(uint16_t mtu);
	void SetInterframeGapSrc(string time);
	void SetInterframeGapDst(string time);
	void SetDelay(string delay);

	string GetDataRateSrc() const;
	string GetDataRateDst() const;
	uint16_t GetMTUSrc() const;
	uint16_t GetMTUDst() const;
	string GetInterframeGapSrc() const;
	string GetInterframeGapDst() const;
	string GetDelay() const;

	bool UpdateConfiguration();
};

} /* namespace simul */

#endif /* COMPONENTS_P2PEDGE_H_ */
