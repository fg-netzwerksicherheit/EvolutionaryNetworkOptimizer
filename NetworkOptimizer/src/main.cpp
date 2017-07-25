/*
 * main.cpp
 *
 *  Created on: Mar 28, 2017
 *      Author: Kiet Vo
 */

#include "Simul.h"

int DataRateMain(bool cluster);
int LinkArrangementMain(bool cluster);
int LinkDelayMain(bool cluster);

int main(int argc, char * argv[]) {

	std::string program = "linkarrangement";
	bool cluster = false;

	ns3::CommandLine cmd;
	cmd.AddValue("program", "Specific program to run", program);
	cmd.AddValue("cluster", "Distributed running", cluster);

	cmd.Parse(argc, argv);

	if (program == "datarate")
		DataRateMain(cluster);
	else if (program == "linkarrangement")
		LinkArrangementMain(cluster);
	else if (program == "linkdelay")
		LinkDelayMain(cluster);

	return 0;
}

