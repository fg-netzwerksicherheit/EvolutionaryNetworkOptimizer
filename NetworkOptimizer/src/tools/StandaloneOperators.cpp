/*
 * StandaloneOperators.cpp
 *
 *  Created on: May 16, 2017
 *      Author: Kiet Vo
 */

#include <stdexcept>
#include <vector>
using namespace std;

template<class T>
static vector<vector<T>> WholeArithmeticCrossover(vector<T> parent1,
		vector<T> parent2, double alpha) {

	if (parent1.size() == 0 || parent1.size() != parent2.size())
		throw invalid_argument("Invalid parent size(s)!");
	if (alpha < 0 || alpha > 1)
		throw invalid_argument("Invalid alpha!");

	vector<vector<T>> children(2);
	for (size_t i = 0; i < parent1.size(); i++) {
		children[0].push_back(alpha * parent1[i] + (1 - alpha) * parent2[i]);
		children[1].push_back(alpha * parent2[i] + (1 - alpha) * parent1[i]);
	}

	return children;
}

vector<vector<double>> WholeArithmeticCrossover(vector<double> parent1,
		vector<double> parent2, double alpha) {

	return WholeArithmeticCrossover<double>(parent1, parent2, alpha);
}

