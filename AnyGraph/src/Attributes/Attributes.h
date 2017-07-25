/*
 * Attributes.h
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */

#ifndef ATTRIBUTES_H_
#define ATTRIBUTES_H_

#include<string>

namespace anygraph {
using namespace std;
class Attributes {
private:
	string attribute_name;
	string attribute_data;
	string attribute_id;
	string attribute_type;
public:
	Attributes();
	void set_id(string id_data);
	void set_name(string name_data);
	void set_data(string data_data);
	void set_type(string type);
	string get_attribute_name();
	string get_attribute_id();
	string get_attribute_data();
	string get_attribute_type();
	virtual ~Attributes();
};

} /* namespace std */

#endif /* ATTRIBUTES_H_ */
