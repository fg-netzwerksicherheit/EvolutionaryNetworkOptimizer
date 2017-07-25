/*
 * Topology.h
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */
#ifndef TOPOLOGY_H_
#define TOPOLOGY_H_

#include "AnyNode.h"
#include "AnyEdge.h"
#include "ITopology.h"

namespace anygraph {

class Topology: public ITopology {
private:
	vector<AnyNode> node_list;
	vector<AnyEdge> edge_list;
	vector<Attributes> topology_attributes;
	string id;

public:
	Topology();
	virtual ~Topology();

	void add_node(AnyNode node);
	void add_edge(AnyEdge edge);
	void add_attribute(Attributes t);
	void set_ID(string s);
	int get_node_size();
	int get_edge_size();
	int get_device_size();

	virtual vector<AnyNode> get_node() override;
	virtual vector<AnyEdge> get_edge() override;
	virtual vector<Attributes> get_attributes() override;
};

} /* namespace std */

#endif /* TOPOLOGY_H_ */
