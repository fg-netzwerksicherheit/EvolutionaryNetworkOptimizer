/*
 * tools.h
 *
 *  Created on: May 22, 2017
 *      Author: kiet
 */

#ifndef TOOLS_TOOLS_H_
#define TOOLS_TOOLS_H_

#include <vector>
#include <string>

std::string GetProjectNetOptDirectory();

std::vector<std::vector<double>> WholeArithmeticCrossover(std::vector<double> parent1,
		std::vector<double> parent2, double alpha);

#endif /* TOOLS_TOOLS_H_ */
