/*
 * NetIntAllInitializer.h
 *
 *  Created on: Jun 14, 2017
 *      Author: Kiet Vo
 */

#ifndef NETINTERFACEALLOCATION_NETINTALLINITIALIZER_H_
#define NETINTERFACEALLOCATION_NETINTALLINITIALIZER_H_

#include <openea/core/interface/Initializer.h>

class NetIntAllInitializer: public ea::Initializer {
public:
	NetIntAllInitializer(std::string graphFile);
	virtual ~NetIntAllInitializer();

private:
	virtual ea::GenomePoolPtr DoInitialize(uint pSize) override;
};

#endif /* NETINTERFACEALLOCATION_NETINTALLINITIALIZER_H_ */
