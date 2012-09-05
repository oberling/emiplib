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
#include "miposcencoder.h"
#include "miposcmessage.h"

#include "mipdebug.h"

#define MIPOSCENCODER_ERRSTR_NOTINIT				"Not initialized"
#define MIPOSCENCODER_ERRSTR_ALREADYINIT			"Already initialized"
#define MIPOSCENCODER_ERRSTR_BADMESSAGE			"Only signed 16 bit native endian raw audio messages are accepted"

MIPOSCEncoder::MIPOSCEncoder() : MIPComponent("MIPOSCEncoder")
{
	m_init = false;
}

MIPOSCEncoder::~MIPOSCEncoder()
{
	destroy();
}

bool MIPOSCEncoder::init()
{
	if (m_init)
	{
		setErrorString(MIPOSCENCODER_ERRSTR_ALREADYINIT);
		return false;
	}

	m_prevIteration = -1;
	m_msgIt = m_messages.begin();
	m_init = true;

	return true;
}

bool MIPOSCEncoder::destroy()
{
	if (!m_init)
	{
		setErrorString(MIPOSCENCODER_ERRSTR_NOTINIT);
		return false;
	}

	clearMessages();
	m_init = false;

	return true;
}

bool MIPOSCEncoder::push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg)
{
	if (!m_init)
	{
		setErrorString(MIPOSCENCODER_ERRSTR_NOTINIT);
		return false;
	}

	if (iteration != m_prevIteration)
	{
		m_prevIteration = iteration;
		clearMessages();
	}

	if (!(pMsg->getMessageType() == MIPMESSAGE_TYPE_OSC) )
	{
		setErrorString(MIPOSCENCODER_ERRSTR_BADMESSAGE);
		return false;
	}

	MIPOSCMessage* pOSCMsg = (MIPOSCMessage*) pMsg;

	// here we could do some encoding and stuff
	// like history and stuff

	MIPOSCMessage* pNewMsg = new MIPOSCMessage(pOSCMsg->getMessageCopy(), pOSCMsg->getPath());
	pNewMsg->copyMediaInfoFrom(*pOSCMsg);

	m_messages.push_back(pNewMsg);

	m_msgIt = m_messages.begin();
	delete pOSCMsg;

	return true;
}

bool MIPOSCEncoder::pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg)
{
	if (!m_init)
	{
		setErrorString(MIPOSCENCODER_ERRSTR_NOTINIT);
		return false;
	}

	if (iteration != m_prevIteration)
	{
		m_prevIteration = iteration;
		clearMessages();
	}

	if (m_msgIt == m_messages.end())
	{
		// this is where it gets tricky - here we should perhaps invent a dummy-message
		*pMsg = 0;
		m_msgIt = m_messages.begin();
	}
	else
	{
		*pMsg = *m_msgIt;
		m_msgIt++;
	}

	return true;
}

void MIPOSCEncoder::clearMessages()
{
	std::list<MIPOSCMessage *>::iterator it;

	for (it = m_messages.begin() ; it != m_messages.end() ; it++)
		delete (*it);
	m_messages.clear();
	m_msgIt = m_messages.begin();
}

