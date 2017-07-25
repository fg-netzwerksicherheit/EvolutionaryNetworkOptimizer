/*
 * UsageException.h
 *
 *  Created on: Mar 31, 2017
 *      Author: Kiet Vo
 */

#ifndef EXCEPTIONS_USAGEEXCEPTION_H_
#define EXCEPTIONS_USAGEEXCEPTION_H_

#include "../Si_Type.h"

namespace simul {

class UsageException: public std::exception {
private:
	string msg;
public:
	UsageException(string message);
	virtual ~UsageException();

	string GetMessage();
	const char * what();
};

} /* namespace simul */

#endif /* EXCEPTIONS_USAGEEXCEPTION_H_ */
