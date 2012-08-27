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
 * \file miposcmessage.h
 */

#ifndef MIPOSCMESSAGE_H

#define MIPOSCMESSAGE_H

#include "mipconfig.h"
#ifdef MIPCONFIG_SUPPORT_OSC

#include "mipmediamessage.h"

#include "lo/lo.h"

/** This class encodes OSC-Messages
 *  This class encodes OSC-Messages
 */
class EMIPLIB_IMPORTEXPORT MIPOSCMessage : public MIPMediaMessage
{
public:
/** Constructor of the class.
	 *  Constructor of the class, meant to be used by subclasses.
	 */
	MIPOSCMessage(uint32_t msgSubtype, lo_message msg, const char* path) : MIPMediaMessage(OSC, true, msgSubtype)
													{ m_msg = msg; m_path = path; }
	MIPOSCMessage(lo_message msg, const char* path) : MIPMediaMessage(OSC, true, MIPMESSAGE_TYPE_OSC)
													{ m_msg = msg; m_path = path; }

	~MIPOSCMessage()										{ lo_message_free(m_msg); }

	/** Returns the stored OSC Message */
	lo_message getMessage()									{ return m_msg; }
	lo_message getMessageCopy()	{return lo_message_copy(m_msg);	}

	const char* getPath()									{ return m_path; }

	/** Stores the msg */
	void setMessage(lo_message msg)							{ m_msg = msg; }

	/** Copies the info stored in \c msg, including the MIPMediaMessage info. */
	void copyOSCInfoFrom(MIPOSCMessage &msg)							{ copyMediaInfoFrom(msg); m_msg=msg.m_msg; m_path = msg.m_path; }
private:
	lo_message m_msg;
	const char* m_path;
};

#endif // MIPCONFIG_SUPPORT_OSC

#endif // MIPOSCMESSAGE_H

