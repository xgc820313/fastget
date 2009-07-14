/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/**
 * @file   protocol.cpp
 * @author dragon <chinagnu@gmail.com>
 * @date   Wed Jul  1 16:49:28 2009
 * 
 * @brief  protocol operation
 * 
 * 
 */

#include "protocol.h"

/** 
 * @desc create handle with factory mode
 * 
 * @param protocol handle
 * 
 * @return 
 */
Protocol* Protocol::factory(const proto_t type)
{
	switch(type)
	{
	case HTTP_PROTOCOL:
		return new HTTP();
	case FTP_PROTOCOL:
		return new FTP();
	case P2P_PROTOCOL:
		return new P2P();
	case MMS_PROTOCOL:
		return new MMS();
	default:
		return NULL;
	}
}

/** 
 * @desc get target file size
 * 
 * @param interge
 * 
 * @return target file size or zero
 */
bool Protocol::get_target_resource(const Attribute& m_attr)
{
	Protocol *m_proto = factory(m_attr.current_protocol);
	if(m_proto != NULL)
	{
		if(m_proto->get_resource_info(m_attr))
		{
			delete m_proto;
			return true;
		}
		else
			delete m_proto;
	}

	return false;
}

