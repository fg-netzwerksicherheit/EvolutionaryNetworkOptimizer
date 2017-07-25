/*
 * EventInput.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Quang Pham, Kiet Vo
 */

#include "EventInput.h"
#include "../components/Node.h"
#include "../simulation/events/TcpAppEvent.h"
#include "../simulation/events/UdpAppEvent.h"
#include "../simulation/events/UdpTestEvent.h"
#include "../simulation/Simulation.h"

namespace simul {

EventInput::EventInput() {
}

EventInput::~EventInput() {
}

enum EventType {
	TCPTYPE_EVENT, UDPTYPE_EVENT, UDPTESTTYPE_EVENT
};

vector<weak_ptr<Event>> EventInput::ReadEvent(string path, SimulationPtr si) {

	AnyEventListPtr list_of_event = make_shared<anygraph::AnyEventList>();
	if (path.find(".json") != string::npos) {
		list_of_event->read_json_event(path);
	}
	if (path.find(".xml") != string::npos) {
		list_of_event->read_xml_event(path);
	}

	weak_ptr<Event> event;
	NodePtr sourceNode;
	NodePtr destNode;
	vector<weak_ptr<Event>> event_list;
	string start;
	string data;
	EventType eventType = UDPTESTTYPE_EVENT;

	//cout << "total events: " << list_of_event->get_number_of_event() << endl;
	for (int j = 0; j < list_of_event->get_number_of_event(); ++j) {
		for (int i = 0; i < list_of_event->get_event_at(j).get_attribute_size(); ++i) {
			if (list_of_event->get_event_at(j).get_attribute_name_at(i) == "src") {
				string sourceID = list_of_event->get_event_at(j).get_attribute_data_at(i);
				sourceNode = si->GetNode(sourceID);
				if (sourceNode == nullptr)
					sourceNode = make_shared<Node>(sourceID);
			}

			if (list_of_event->get_event_at(j).get_attribute_name_at(i) == "dst") {
				string destID = list_of_event->get_event_at(j).get_attribute_data_at(i);
				destNode = si->GetNode(destID);
				if (destNode == nullptr)
					destNode = make_shared<Node>(destID);
			}

			if (list_of_event->get_event_at(j).get_attribute_name_at(i) == "protocol") {

				if (list_of_event->get_event_at(j).get_attribute_data_at(i) == "tcp")
					eventType = TCPTYPE_EVENT;
				else if (list_of_event->get_event_at(j).get_attribute_data_at(i) == "udp")
					eventType = UDPTYPE_EVENT;
				else if (list_of_event->get_event_at(j).get_attribute_data_at(i) == "udptest")
					eventType = UDPTESTTYPE_EVENT;
			}

			if (list_of_event->get_event_at(j).get_attribute_name_at(i) == "datatransfer")
				data = list_of_event->get_event_at(j).get_attribute_data_at(i);

			if (list_of_event->get_event_at(j).get_attribute_name_at(i) == "start")
				start = list_of_event->get_event_at(j).get_attribute_data_at(i);
		}

		double startTime = atof(start.c_str());
		uint32_t bytes = atoi(data.c_str());

		switch (eventType) {
		case (UDPTESTTYPE_EVENT):
			event = si->CreateEvent<UdpTestEvent>(sourceNode, destNode);
			break;
		case (TCPTYPE_EVENT):
			event = si->CreateEvent<TcpAppEvent>(sourceNode, destNode, bytes);
			break;
		case (UDPTYPE_EVENT):
			event = si->CreateEvent<UdpAppEvent>(sourceNode, destNode, bytes);
			break;
		default:
			event = si->CreateEvent<UdpTestEvent>(sourceNode, destNode);
		}
		event.lock()->SetStartTime(startTime);

		event_list.push_back(event);
	}
	return event_list;
}

}

/* namespace std */
