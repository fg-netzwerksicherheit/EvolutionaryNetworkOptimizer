/*
 * Node.h
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */

#ifndef ANYNODE_H_
#define ANYNODE_H_

#include "../../Attributes/Attributes.h"
#include <vector>
#include "IAnyNode.h"

namespace anygraph {
using namespace std;

class AnyNode :public IAnyNode{
private:
	string id;
	vector<Attributes> attributes;

public:
	AnyNode();
	AnyNode(string id_key);
	void set_ID(string id_data);
	void add_attributes(Attributes attr);
	string get_ID();
	vector<Attributes> get_attribute_list();
	Attributes get_attribute_at(int index);
	virtual ~AnyNode();
};

}

#endif /* ANYNODE_H_ */
