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

/**
 * \file miposcinput.h
 */
#ifndef MIPOSCINPUT_H

#define MIPOSCINPUT_H

#include "mipconfig.h"

#ifdef MIPCONFIG_SUPPORT_OSC

#include "mipcomponent.h"
#include "miposcmessage.h"
#include "miptime.h"
#include <string>
#include <list>

/** A sound file input component.
 *  This component can be used to read audio from a file. The component uses the
 *  \c libaudiofile library to accomplish this. This component accepts MIPSYSTEMMESSAGE_TYPE_ISTIME
 *  messages, so a timing component should sent its messages to the component. The
 *  output messages contain raw audio in 16 bit integer encoding.
 *  \note A frame consists of a number of samples (equal to the number of channels)
 *        sampled at the same instant. For example, in a stereo sound file, one
 *        frame would consist of two samples: one for the left channel and one for
 *        the right.
 */
class EMIPLIB_IMPORTEXPORT MIPOSCInput : public MIPComponent
{
public:
	MIPOSCInput();
	~MIPOSCInput();

	/** Selects which source ID will be set in outgoing osc messages. */
	void setSourceID(uint64_t sourceID)							{ m_sourceID = sourceID; }

	bool push(lo_message msg);

	bool push(const MIPComponentChain &chain, int64_t iteration, MIPMessage *pMsg);
	bool pull(const MIPComponentChain &chain, int64_t iteration, MIPMessage **pMsg);
private:
	bool init();
	bool destroy();
	void clearMessages();

	int64_t m_prevIteration;
	std::list<MIPOSCMessage *> m_messages;
	std::list<MIPOSCMessage *>::const_iterator m_msgIt;
	uint64_t m_sourceID;
};

#endif // MIPCONFIG_SUPPORT_OSC

#endif // MIPOSCINPUT_H

