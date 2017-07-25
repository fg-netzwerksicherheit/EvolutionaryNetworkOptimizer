/*
 * Event.h
 *
 *  Created on: Apr 3, 2017
 *      Author: quang
 */

#ifndef EVENT_H_
#define EVENT_H_

#include"../Attributes/Attributes.h"
#include <vector>
#include <string>

namespace anygraph {

using namespace std;

class AnyEvent {
private:
	vector<Attributes>event_attributes;
	string event_id;
public:
	AnyEvent();
	AnyEvent(string id);
	vector<Attributes>get_attributes_list();
	void add_attribute(Attributes attr);
	int get_attribute_size();
	string get_attribute_name_at(int index);
	string get_attribute_data_at(int index);
	virtual ~AnyEvent();
};

} /* namespace std */

#endif /* EVENT_H_ */
