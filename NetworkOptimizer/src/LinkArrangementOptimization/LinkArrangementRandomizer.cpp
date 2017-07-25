/*
 * LinkArrangementRandomizer.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#include "LinkArrangementRandomizer.h"
#include "LinkArrangementDecoder.h"

EA_TYPEINFO_CUSTOM_IMPL(LinkArrangementRandomizer) {
	return *ea::TypeInfo("LinkArrangementRandomizer")
	.SetConstructor<LinkArrangementRandomizer>();
}

LinkArrangementRandomizer::LinkArrangementRandomizer() :
		ea::IntRandomizer(0, LA_DEC_CB_LENGTH - 1) {
}

LinkArrangementRandomizer::~LinkArrangementRandomizer() {
}

