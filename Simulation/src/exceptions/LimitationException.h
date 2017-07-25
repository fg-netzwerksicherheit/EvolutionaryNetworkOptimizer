/*
 * LimitationException.h
 *
 *  Created on: Apr 15, 2017
 *      Author: Kiet Vo
 */

#ifndef EXCEPTIONS_LIMITATIONEXCEPTION_H_
#define EXCEPTIONS_LIMITATIONEXCEPTION_H_

#include "../Si_Type.h"

namespace simul {

class LimitationException: public std::exception {
private:
	string msg;
public:
	LimitationException(string message);
	virtual ~LimitationException();

	string GetMessage();
	const char * what();
};

} /* namespace ea */

#endif /* EXCEPTIONS_LIMITATIONEXCEPTION_H_ */
