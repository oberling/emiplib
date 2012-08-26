/*

  This file is a part of EMIPLIB, the EDM Media over IP Library.

  Copyright (C) 2006-2011  Hasselt University - Expertise Centre for
                      Digital Media (EDM) (http://www.edm.uhasselt.be)

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
  USA

*/

#include "mipconfig.h"
#include "miprtposcdecoder.h"
#include "miposcmessage.h"
#include "miprtpmessage.h"
#include <jrtplib3/rtppacket.h>

#include "mipdebug.h"

using namespace jrtplib;

MIPRTPOSCDecoder::MIPRTPOSCDecoder()
{
}

MIPRTPOSCDecoder::~MIPRTPOSCDecoder()
{
}

bool MIPRTPOSCDecoder::validatePacket(const RTPPacket *pRTPPack, real_t &timestampUnit, real_t timestampUnitEstimate)
{
	size_t length;

	length = (size_t)pRTPPack->GetPayloadLength();
	if (length < 1)
		return false;

	timestampUnit = 1.0/100.0;
	return true;
}

void MIPRTPOSCDecoder::createNewMessages(const RTPPacket *pRTPPack, std::list<MIPMediaMessage *> &messages, std::list<uint32_t> &timestamps)
{
	size_t length = pRTPPack->GetPayloadLength();
	uint8_t *pData = new uint8_t [length];
	char* path;

	memcpy(pData, pRTPPack->GetPayloadData(), length);
	path = (char*) pData;
	int lo_error_code;

	lo_message ploOSCMsg = lo_message_deserialise((void*) pData, length, &lo_error_code);

	MIPOSCMessage* pOSCMsg = new MIPOSCMessage(ploOSCMsg, path);

	messages.push_back(pOSCMsg);
	timestamps.push_back(pRTPPack->GetTimestamp());
}

