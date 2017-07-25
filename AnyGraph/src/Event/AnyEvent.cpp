/*
 * Event.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: quang
 */

#include "AnyEvent.h"

namespace anygraph {

	AnyEvent::AnyEvent(){
		// TODO Auto-generated constructor stub

	}
	AnyEvent::AnyEvent(string id){
		event_id = id;
	}


	AnyEvent::~AnyEvent() {
		// TODO Auto-generated destructor stub
	}
	vector<Attributes> AnyEvent::get_attributes_list()
	{
		return event_attributes;
	}
	void AnyEvent::add_attribute(Attributes attr)
	{
		event_attributes.push_back(attr);
	}
	int AnyEvent::get_attribute_size()
	{
		return event_attributes.size();
	}
	string AnyEvent::get_attribute_name_at(int index){
		return event_attributes[index].get_attribute_name();

	}
	string AnyEvent::get_attribute_data_at(int index){
		return event_attributes[index].get_attribute_data();
	}


} /* namespace std */
