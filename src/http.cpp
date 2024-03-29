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
 * @file   http.cpp
 * @author dragon <chinagnu@gmail.com>
 * @date   Wed Jul  1 16:12:56 2009
 * 
 * @brief  operation with http transport protocol
 * 
 * 
 */

#include "protocol.h"

bool HTTP::get_resource_info(const Attribute& m_attr)
{
	Conn m_conn;
	m_conn.set_timeout(7);	
	if(m_conn.open_conn(m_attr.get_hostname()->c_str(),m_attr.get_port()))
	{
		cout<<"good"<<endl;
	}
	else
		cout<<"get target file size with http protocol"<<endl;

	return true;
}

void HTTP::encode_request(const Attribute& m_attr)
{
	char *tmp = g_strdup_printf("GET %s%s HTTP/1.1", m_attr.get_dir()->c_str(),m_attr.get_file_name()->c_str());
	cout<<tmp<<endl;
	
}
