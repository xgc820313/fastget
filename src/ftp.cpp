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
 * @file   ftp.cpp
 * @author dragon <chinagnu@gmail.com>
 * @date   Wed Jul  1 16:10:55 2009
 * 
 * @brief  operation with ftp transport protocal
 * 
 * 
 */

#include "protocol.h"

/** 
 * @brief get target resource information
 * 
 * @param m_attr 
 * 
 * @return 
 */
bool FTP::get_resource_info(const Attribute& m_attr)
{
	Conn m_conn;
	m_conn.set_timeout(7);	
	if(m_conn.open_conn(m_attr.get_hostname()->c_str(),m_attr.get_port()))
	{
		char *buf="hi, i just want to ...";
		int len = strlen(buf);

		if(m_conn.send_data(buf,len))
		{
			cout<<buf<<endl;
			cout<<"get resource "<<m_attr.get_target_name()->c_str()<<" from "<<m_attr.get_hostname()->c_str()<<endl;
			cout<<"with user name: "<<m_attr.get_user_name()->c_str()<<" password:"<<m_attr.get_password()->c_str()<<endl;

			char *buffer = new char[1024];
			if(m_conn.recv_data(buffer,1024)>0)
			{
				cout<<buffer<<endl;
			}
			delete buffer;

			return true;
		}
	}
	
	return false;
}
