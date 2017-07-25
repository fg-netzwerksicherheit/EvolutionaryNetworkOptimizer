/*
 * IPacketInfoOutput.h
 *
 *  Created on: May 1, 2017
 *      Author: Kiet Vo
 */

#ifndef OUTPUT_IPACKETINFOOUTPUT_H_
#define OUTPUT_IPACKETINFOOUTPUT_H_

#include "../Si_Type.h"

namespace simul {

/**
 * \ingroup output
 *
 * \brief Interface for the implementation of a specific output system.
 */
class IPacketInfoOutput {
public:
	virtual ~IPacketInfoOutput() {
	}
	;

	virtual void ClearPacketInfoList() = 0;
	virtual VectorPtr<PacketInfoPtr> GetPacketInfoList() = 0;
};

} /* namespace simul */

#endif /* OUTPUT_IPACKETINFOOUTPUT_H_ */
