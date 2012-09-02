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
	m_sourceID = 0;
	return true;
}

bool MIPOSCInput::destroy() {
	std::queue<MIPOSCMessage *> empty;
	std::swap(empty, m_messages);
	return true;
}

bool MIPOSCInput::push(lo_message msg, const char* path) {
	MIPOSCMessage* pNewMsg = new MIPOSCMessage(lo_message_copy(msg), path);
	m_messages.push(pNewMsg);
	return true;
}

bool MIPOSCInput::push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg)
{
	if (!(pMsg->getMessageType() == MIPMESSAGE_TYPE_SYSTEM && pMsg->getMessageSubtype() == MIPSYSTEMMESSAGE_TYPE_ISTIME))
	{
		setErrorString(MIPOSCINPUT_ERRSTR_BADMESSAGE);
		return false;
	}

	return true;
}

bool MIPOSCInput::pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg)
{
	if(!m_messages.empty()) {
		*pMsg = m_messages.front();
		m_messages.pop();
	} else {
		*pMsg = 0;
	}
	return true;
}

#endif // MIPCONFIG_SUPPORT_OSC

