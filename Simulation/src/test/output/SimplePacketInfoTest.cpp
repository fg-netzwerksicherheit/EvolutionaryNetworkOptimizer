/*
 * SimplePacketInfoTest.cpp
 *
 *  Created on: May 4, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include "boost/test/unit_test.hpp"

#include "../../Simul.h"

using namespace simul;

BOOST_AUTO_TEST_CASE(SimplePacketInfoTest) {
	SimplePacketInfo simpleInfo;
	BOOST_REQUIRE_EQUAL(simpleInfo.GetType(), PACKET_UNKNOWN);

	// time test 1
	ns3::Time t1, t2;
	t1 = ns3::Time("2s");
	t2 = ns3::Time("100min");

	simpleInfo.SetSentTime(t1);
	simpleInfo.SetDeliveredTime(t2);
	BOOST_REQUIRE_EQUAL(simpleInfo.GetSentTime() - simpleInfo.GetDeliveredTime(), t1 - t2);
	BOOST_REQUIRE_EQUAL((t1 - t2).GetSeconds(), -5998);
	BOOST_REQUIRE_EQUAL(t2.GetSeconds() - t1.GetSeconds(), 5998);

	// time test 2
	t1 = ns3::Time("3ns");
	t2 = ns3::Time("2min");
	simpleInfo.SetSentTime(t1);
	simpleInfo.SetDeliveredTime(t2);

	SimplePacketInfo simpleInfo2;
	ns3::Time t3("2ns");
	simpleInfo2.SetSentTime(t3);
	simpleInfo2.SetDeliveredTime(t2);

	BOOST_REQUIRE_LT(119, (simpleInfo.GetDeliveredTime() - simpleInfo.GetSentTime()).GetSeconds());
	BOOST_REQUIRE_LT(simpleInfo.GetDeliveredTime() - simpleInfo.GetSentTime(),
			simpleInfo2.GetDeliveredTime() - simpleInfo2.GetSentTime());
	BOOST_REQUIRE_LT(
			simpleInfo2.GetDeliveredTime().GetSeconds() - simpleInfo2.GetSentTime().GetSeconds(),
			t2.GetSeconds());
	BOOST_REQUIRE_EQUAL(
			(simpleInfo.GetDeliveredTime() - simpleInfo.GetSentTime()).GetNanoSeconds()
					- (simpleInfo2.GetDeliveredTime() - simpleInfo2.GetSentTime()).GetNanoSeconds(),
			-1);
}

