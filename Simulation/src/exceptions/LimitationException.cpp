/*
 * LimitationException.cpp
 *
 *  Created on: Apr 15, 2017
 *      Author: Kiet Vo
 */

#include "LimitationException.h"

namespace simul {

LimitationException::LimitationException(string message) {
	this->msg = message;
}

LimitationException::~LimitationException() {
}

string LimitationException::GetMessage() {
	return msg;
}

const char * LimitationException::what() {
	return msg.c_str();
}

} /* namespace ea */
