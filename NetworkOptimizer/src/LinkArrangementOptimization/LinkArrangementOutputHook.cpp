/*
 * LinkArrangementOutputHook.cpp
 *
 *  Created on: Jun 20, 2017
 *      Author: Kiet Vo
 */

#include "LinkArrangementOutputHook.h"
#include "LinkArrangementFitness.h"
#include <iomanip>

EA_TYPEINFO_CUSTOM_IMPL(LinkArrangementOutputHook) {
	return *ea::TypeInfo("LinkArrangementOutputHook").SetConstructor<LinkArrangementOutputHook, bool>("print-genome");
}

LinkArrangementOutputHook::LinkArrangementOutputHook(bool pPrintGenome) :
		printGenome(pPrintGenome) {
}

LinkArrangementOutputHook::~LinkArrangementOutputHook() {
}

using namespace ea;
using namespace std;

void LinkArrangementOutputHook::DoGenerational() {
	OrganismPtr organism = GetBestOrganism();
	LogStream info(Log::INFO);
	info.imbue(std::locale("en_US.UTF-8"));

	info << "Gen " << setw(6) << GetGeneration() << "\tBest = ("
			<< *dynamic_pointer_cast<LinkArrangementFitness>(organism->GetFitness()) << ")";
	if (printGenome) {
		info << "\t" << *organism->GetGenome();
	}
	info << flush;
}
