/*
 * Attributes.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: quang
 */

#include "Attributes.h"

namespace anygraph {

Attributes::Attributes() {
	attribute_name = ("");
	attribute_data = ("");
	attribute_id = ("");
	attribute_type = ("");

}

Attributes::~Attributes() {
	// TODO Auto-generated destructor stub
}

string Attributes::get_attribute_data()
{
	return attribute_data;
}
string Attributes::get_attribute_id()
{
	return attribute_id;
}
string Attributes::get_attribute_name()
{
	return attribute_name;
}
string Attributes::get_attribute_type()
{
	return attribute_type;
}
void Attributes::set_id(string id_data)
{
	attribute_id = id_data;
}
void Attributes::set_name(string name_data)
{
	attribute_name = name_data;
}
void Attributes::set_data(string data_data)
{
	attribute_data = data_data;
}
void Attributes::set_type(string type)
{
	attribute_type = type;
}

} /* namespace std */
