/*
 * P2PNetMLHandler.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: quang
 */

#include "P2PNetMLHandler.h"
#include <vector>
#include <tinyxml2.h>
#include <fstream>
#include <string>
#include "Components/Topology.h"

using namespace std;
using namespace tinyxml2;

namespace anygraph {

P2PNetMLHandler::P2PNetMLHandler() {
	// TODO Auto-generated constructor stub

}

P2PNetMLHandler::~P2PNetMLHandler() {
	// TODO Auto-generated destructor stub
}
std::shared_ptr<Topology> P2PNetMLHandler::ReadGraph(std::string file_name) {
	std::shared_ptr<Topology> top = make_shared<Topology>();
	vector<Attributes> combo_key_name;
	Attributes e_key_name, attr;
	XMLDocument doc;
	doc.LoadFile(file_name.c_str());

	XMLElement* root = doc.RootElement();
	XMLElement* graph = root->FirstChildElement("graph");

	for (XMLElement* attribute_name = root->FirstChildElement("key");
			attribute_name;
			attribute_name = attribute_name->NextSiblingElement("key")) {
		//cout<<"begin"<<endl;
		e_key_name = Attributes();
		e_key_name.set_id(attribute_name->Attribute("id"));
		e_key_name.set_name(attribute_name->Attribute("attr.name"));
		e_key_name.set_type(attribute_name->Attribute("attr.type"));
		combo_key_name.push_back(e_key_name);
		//cout<<"end"<<endl;

	}

	for (XMLElement* node = graph->FirstChildElement("node"); node;
			node = node->NextSiblingElement("node")) {
		//cout<<"begin note"<<endl;
		AnyNode anyNode(node->Attribute("id"));
		for (XMLElement *attribute = node->FirstChildElement("data"); attribute;
				attribute = attribute->NextSiblingElement("data")) {
			attr = Attributes();
			attr.set_id(attribute->Attribute("key"));
			for (uint i = 0; i < combo_key_name.size(); ++i) {
				if (combo_key_name[i].get_attribute_id()
						== attribute->Attribute("key")) {
					attr.set_name(combo_key_name[i].get_attribute_name());
					attr.set_type(combo_key_name[i].get_attribute_type());
				}
			}
			if (attribute->GetText()) {
				attr.set_data(attribute->GetText());
			}
			anyNode.add_attributes(attr);
		}
		top->add_node(anyNode);
	}
	for (XMLElement* edge = graph->FirstChildElement("edge"); edge;
			edge = edge->NextSiblingElement("edge")) {
		AnyEdge anyEdge(edge->Attribute("source"), edge->Attribute("target"));
		for (XMLElement *attribute = edge->FirstChildElement("data"); attribute;
				attribute = attribute->NextSiblingElement("data")) {
			attr = Attributes();
			attr.set_id(attribute->Attribute("key"));
			for (uint i = 0; i < combo_key_name.size(); ++i) {
				if (combo_key_name[i].get_attribute_id()
						== attribute->Attribute("key")) {
					attr.set_name(combo_key_name[i].get_attribute_name());
					attr.set_type(combo_key_name[i].get_attribute_type());
				}
			}
			if (attribute->GetText()) {
				attr.set_data(attribute->GetText());
			}
			anyEdge.add_attribute(attr);
		}
		top->add_edge(anyEdge);
	}

	for (auto att : combo_key_name)
		top->add_attribute(att);

	return top;
}

void P2PNetMLHandler::WriteGraph(std::shared_ptr<Topology> topo, std::string file_name) {
	XMLDocument xmlDoc;
	std::string filetype = "p2pnetml";
	XMLNode * pRoot = xmlDoc.NewElement(filetype.c_str());
	xmlDoc.InsertFirstChild(pRoot);

	// write graph's keys
	for (Attributes att : topo->get_attributes()) {
		XMLElement * key_element = xmlDoc.NewElement("key");
		key_element->SetAttribute("id", att.get_attribute_id().c_str());
		key_element->SetAttribute("attr.name",
				att.get_attribute_name().c_str());
		key_element->SetAttribute("attr.type",
				att.get_attribute_type().c_str());
		pRoot->InsertEndChild(key_element);
	}

	// write graph nodes & edges
	XMLElement * graph_element = xmlDoc.NewElement("graph");

	// write nodes
	for (AnyNode node : topo->get_node()) {
		XMLElement * node_element = xmlDoc.NewElement("node");
		node_element->SetAttribute("id", node.get_ID().c_str());

		// write a node's data
		for (Attributes att : node.get_attribute_list()) {
			XMLElement * data_element = xmlDoc.NewElement("data");
			data_element->SetAttribute("key", att.get_attribute_id().c_str());
			data_element->SetText(att.get_attribute_data().c_str());
			node_element->InsertEndChild(data_element);
		}
		graph_element->InsertEndChild(node_element);
	}

	// write edges
	for (AnyEdge edge : topo->get_edge()) {
		XMLElement * edge_element = xmlDoc.NewElement("edge");
		edge_element->SetAttribute("source", edge.get_From().c_str());
		edge_element->SetAttribute("target", edge.get_To().c_str());

		// write an edge's data
		for (Attributes att : edge.get_Attribute()) {
			XMLElement * data_element = xmlDoc.NewElement("data");
			data_element->SetAttribute("key", att.get_attribute_id().c_str());
			data_element->SetText(att.get_attribute_data().c_str());
			edge_element->InsertEndChild(data_element);
		}
		graph_element->InsertEndChild(edge_element);
	}

	pRoot->InsertEndChild(graph_element);

	XMLError eResult = xmlDoc.SaveFile(file_name.c_str());
	if (eResult != XML_SUCCESS)
		throw "Error writing " + filetype + " file!";
}
} /* namespace anygraph */
