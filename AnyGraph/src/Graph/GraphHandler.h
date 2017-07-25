/*
 * ClassHandler.h
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */

#ifndef GRAPH_GRAPHHANDLER_H_
#define GRAPH_GRAPHHANDLER_H_

#include <iostream>
#include <memory>
#include <string>
#include "Components/Topology.h"

namespace anygraph {

class GraphHandler {
public:
	GraphHandler();
	virtual ~GraphHandler();
	virtual std::shared_ptr<Topology> ReadGraph(std::string file_name)=0;
	virtual void WriteGraph(std::shared_ptr<Topology>topo, std::string file_name)=0;
};

} /* namespace anygraph */

#endif /* GRAPH_GRAPHHANDLER_H_ */
