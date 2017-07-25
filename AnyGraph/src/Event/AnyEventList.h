/*
 * AnyEventList.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Quang
 */

#ifndef EVENT_ANYEVENTLIST_H_
#define EVENT_ANYEVENTLIST_H_

#include "AnyEvent.h"
#include <tinyxml2.h>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <fstream>

namespace anygraph{
	using namespace std;
	class AnyEventList {
	private:
		vector<AnyEvent>event_list;
	public:
		AnyEventList();
		AnyEvent get_event_at(int index);
		void add_event(AnyEvent event);
		int get_number_of_event();
		void read_json_event(string jsonpath);
		void read_xml_event(string path);
		virtual ~AnyEventList();
	};
}

#endif /* EVENT_ANYEVENTLIST_H_ */
