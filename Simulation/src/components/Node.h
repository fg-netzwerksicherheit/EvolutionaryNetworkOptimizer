/*
 * Node.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Kiet Vo
 */

#ifndef COMPONENTS_NODE_H_
#define COMPONENTS_NODE_H_

#include "../Si_Type.h"

namespace simul {

/**
 * \ingroup components
 */
class Node {
	friend class Simulation;

private:
	string ID;
	ns3::Vector3D position;

	ns3::Ptr<ns3::Node> ns3Node;

	void NotifySimulatorDestroyed();

public:
	Node();
	Node(string ID);
	Node(double x, double y, double z = 0);
	virtual ~Node();

	void SetID(string ID);
	void SetPosition(double x, double y, double z = 0);

	string GetID();
	ns3::Vector3D GetPosition();

	ns3::Ptr<ns3::Node> GetNs3Node();
};

} /* namespace simul */

#endif /* COMPONENTS_NODE_H_ */
