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
#include "miprtposcencoder.h"
#include "miprtpmessage.h"
#include "miposcmessage.h"

#include "mipdebug.h"

#define MIPRTPOSCENCODER_ERRSTR_BADMESSAGE		"Can't understand message"
#define MIPRTPOSCENCODER_ERRSTR_NOTINIT		"RTP A-law encoder not initialized"
#define MIPRTPOSCENCODER_ERRSTR_BADSAMPLINGRATE	"Only a sampling rate of 8kHz is allowed"
#define MIPRTPOSCENCODER_ERRSTR_BADCHANNELS		"Only mono audio is allowed"

MIPRTPOSCEncoder::MIPRTPOSCEncoder() : MIPRTPEncoder("MIPRTPOSCEncoder")
{
	m_init = false;
}

MIPRTPOSCEncoder::~MIPRTPOSCEncoder()
{
	cleanUp();
}

bool MIPRTPOSCEncoder::init()
{
	if (m_init)
		cleanUp();

	m_prevIteration = -1;
	m_init = true;
	return true;
}

bool MIPRTPOSCEncoder::push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg)
{
	if (!m_init)
	{
		setErrorString(MIPRTPOSCENCODER_ERRSTR_NOTINIT);
		return false;
	}

	if (m_prevIteration != iteration)
	{
		m_prevIteration = iteration;
		clearMessages();
	}

	if (!(pMsg->getMessageType() == MIPMESSAGE_TYPE_OSC))
	{
		setErrorString(MIPRTPOSCENCODER_ERRSTR_BADMESSAGE);
		return false;
	}

	MIPOSCMessage *pOSCMsg = (MIPOSCMessage *)pMsg;

	void* pData=NULL;
	size_t* size;
	lo_message_serialise(pOSCMsg->getMessage(), pOSCMsg->getPath(), pData, size);

	bool marker = false;
	uint8_t *pPayload = new uint8_t [*size];

	memcpy(pPayload,pData,*size);

	MIPRTPSendMessage *pNewMsg;

	// 100 must be replaced ... dunno right now by what (timestamp-Increment)
	pNewMsg = new MIPRTPSendMessage(pPayload,*size,getPayloadType(),marker,100);
	pNewMsg->setSamplingInstant(pOSCMsg->getTime());

	m_messages.push_back(pNewMsg);
	m_msgIt = m_messages.begin();

	return true;
}

bool MIPRTPOSCEncoder::pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg)
{
	if (!m_init)
	{
		setErrorString(MIPRTPOSCENCODER_ERRSTR_NOTINIT);
		return false;
	}

	if (m_prevIteration != iteration)
	{
		m_prevIteration = iteration;
		clearMessages();
	}

	if (m_msgIt == m_messages.end())
	{
		m_msgIt = m_messages.begin();
		*pMsg = 0;
	}
	else
	{
		*pMsg = *m_msgIt;
		m_msgIt++;
	}
	return true;
}

void MIPRTPOSCEncoder::cleanUp()
{
	clearMessages();
	m_init = false;
}

void MIPRTPOSCEncoder::clearMessages()
{
	std::list<MIPRTPSendMessage *>::iterator it;

	for (it = m_messages.begin() ; it != m_messages.end() ; it++)
		delete (*it);
	m_messages.clear();
	m_msgIt = m_messages.begin();
}

