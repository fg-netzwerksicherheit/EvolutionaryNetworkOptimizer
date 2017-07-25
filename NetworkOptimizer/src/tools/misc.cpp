/*
 * misc.cpp
 *
 *  Created on: May 18, 2017
 *      Author: kiet
 */

#include <string>
#include <unistd.h>
#include <iostream>
using namespace std;

bool hasSuffix(const string &str, const string &suffix) {
	return str.size() >= suffix.size()
			&& str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

/**
 * \brief Retrieve the absolute path of this project's directory
 */
string GetProjectNetOptDirectory() {
	char *current_dir = get_current_dir_name();
	string path("");

	if (hasSuffix(string(current_dir), "NetworkOptimizer"))
		path = string(current_dir);
	else if (hasSuffix(string(current_dir), "NetworkOptimizer/Debug"))
		path = string(current_dir) + "/..";
	else if (hasSuffix(string(current_dir), "NetworkOptimizer/Debug-Test"))
		path = string(current_dir) + "/..";
	else if (hasSuffix(string(current_dir), "NetworkOptimizer/Optimized"))
		path = string(current_dir) + "/..";
	else {
		string msg = "[misc] Project's directory not found!";
		cout << msg << endl;
		throw domain_error(msg);
	}

	return path;
}

string getPathToGraphFile() {
	// read graph file
	char *current_dir = get_current_dir_name();
	string path("/input");
	if (hasSuffix(string(current_dir), "NetworkOptimizer"))
		path = string(current_dir) + path;
	else if (hasSuffix(string(current_dir), "NetworkOptimizer/Debug"))
		path = string(current_dir) + "/.." + path;
	else if (hasSuffix(string(current_dir), "NetworkOptimizer/Debug-Optimized"))
		path = string(current_dir) + "/.." + path;
	else
		cout << "[misc] Path not found!" << endl;

	return path + "/use_case1.graphml";
}

string getPathToEventFile() {
	// read graph file
	char *current_dir = get_current_dir_name();
	string path("/input");
	if (hasSuffix(string(current_dir), "NetworkOptimizer"))
		path = string(current_dir) + path;
	else if (hasSuffix(string(current_dir), "NetworkOptimizer/Debug"))
		path = string(current_dir) + "/.." + path;
	else if (hasSuffix(string(current_dir), "NetworkOptimizer/Debug-Optimized"))
		path = string(current_dir) + "/.." + path;
	else
		cout << "[misc] Path not found!" << endl;

	return path + "/use_case1_events.json";
}
