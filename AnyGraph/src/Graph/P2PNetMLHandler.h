/*
 * P2PNetMLHandler.h
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */

#ifndef GRAPH_P2PNETMLHANDLER_H_
#define GRAPH_P2PNETMLHANDLER_H_

#include "GraphHandler.h"

namespace anygraph {

class P2PNetMLHandler: public GraphHandler {
public:
	P2PNetMLHandler();
	virtual ~P2PNetMLHandler();
	virtual std::shared_ptr<Topology> ReadGraph(std::string file_name) override;
	virtual void WriteGraph(std::shared_ptr<Topology>topo, std::string file_name)
			override;
};

} /* namespace anygraph */

#endif /* GRAPH_P2PNETMLHANDLER_H_ */
