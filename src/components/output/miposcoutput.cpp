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

#ifdef MIPCONFIG_SUPPORT_OSC

#include "miposcoutput.h"

#include "mipdebug.h"

#include <iostream>
#include <cstring>

#define MIPOSCOUTPUT_ERRSTR_PULLUNSUPPORTED			"Pull is not supported"
#define MIPOSCOUTPUT_ERRSTR_BADMESSAGE			"Only raw audio messages are supported"

MIPOSCOutput::MIPOSCOutput() : MIPComponent("MIPOSCOutput")
{
}

MIPOSCOutput::~MIPOSCOutput()
{
}

bool MIPOSCOutput::push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg)
{
	if (!(pMsg->getMessageType() == MIPMESSAGE_TYPE_OSC))
	{
		setErrorString(MIPOSCOUTPUT_ERRSTR_BADMESSAGE);
		return false;
	}

	MIPOSCMessage* oscMessage = (MIPOSCMessage*) pMsg;
	MIPOSCMessage* toQueue = new MIPOSCMessage(oscMessage->getMessageCopy(), strdup(oscMessage->getPath()));
	toQueue->copyMediaInfoFrom(*oscMessage);
	std::cout<<"pushing oscMessage to path "<<oscMessage->getPath()<<" came from source: "<<oscMessage->getSourceID()<<std::endl;
	m_messages.push(toQueue);
	return true;
}

bool MIPOSCOutput::pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg)
{
	setErrorString(MIPOSCOUTPUT_ERRSTR_PULLUNSUPPORTED);
	return false;
}

#endif // MIPCONFIG_SUPPORT_OSC
