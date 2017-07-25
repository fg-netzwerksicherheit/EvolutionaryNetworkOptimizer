/*
 * NullPointerException.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Kiet Vo
 */

#ifndef EXCEPTIONS_NULLPOINTEREXCEPTION_H_
#define EXCEPTIONS_NULLPOINTEREXCEPTION_H_

#include "../Si_Type.h"

namespace simul {

class NullPointerException: public std::exception {
private:
	string msg;
public:
	NullPointerException(string message);
	virtual ~NullPointerException();

	string GetMessage();
	const char * what();
};

} /* namespace simul */

#endif /* EXCEPTIONS_NULLPOINTEREXCEPTION_H_ */
