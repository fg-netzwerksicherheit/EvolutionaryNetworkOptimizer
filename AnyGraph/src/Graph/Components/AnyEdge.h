/*
 * Edge.h
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */

#ifndef ANYEDGE_H_
#define ANYEDGE_H_

#include "../../Attributes/Attributes.h"
#include<vector>
#include "IAnyEdge.h"

namespace anygraph {
using namespace std;

class AnyEdge :public IAnyEdge{
private:
	string from;
	string to;
	vector<Attributes> attributes;
	string id;
public:
	AnyEdge();
	AnyEdge(string fromNode, string toNode);
	void set_from(string fromNode);
	void set_to(string toNode);
	void set_ID(string i);
	void add_attribute(Attributes a);
	string get_From();
	string get_To();
	vector<Attributes> get_Attribute();
	Attributes get_attribute_at(int index);
	virtual ~AnyEdge();
};

}

#endif /* ANYEDGE_H_ */
