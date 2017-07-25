/*
 * IAnyEdge.h
 *
 *  Created on: Apr 9, 2017
 *      Author: Kiet Vo
 */

#ifndef GRAPH_IANYEDGE_H_
#define GRAPH_IANYEDGE_H_

#include <string>

namespace anygraph {

class IAnyEdge {
public:
	virtual ~IAnyEdge() {
	}

	virtual std::string get_From() = 0;
	virtual std::string get_To() = 0;
};

} /* namespace anygraph */

#endif /* GRAPH_IANYEDGE_H_ */
