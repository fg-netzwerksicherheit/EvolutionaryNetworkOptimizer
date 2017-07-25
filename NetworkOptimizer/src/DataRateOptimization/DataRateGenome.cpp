/*
 * NetworkGenome.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham
 */

#include "DataRateGenome.h"

using namespace std;

/**
 * \brief Default constructor to be used by MPI, required by ea::Storable
 */
DataRateGenome::DataRateGenome() {
}

/**
 * \see simul::P2PEdge
 */
DataRateGenome::DataRateGenome(vector<double> p_DataRateSource,
		vector<double> p_DataRateDestination) :
		DataRateSource(p_DataRateSource), DataRateDestination(p_DataRateDestination) {
}

DataRateGenome::~DataRateGenome() {

}

void DataRateGenome::setDataRateSrc(vector<double> v_DataRateSrc) {
	DataRateSource = v_DataRateSrc;
}

void DataRateGenome::setDataRateDst(vector<double> v_DataRateDst) {
	DataRateDestination = v_DataRateDst;
}

vector<double> DataRateGenome::getDataRateSrc() {
	return DataRateSource;
}

vector<double> DataRateGenome::getDataRateDst() {
	return DataRateDestination;
}

double DataRateGenome::getTotalDataRateSrc() {
	double sum = 0;
	for (auto del : DataRateSource)
		sum += del;
	return sum;
}

double DataRateGenome::getTotalDataRateDst() {
	double sum = 0;
	for (auto del : DataRateDestination)
		sum += del;
	return sum;
}

ostream& DataRateGenome::Print(ostream& os) const {
	uint size = DataRateSource.size();

	if (size == 0)
		os << "<Empty>";
	else
		os << "(" << DataRateSource[0] << ", " << DataRateDestination[0] << ")";
	for (uint i = 1; i < size; i++)
		os << "-(" << DataRateSource[i] << ", " << DataRateDestination[i] << ")";

	return os;
}

/**
 * \brief To pass data to other processes, required by ea::Storable
 */
void DataRateGenome::DoSerialize(std::ostream& pStream) const {
	Write(pStream, DataRateSource);
	Write(pStream, DataRateDestination);
}

/**
 * \brief To read data from another process, required by ea::Storable
 */
void DataRateGenome::DoDeserialize(std::istream& pStream) {
	DataRateSource = vector<double>(Read<std::vector<double>>(pStream));
	DataRateDestination = vector<double>(Read<std::vector<double>>(pStream));
}
