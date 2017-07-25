/*
 * LinkArrangementInitializer.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#include "LinkArrangementInitializer.h"
#include "LinkArrangementRandomizer.h"

EA_TYPEINFO_CUSTOM_IMPL(LinkArrangementInitializer){
return *ea::TypeInfo("LinkArrangementInitializer")
.SetConstructor<LinkArrangementInitializer, uint>("numComputers");
}

LinkArrangementInitializer::LinkArrangementInitializer(uint numComputers) :
		ea::IntRandomArrayInitializer(numComputers * (numComputers - 1) / 2,
				std::make_shared<LinkArrangementRandomizer>()) {
}

LinkArrangementInitializer::~LinkArrangementInitializer() {
}

