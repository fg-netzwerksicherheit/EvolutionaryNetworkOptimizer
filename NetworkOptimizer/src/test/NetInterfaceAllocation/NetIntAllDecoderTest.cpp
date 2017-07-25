/*
 * NetIntAllDecoderTest.cpp
 *
 *  Created on: Jun 14, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../NetInterfaceAllocation/NetIntAllDecoder.h"
#include <chrono>

using namespace std;
using namespace anygraph;

struct NetIntAllDecoderFixture {

	void TestConstructor(vector<uint> interfacesInComputer) {
		random_shuffle(interfacesInComputer.begin(), interfacesInComputer.end());

		shared_ptr<anygraph::Topology> topo = make_shared<anygraph::Topology>();

		vector<AnyNode> deviceList;
		for (uint c_Index = 0; c_Index < interfacesInComputer.size(); c_Index++) {
			anygraph::Attributes computer;
			computer.set_name("Computer");
			computer.set_data(to_string(c_Index));
			for (uint d_Index = 0; d_Index < interfacesInComputer[c_Index]; d_Index++) {
				anygraph::AnyNode device;
				device.add_attributes(computer);
				deviceList.push_back(device);
			}
		}

		for (auto device : deviceList)
			topo->add_node(device);

		NetIntAllDecoder decoder(topo);

		// check numValidDevices
		uint numValidInterfaces = 0;
		for (uint numLocalInterfaces : interfacesInComputer)
			numValidInterfaces += numLocalInterfaces;

		BOOST_CHECK_EQUAL(numValidInterfaces, decoder.GetNumInterfaces());

		// check possible-values list's range
		BOOST_CHECK_EQUAL(decoder.GetPossibleValues().size(), numValidInterfaces);

		// check possible values
		int sumPossibleValues_1 = 0;
		for (vector<int> p1 : decoder.GetPossibleValues())
			for (int p2 : p1)
				sumPossibleValues_1 += p2;

		int sumPossibleValues_2 = 0;
		int lowerInvalid = 0, upperInvalid = -1;
		for (uint numLocalInterfaces : interfacesInComputer) {
			lowerInvalid = upperInvalid + 1;
			upperInvalid = lowerInvalid + numLocalInterfaces - 1;

			int localPossible = 0;
			localPossible += lowerInvalid * (lowerInvalid - 1) / 2;
			localPossible += (numValidInterfaces - 1 + upperInvalid + 1)
					* (numValidInterfaces - 1 - upperInvalid) / 2;

			sumPossibleValues_2 += localPossible * numLocalInterfaces;
		}

		BOOST_CHECK_EQUAL(sumPossibleValues_1, sumPossibleValues_2);
	}
};

//XXX: this test is fragile because the order of a vector's elements may change

BOOST_FIXTURE_TEST_CASE(NetIntAllDecoderConstructorTest, NetIntAllDecoderFixture) {
	// random generator
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);

	// no comp
	TestConstructor(vector<uint>());

	// 1 comp
	vector<uint> interfacesInComputer(1);
	uniform_int_distribution<uint> dist(0, 3);
	interfacesInComputer[0] = dist(generator);
	TestConstructor(interfacesInComputer);

	// 2 comps
	interfacesInComputer = vector<uint>(2);
	dist = uniform_int_distribution<uint>(0, 3);
	interfacesInComputer[0] = dist(generator);
	interfacesInComputer[1] = dist(generator);
	TestConstructor(interfacesInComputer);

	// more comps
	for (uint testcount = 0; testcount < 4; testcount++) {
		dist = uniform_int_distribution<uint>(1, 100);
		uint numComps = dist(generator);
		interfacesInComputer = vector<uint>(numComps);

		dist = uniform_int_distribution<uint>(0, 1);
		for (uint i = 0; i < numComps; i++)
			interfacesInComputer[i] = dist(generator);

		TestConstructor(interfacesInComputer);
	}

	for (uint testcount = 0; testcount < 4; testcount++) {
		dist = uniform_int_distribution<uint>(1, 100);
		uint numComps = dist(generator);
		interfacesInComputer = vector<uint>(numComps);

		dist = uniform_int_distribution<uint>(1, 20);
		for (uint i = 0; i < numComps; i++)
			interfacesInComputer[i] = dist(generator);

		TestConstructor(interfacesInComputer);
	}
}

string GetProjectNetOptDirectory();

using namespace simul;

BOOST_AUTO_TEST_CASE(NetIntAllDecoderExceptionTest) {
	string path = GetProjectNetOptDirectory()
			+ "/../Simulation/src/test/input/graphfiles/use_case4a.graphml";

	NetIntAllDecoder decoder(path);

	// scenario 0
	vector<int> invalidGenotype(7);
	BOOST_CHECK_THROW(decoder.Decode(invalidGenotype), invalid_argument);

	invalidGenotype = vector<int>(5);
	BOOST_CHECK_THROW(decoder.ComputeCost(invalidGenotype), invalid_argument);
}

BOOST_AUTO_TEST_CASE(NetIntAllDecoderDecodingTest) {
	string path = GetProjectNetOptDirectory()
			+ "/../Simulation/src/test/input/graphfiles/use_case4a.graphml";

	NetIntAllDecoder decoder(path);

	// scenario 1
	vector<int> genotype(6);
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = 1;
	genotype[3] = 4, genotype[4] = 3;

	SetPtr<EdgePtr> edgeList = decoder.Decode(genotype);
	BOOST_CHECK_EQUAL(edgeList->size(), 3);

	SimulationPtr si = make_shared<Simulation>();
	si->AddEdges(edgeList);
	si->SetUpNetworks();

	weak_ptr<Event> event = si->CreateEvent<UdpTestEvent>(si->GetNode("comp2"),
			si->GetNode("comp3"));
	event.lock()->SetStartTime(0.1);

	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);
	si->Run();
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), true);

	// scenario 2
	genotype[0] = 2, genotype[2] = 1;
	genotype[1] = 2, genotype[5] = 2;
	genotype[3] = 4, genotype[4] = 5;

	edgeList = decoder.Decode(genotype);
	BOOST_CHECK_EQUAL(edgeList->size(), 1);

	si = make_shared<Simulation>();
	si->AddEdges(edgeList);
	si->SetUpNetworks();

	event = si->CreateEvent<UdpTestEvent>(si->GetNode("comp1"), si->GetNode("comp0"));
	event.lock()->SetStartTime(0.1);

	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);
	si->Run();
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), true);

	// scenario 3
	genotype[0] = 1, genotype[2] = 1;
	genotype[1] = 0, genotype[5] = 0;
	genotype[3] = 4, genotype[4] = 5;
	BOOST_CHECK_EQUAL(decoder.Decode(genotype)->size(), 0);

	// scenario 4a
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = -1;
	genotype[3] = -1, genotype[4] = -1;

	edgeList = decoder.Decode(genotype);
	BOOST_CHECK_EQUAL(edgeList->size(), 1);

	si = make_shared<Simulation>();
	si->AddEdges(edgeList);
	si->SetUpNetworks();

	event = si->CreateEvent<UdpTestEvent>(si->GetNode("comp0"), si->GetNode("comp1"));
	event.lock()->SetStartTime(0.1);

	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);
	si->Run();
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), true);

	// scenario 4b
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = -1;
	genotype[3] = -11, genotype[4] = -12;

	edgeList = decoder.Decode(genotype);
	BOOST_CHECK_EQUAL(edgeList->size(), 1);

	si = make_shared<Simulation>();
	si->AddEdges(edgeList);
	si->SetUpNetworks();

	event = si->CreateEvent<UdpTestEvent>(si->GetNode("comp0"), si->GetNode("comp1"));
	event.lock()->SetStartTime(0.1);

	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), false);
	si->Run();
	BOOST_CHECK_EQUAL(event.lock()->IsSuccessful(), true);
}

BOOST_AUTO_TEST_CASE(NetIntAllDecoderCostTest) {
	string path = GetProjectNetOptDirectory()
			+ "/../Simulation/src/test/input/graphfiles/use_case4a.graphml";

	NetIntAllDecoder decoder(path);

	// scenario 1
	vector<int> genotype(6);
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = 1;
	genotype[3] = 4, genotype[4] = 3;
	BOOST_CHECK_EQUAL(decoder.ComputeCost(genotype), 11);

	// scenario 2
	genotype[0] = 2, genotype[2] = 5;
	genotype[1] = 2, genotype[5] = 2;
	genotype[3] = 4, genotype[4] = 5;
	BOOST_CHECK_EQUAL(decoder.ComputeCost(genotype), 3);

	// scenario 3
	genotype[0] = 1, genotype[2] = 1;
	genotype[1] = 0, genotype[5] = 0;
	genotype[3] = 4, genotype[4] = 5;
	BOOST_CHECK_EQUAL(decoder.ComputeCost(genotype), 0);

	// scenario 4a
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = -1;
	genotype[3] = -1, genotype[4] = -1;
	BOOST_CHECK_EQUAL(decoder.ComputeCost(genotype), 4);

	// scenario 4b
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = -12;
	genotype[3] = -21, genotype[4] = -2;
	BOOST_CHECK_EQUAL(decoder.ComputeCost(genotype), 4);
}

BOOST_AUTO_TEST_CASE(NetIntAllDecoderViolationTest) {
	string path = GetProjectNetOptDirectory()
			+ "/../Simulation/src/test/input/graphfiles/use_case4a.graphml";

	NetIntAllDecoder decoder(path);

	// scenario 1
	vector<int> genotype(6);
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = 1;
	genotype[3] = 4, genotype[4] = 3;
	BOOST_CHECK_EQUAL(decoder.GetNumViolatedInterfaces(genotype), 0);

	// scenario 2
	genotype[0] = 2, genotype[2] = 5;
	genotype[1] = 2, genotype[5] = 2;
	genotype[3] = 4, genotype[4] = 5;
	BOOST_CHECK_EQUAL(decoder.GetNumViolatedInterfaces(genotype), 4);

	// scenario 3
	genotype[0] = 1, genotype[2] = 1;
	genotype[1] = 0, genotype[5] = 0;
	genotype[3] = 4, genotype[4] = 5;
	BOOST_CHECK_EQUAL(decoder.GetNumViolatedInterfaces(genotype), 6);

	// scenario 4a
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = -1;
	genotype[3] = -1, genotype[4] = -1;
	BOOST_CHECK_EQUAL(decoder.GetNumViolatedInterfaces(genotype), 1);

	// scenario 4b
	genotype[0] = 2, genotype[2] = 0;
	genotype[1] = 5, genotype[5] = -222;
	genotype[3] = -222, genotype[4] = -222;
	BOOST_CHECK_EQUAL(decoder.GetNumViolatedInterfaces(genotype), 1);
}

