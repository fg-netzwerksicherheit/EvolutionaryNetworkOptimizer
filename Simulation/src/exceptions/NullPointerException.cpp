/*
 * NullPointerException.cpp
 *
 *  Created on: Mar 31, 2017
 *      Author: Kiet Vo
 */

#include "NullPointerException.h"

namespace simul {

NullPointerException::NullPointerException(string message) {
	this->msg = message;
}

NullPointerException::~NullPointerException() {
}

string NullPointerException::GetMessage() {
	return msg;
}

const char * NullPointerException::what() {
	return msg.c_str();
}

} /* namespace simul */
