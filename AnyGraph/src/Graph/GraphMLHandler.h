/*
 * GraphMLHandler.h
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */

#ifndef GRAPH_GRAPHMLHANDLER_H_
#define GRAPH_GRAPHMLHANDLER_H_

#include "GraphHandler.h"

namespace anygraph {

class GraphMLHandler: public GraphHandler {
public:
	GraphMLHandler();
	virtual ~GraphMLHandler();
	virtual std::shared_ptr<Topology> ReadGraph(std::string file_name) override;
	virtual void WriteGraph(std::shared_ptr<Topology>topo, std::string file_name)
			override;
};

} /* namespace anygraph */

#endif /* GRAPH_GRAPHMLHANDLER_H_ */
