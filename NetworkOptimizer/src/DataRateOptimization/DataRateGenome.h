/*
 * DataRateGenome.h
 *
 *  Created on: May 15, 2017
 *      Author: Quang Pham
 */

#ifndef DATARATEOPTIMIZATION_DATARATEGENOME_H_
#define DATARATEOPTIMIZATION_DATARATEGENOME_H_

#include <openea/EA.h>
#include <iostream>
#include <unistd.h>

using namespace ea;
/**
 * \defgroup datarate	Data-rate Optimization
 *
 * \brief Only works with p2pnetml file.
 */

/**
 * \ingroup datarate
 *
 * \brief Contains 2 vectors of data-rate values for source nodes and destination nodes.
 *
 * Each point-to-point connection (currently bi-directional) has a source node and a destination node.
 * Data-rate values should be of double type and in "Mbps" unit (e.g. 7.3 means "7.3 Mbps").
 *
 * \see simul::P2PEdge
 */
class DataRateGenome: public ea::GenomeClonable<DataRateGenome> {
public:
	EA_TYPEINFO_DEFAULT(DataRateGenome);

	DataRateGenome();
	DataRateGenome(std::vector<double> p_DataRateSource, std::vector<double> p_DataRateDestination);
	virtual ~DataRateGenome();

	std::vector<double> getDataRateSrc();
	std::vector<double> getDataRateDst();
	void setDataRateSrc(std::vector<double> v_DataRateSrc);
	void setDataRateDst(std::vector<double> v_DataRateDst);

	double getTotalDataRateSrc();
	double getTotalDataRateDst();

	virtual ostream& Print(ostream& os) const;

protected:

	virtual void DoSerialize(std::ostream& pStream) const override;
	virtual void DoDeserialize(std::istream& pStream) override;

private:
	std::vector<double> DataRateSource;
	std::vector<double> DataRateDestination;
};

REGISTER_ADDON(DataRateGenome)

#endif /* DATARATEOPTIMIZATION_DATARATEGENOME_H_ */
