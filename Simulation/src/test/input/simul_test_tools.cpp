/*
 * tools.h
 *
 *  Created on: Jun 17, 2017
 *      Author: Kiet Vo
 */

#ifndef TEST_INPUT_TOOLS_CPP_
#define TEST_INPUT_TOOLS_CPP_

#include <unistd.h>
#include <string>
#include <iostream>

static bool endsWith(const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size()
			&& str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string GetProjectSimulDirectory() {

	char *current_dir = get_current_dir_name();
	std::string path("");

	if (endsWith(std::string(current_dir), "Simulation"))
		path = std::string(current_dir);
	else if (endsWith(std::string(current_dir), "Simulation/Debug"))
		path = std::string(current_dir) + "/..";
	else if (endsWith(std::string(current_dir), "Simulation/Optimized"))
		path = std::string(current_dir) + "/..";
	else
		std::cout << "[GetProjectSimulDirectory] Path not found!" << std::endl;

	return path;
}

#endif /* TEST_INPUT_TOOLS_CPP_ */
