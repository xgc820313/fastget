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
 * @file   protocol.h
 * @author dragon <chinagnu@gmail.com>
 * @date   Wed Jul  1 14:12:49 2009
 * 
 * @brief  virtual interface define
 * 
 * 
 */

#ifndef  PROTOCOL_H_INC
#define  PROTOCOL_H_INC

#include "conn.h"
#include "attribute.h"

class Protocol
{
public:
	Protocol(void){};
	~Protocol(void){};

	static Protocol* factory(const proto_t type);	
	bool get_target_resource(const Attribute& m_attr);
	virtual bool get_resource_info(const Attribute& m_attr){};

private:

};

class HTTP:public Protocol
{
public:
	bool get_resource_info(const Attribute& m_attr);
	void encode_request(const Attribute& m_attr);

private:

};

class FTP:public Protocol
{
public:
	bool get_resource_info(const Attribute& m_attr);

private:

};

class P2P:public Protocol
{
public:
	bool get_resource_info(const Attribute& m_attr);

private:

};

class MMS:public Protocol
{
public:
	bool get_resource_info(const Attribute& m_attr);

private:
	
};

#endif	/* define end */
