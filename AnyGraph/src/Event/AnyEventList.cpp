/*
 * AnyEventList.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Quang
 */

#include "AnyEventList.h"

namespace anygraph {
AnyEventList::AnyEventList() {
	// TODO Auto-generated constructor stub

}

AnyEventList::~AnyEventList() {
	// TODO Auto-generated destructor stub
}
AnyEvent AnyEventList::get_event_at(int index) {
	return event_list[index];
}
void AnyEventList::add_event(AnyEvent event) {
	event_list.push_back(event);
}
int AnyEventList::get_number_of_event() {
	return event_list.size();
}
void AnyEventList::read_json_event(string jsonpath) {
	AnyEvent single_event;
	Attributes single_attribute;
	Json::Reader reader;
	Json::Value obj;
	ifstream file(jsonpath.c_str());
	reader.parse(file, obj);
	const Json::Value& characters = obj["events"];
	for (uint i = 0; i < characters.size(); i++) {
		if (characters[i].isNull()) {
			break;
		}
		single_event = AnyEvent(characters[i]["name"].asString());

		single_attribute = Attributes();
		single_attribute.set_name("protocol");
		single_attribute.set_data(characters[i]["protocol"].asString());
		single_event.add_attribute(single_attribute);

		single_attribute = Attributes();
		single_attribute.set_name("start");
		single_attribute.set_data(characters[i]["start"].asString());
		single_event.add_attribute(single_attribute);

		single_attribute = Attributes();
		single_attribute.set_name("src");
		single_attribute.set_data(characters[i]["src"].asString());
		single_event.add_attribute(single_attribute);

		single_attribute = Attributes();
		single_attribute.set_name("dst");
		single_attribute.set_data(characters[i]["dst"].asString());
		single_event.add_attribute(single_attribute);

		single_attribute = Attributes();
		single_attribute.set_name("datatransfer");
		single_attribute.set_data(characters[i]["datatransfer"].asString());
		single_event.add_attribute(single_attribute);

		event_list.push_back(single_event);
	}
}
void AnyEventList::read_xml_event(string path) {

	using namespace tinyxml2;
	AnyEvent single_event;
	Attributes attr;
	XMLElement* attribute;

	XMLDocument doc;
	doc.LoadFile(path.c_str());

	XMLElement* root = doc.RootElement();

	for (XMLElement* event = root->FirstChildElement("event"); event; event =
			event->NextSiblingElement("event")) {
		attribute = event->FirstChildElement("name");
		single_event = AnyEvent(attribute->GetText());

		attribute = event->FirstChildElement("protocol");
		attr = Attributes();
		attr.set_name("protocol");
		attr.set_data(attribute->GetText());
		single_event.add_attribute(attr);

		attribute = event->FirstChildElement("start");
		attr = Attributes();
		attr.set_name("start");
		attr.set_data(attribute->GetText());
		single_event.add_attribute(attr);

		attribute = event->FirstChildElement("src");
		attr = Attributes();
		attr.set_name("src");
		attr.set_data(attribute->GetText());
		single_event.add_attribute(attr);

		attribute = event->FirstChildElement("dst");
		attr = Attributes();
		attr.set_name("dst");
		attr.set_data(attribute->GetText());
		single_event.add_attribute(attr);

		attribute = event->FirstChildElement("datatransfer");
		attr = Attributes();
		attr.set_name("datatransfer");
		attr.set_data(attribute->GetText());
		single_event.add_attribute(attr);

		event_list.push_back(single_event);
	}

}
}

