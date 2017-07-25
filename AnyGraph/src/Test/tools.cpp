/*
 * tools.cpp
 *
 *  Created on: Jun 28, 2017
 *      Author: Kiet Vo
 */

#include <string>
#include <unistd.h>
#include <iostream>
using namespace std;

bool static hasSuffix(const string &str, const string &suffix) {
	return str.size() >= suffix.size()
			&& str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

string GetProjectAnyGraphDirectory() {
	char *current_dir = get_current_dir_name();
	string path("");

	if (hasSuffix(string(current_dir), "AnyGraph"))
		path = string(current_dir);
	else if (hasSuffix(string(current_dir), "AnyGraph/Debug"))
		path = string(current_dir) + "/..";
	else {
		string msg = "[tools] Project AnyGraph's directory not found!";
		cout << msg << endl;
		throw domain_error(msg);
	}

	return path;
}


