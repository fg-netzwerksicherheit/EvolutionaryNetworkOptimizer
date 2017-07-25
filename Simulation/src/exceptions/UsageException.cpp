/*
 * UsageException.cpp
 *
 *  Created on: Mar 31, 2017
 *      Author: Kiet Vo
 */

#include "UsageException.h"

namespace simul {

UsageException::UsageException(string message) {
	this->msg = message;
}

UsageException::~UsageException() {
}

string UsageException::GetMessage() {
	return msg;
}

const char * UsageException::what() {
	return msg.c_str();
}

} /* namespace simul */
