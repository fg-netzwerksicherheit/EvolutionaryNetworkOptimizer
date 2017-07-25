/*
 * IAnyNode.h
 *
 *  Created on: Apr 9, 2017
 *      Author: Kiet Vo
 */

#ifndef GRAPH_IANYNODE_H_
#define GRAPH_IANYNODE_H_

#include <string>

namespace anygraph {

class IAnyNode {
public:
	virtual ~IAnyNode() {
	}

	virtual void set_ID(string) = 0;
	virtual std::string get_ID() = 0;
};

} /* namespace anygraph */

#endif /* GRAPH_IANYNODE_H_ */
