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

#include "miposcinput.h"
#include "mipsystemmessage.h"

#include "mipdebug.h"

#define MIPOSCINPUT_ERRSTR_BADMESSAGE			"Message is not a timing event"

MIPOSCInput::MIPOSCInput() : MIPComponent("MIPOSCInput")
{
	init();
}

MIPOSCInput::~MIPOSCInput()
{
	destroy();
}

bool MIPOSCInput::init() {
	m_msgIt = m_messages.begin();
	m_sourceID = 0;
	return true;
}

bool MIPOSCInput::destroy() {
	clearMessages();
	return true;
}

bool MIPOSCInput::push(lo_message msg) {
	MIPOSCMessage* pNewMsg = new MIPOSCMessage(msg);
	m_messages.push_back(pNewMsg);
	return true;
}

bool MIPOSCInput::push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg)
{
	if(iteration != m_prevIteration)
	{
		m_prevIteration = iteration;
		clearMessages();
	}

	if (!(pMsg->getMessageType() == MIPMESSAGE_TYPE_SYSTEM && pMsg->getMessageSubtype() == MIPSYSTEMMESSAGE_TYPE_ISTIME))
	{
		setErrorString(MIPOSCINPUT_ERRSTR_BADMESSAGE);
		return false;
	}

	return true;
}

bool MIPOSCInput::pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg)
{
	if(iteration != m_prevIteration)
	{
		m_prevIteration = iteration;
		clearMessages();
	}

	if(m_msgIt == m_messages.end()) {
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

void MIPOSCInput::clearMessages()
{
	std::list<MIPOSCMessage *>::iterator it;

	for (it = m_messages.begin() ; it != m_messages.end() ; it++)
		delete (*it);
	m_messages.clear();
	m_msgIt = m_messages.begin();
}

#endif // MIPCONFIG_SUPPORT_OSC

