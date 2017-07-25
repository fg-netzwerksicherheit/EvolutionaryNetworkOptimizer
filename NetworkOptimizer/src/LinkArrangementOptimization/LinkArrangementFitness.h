/*
 * LinkArrangementFitness.h
 *
 *  Created on: May 26, 2017
 *      Author: Kiet Vo
 */

#ifndef LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTFITNESS_H_
#define LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTFITNESS_H_

#include <openea/EA.h>

/**
 * \ingroup linkarrangement
 *
 * \brief Non-scalar fitness for a network instance.
 *
 * This type of fitness is a vector of 3 elements.
 * When two network instances are evaluated against each other, their fitness vectors will
 * be compared according to the criteria specified in this class.
 *
 \verbatim
 |	elements	|				definitions					|
 -------------------------------------------------------------
 |	data		|	number of successfully delivered bytes	|
 |	cost		|	cost of the whole network				|
 |	latency		|	average latency of data transfer		|
 \endverbatim
 *
 */
class LinkArrangementFitness: public ea::Fitness {
public:
	EA_TYPEINFO_CUSTOM_DECL;

	LinkArrangementFitness();
	LinkArrangementFitness(double budget, double cost, double data, double latency);
	virtual ~LinkArrangementFitness();

	double GetData();
	double GetCost();

	std::ostream& Print(std::ostream& os) const;
	virtual int Compare(const Fitness& other) const override;

protected:
	virtual void DoSerialize(std::ostream& pStream) const override;
	virtual void DoDeserialize(std::istream& pStream) override;

private:
	double budget = -1;
	double cost = -1;	// cost of this network
	double data = -1;// total bytes delivered successfully
	double latency = -1;// average latency of this network
};

REGISTER_ADDON(LinkArrangementFitness)

inline std::ostream& operator<<(std::ostream& os, const LinkArrangementFitness& fitness) {
	return fitness.Print(os);
}

#endif /* LINKARRANGEMENTOPTIMIZATION_LINKARRANGEMENTFITNESS_H_ */
