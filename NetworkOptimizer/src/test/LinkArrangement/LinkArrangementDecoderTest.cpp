/*
 * LinkArrangementDecoderTest.cpp
 *
 *  Created on: Jun 27, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../LinkArrangementOptimization/LinkArrangementDecoder.h"
#include <chrono>

using namespace std;
using namespace simul;

struct LinkArrangementDecoderFixture {
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator = default_random_engine(seed);
	uniform_int_distribution<int> dist = uniform_int_distribution<int>(0, LA_DEC_CB_LENGTH - 1);

	void TestDecodingNRun(vector<int> genes, int srcID = 0, int dstID = 0, bool allconnected =
			true) {
		uint numNodes = static_cast<uint>(sqrt(2 * genes.size())) + 1;

		Simulation si;
		si.AddEdges(LinkArrangementDecoder().DecodeToEdges(genes));
		si.SetUpNetworks();

		vector<weak_ptr<Event>> events;

		if (allconnected) {
			events = vector<weak_ptr<Event>>(numNodes);
			for (uint i = 0; i < numNodes - 1; i++) {
				events[i] = si.CreateEvent<UdpTestEvent>(si.GetNode("n" + to_string(i)),
						si.GetNode("n" + to_string(i + 1)));
				events[i].lock()->SetStartTime(0.1);
			}
			events[numNodes - 1] = si.CreateEvent<UdpTestEvent>(
					si.GetNode("n" + to_string(numNodes - 1)), si.GetNode("n0"));
			events[numNodes - 1].lock()->SetStartTime(0.1);
		}

		else {
			weak_ptr<Event> event = si.CreateEvent<UdpTestEvent>(si.GetNode("n" + to_string(srcID)),
					si.GetNode("n" + to_string(dstID)));
			event.lock()->SetStartTime(0.1);
			events.push_back(event);
		}

		//
		bool before = false;
		for (auto event : events)
			before = before || event.lock()->IsSuccessful();

		si.Run();

		bool after = true;
		for (auto event : events)
			after = after && event.lock()->IsSuccessful();

		BOOST_CHECK_GE(events.size(), 1);
		BOOST_CHECK_EQUAL(before, false);
		BOOST_CHECK_EQUAL(after, true);
	}

	void TestDecoding(vector<int> genes, uint numEdges = 0, bool checkNumEdges = false) {
		uint64_t sumdr1 = 0;
		ns3::Time sumig1("0s");

		for (uint gene : genes) {
			switch (gene) {
			case 1:
				sumdr1 += ns3::DataRate("10Mbps").GetBitRate();
				sumig1 += ns3::Time("9.6us");
				break;
			case 2:
				sumdr1 += ns3::DataRate("100Mbps").GetBitRate();
				sumig1 += ns3::Time("0.96us");
				break;
			case 3:
				sumdr1 += ns3::DataRate("1Gbps").GetBitRate();
				sumig1 += ns3::Time("96ns");
				break;
			case 4:
				sumdr1 += ns3::DataRate("2.5Gbps").GetBitRate();
				sumig1 += ns3::Time("38.4ns");
				break;
			case 5:
				sumdr1 += ns3::DataRate("5Gbps").GetBitRate();
				sumig1 += ns3::Time("19.2ns");
				break;
			case 6:
				sumdr1 += ns3::DataRate("10Gbps").GetBitRate();
				sumig1 += ns3::Time("9.6ns");
				break;
			case 7:
				sumdr1 += ns3::DataRate("40Gbps").GetBitRate();
				sumig1 += ns3::Time("2.4ns");
				break;
			case 8:
				sumdr1 += ns3::DataRate("100Gbps").GetBitRate();
				sumig1 += ns3::Time("0.96ns");
				break;
			}
		}

		uint64_t sumdr2 = 0;
		ns3::Time sumig2("0s");

		LinkArrangementDecoder decoder;
		SetPtr<EdgePtr> edgeList = decoder.DecodeToEdges(genes);

		for (EdgePtr edge : *edgeList) {
			P2PEdgePtr p2pedge = dynamic_pointer_cast<P2PEdge>(edge);
			sumdr2 += ns3::DataRate(p2pedge->GetDataRateSrc()).GetBitRate();
			sumig2 += ns3::Time(p2pedge->GetInterframeGapSrc());
		}

		BOOST_CHECK_EQUAL(sumdr1, sumdr2);
		BOOST_CHECK(sumig1 == sumig2);

		if (checkNumEdges)
			BOOST_CHECK_EQUAL(edgeList->size(), numEdges);
	}

	void TestRandomDecoding(uint numNodes) {
		uint length = numNodes * (numNodes - 1) / 2;

		vector<int> genes(length);
		for (uint i = 0; i < length; i++)
			genes[i] = dist(generator);

		TestDecoding(genes);
	}
};

BOOST_FIXTURE_TEST_CASE(LinkArrangementDecoderDecodingTest01, LinkArrangementDecoderFixture) {
	TestRandomDecoding(2);
	TestRandomDecoding(23);
	TestRandomDecoding(117);

	//
	vector<int> genes(6, 0);
	genes[2] = 7;
	TestDecoding(genes, 1, true);

	//
	genes[0] = 8, genes[1] = 2, genes[2] = 4, genes[3] = 3, genes[4] = 6, genes[5] = 7;
	TestDecoding(genes, 6, true);
}

BOOST_FIXTURE_TEST_CASE(LinkArrangementDecoderDecodingTest02, LinkArrangementDecoderFixture) {
	uint length = 6;
	vector<int> genes(length, 1);
	TestDecodingNRun(genes);

	//
	genes[0] = 2, genes[3] = 5, genes[5] = 7;
	TestDecodingNRun(genes, 0, 3, false);

	//
	genes = vector<int>(1, 6);
	TestDecodingNRun(genes);
}

BOOST_FIXTURE_TEST_CASE(LinkArrangementDecoderDecodingTest03, LinkArrangementDecoderFixture) {
	uint numNodes = 5;
	uint length = numNodes * (numNodes - 1) / 2;
	vector<int> genes(length, 0);

	TopologyPtr topo = LinkArrangementDecoder().DecodeToTopology(genes);
	BOOST_CHECK_EQUAL(topo->get_edge().size(), 0);
	BOOST_CHECK_EQUAL(topo->get_node().size(), numNodes);

	//
	genes[2] = 8, genes[7] = 3;
	topo = LinkArrangementDecoder().DecodeToTopology(genes);
	BOOST_CHECK_EQUAL(topo->get_edge().size(), 2);
	BOOST_CHECK_EQUAL(topo->get_node().size(), numNodes);
}


