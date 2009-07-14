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
 * @file       attribute.cpp
 * @author     dragon <chinagnu@gmail.com>
 * @date       Thu Jul  2 11:30:24 2009
 * 
 * @brief      attribute operation
 * @version    0.1
 * 
 * @author     dragon <chinagnu@gmail.com>
 * @version    0.2
 * @remarks    add url parse of resource parse
 *
 * @todo       unknow
 * @bug        unknow
 */

#include "attribute.h"

bool Attribute::url_parse(void)
{
	if(url != NULL && !url->empty())
	{
		if(url->find("://")==url->npos)
			return false;

		if(url->find("http://") != url->npos || url->find("https://") != url->npos)
		{
			current_protocol = HTTP_PROTOCOL;
			
			unsigned int start_pos, host_name_pos;
			string host_name;
			if((start_pos = url->find_first_of("http://")) !=url->npos)
			{
				host_name_pos = url->find_first_of('/',start_pos+7);
				host_name = url->substr(start_pos+7,host_name_pos-(start_pos+7));
			}
			else
			{
				start_pos = url->find_first_of("https://");
				host_name_pos = url->find_first_of('/',start_pos+8);
				host_name = url->substr(start_pos+8,host_name_pos-(start_pos+8));
			}

			if(hostname != NULL)
			{
				if(!hostname->empty())
					hostname->clear();
				hostname->append(host_name);
			}
			else
				hostname = new string(host_name);


			unsigned int dir_pos;
			string dir_name,target;
			if((dir_pos=url->find_last_of('/'),host_name_pos+1) == url->npos)
			{
				dir = new string("/");
				target = url->substr(host_name_pos+1);
			}
			else
			{
				dir_name = url->substr(host_name_pos,dir_pos-host_name_pos+1);
				dir = new string(dir_name);
				target = url->substr(dir_pos+1);				
			}
			
			if(target_name != NULL)
			{
				if(!target_name->empty())
					target_name->clear();
				
				if(!target.empty())
					target_name->append(target);
				else
					target_name->append("index.html");
			}
			else
			{
				if(!target.empty())
					target_name = new string(target);
				else
					target_name = new string("index.html");
			}

#ifdef __DEBUG__
			cout<<url->c_str()<<endl;
			cout<<hostname->c_str()<<endl;
			cout<<dir->c_str()<<endl;
			cout<<target_name->c_str()<<endl;
#endif

			unsigned int port_pos;
			string port_num;
			if((port_pos = hostname->find(':')) != hostname->npos)
			{
				port_num = hostname->substr(port_pos+1);
				port = atoi(port_num.c_str());
			}
			else
			{
				port = 80;
			}

#ifdef __DEBUG__
			cout<<port<<endl;
#endif
			
		}
		else if(url->find("ftp://") != url->npos || url->find("ftps://") != url->npos)
		{
			current_protocol = FTP_PROTOCOL;

			unsigned int start_pos, host_name_pos;
			string host_name;
			if((start_pos = url->find_first_of("ftp://")) !=url->npos)
			{
				host_name_pos = url->find_first_of('/',start_pos+6);
				host_name = url->substr(start_pos+6,host_name_pos-(start_pos+6));
			}
			else
			{
				start_pos = url->find_first_of("ftp://");
				host_name_pos = url->find_first_of('/',start_pos+7);
				host_name = url->substr(start_pos+7,host_name_pos-(start_pos+7));
			}

			if(hostname != NULL)
			{
				if(!hostname->empty())
					hostname->clear();
				hostname->append(host_name);
			}
			else
				hostname = new string(host_name);


			unsigned int dir_pos;
			string dir_name,target;
			if((dir_pos=url->find_last_of('/'),host_name_pos+1) == url->npos)
			{
				dir = new string();
				target = url->substr(host_name_pos+1);
			}
			else
			{
				dir_name = url->substr(host_name_pos+1,dir_pos-host_name_pos);
				dir = new string(dir_name);
				target = url->substr(dir_pos+1);				
			}
			
			if(target_name != NULL)
			{
				if(!target_name->empty())
					target_name->clear();
				
				if(!target.empty())
					target_name->append(target);
				else
					target_name->append("index.html");
			}
			else
			{
				if(!target.empty())
					target_name = new string(target);
				else
					target_name = new string("index.html");
			}

#ifdef __DEBUG__
			cout<<url->c_str()<<endl;
			cout<<hostname->c_str()<<endl;
			cout<<dir->c_str()<<endl;
			cout<<target_name->c_str()<<endl;
#endif

			unsigned int port_pos;
			string port_num;
			if((port_pos = hostname->find(':')) != hostname->npos)
			{
				port_num = hostname->substr(port_pos+1);
				port = atoi(port_num.c_str());
			}
			else
			{
				port = 21;
			}

#ifdef __DEBUG__
			cout<<port<<endl;
#endif

			if(user == NULL && passwd == NULL)
			{
				user = new string("anonymous");
				passwd = new string("i love dragon");
			}

#ifdef __DEBUG__
			cout<<user->c_str()<<endl;
			cout<<passwd->c_str()<<endl;
#endif
			
		}
		else if( url->find("mms://") != url->npos)
		{
			current_protocol = MMS_PROTOCOL;


		}
		else
		{
			current_protocol = UNKNOW_PROTOCOL;
		}

		return true;
	}

	return false;
}

bool Attribute::meta_file_parse(void)
{
	
	return false;
}

bool Attribute::resource_parse(void)
{
	if(resource.empty())
		url_parse();
 	else
 		meta_file_parse();

	return true;
}




bool Attribute::set_url(const char* m_url)
{
	if(m_url != NULL)
	{
		if(url == NULL)
		{
			url = new string(m_url);
		}
		else
		{
			url->clear();
			url->append(m_url);
		}

		return true;
	}

	return false;
}

bool Attribute::set_user_name(const char* m_user)
{
	if(m_user != NULL)
	{
		if(user == NULL)
		{
			user = new string(m_user);
		}
		else
		{
			user->clear();
			user->append(m_user);
		}

		return true;
	}
	return false;
}

bool Attribute::set_password(const char* pass)
{
	if(pass != NULL )
	{
		if(passwd == NULL)
		{
			passwd = new string(pass);
		}
		else
		{
			passwd->clear();
			passwd->append(pass);
		}

		return true;
	}
	return false;
}

bool Attribute::set_status(status_t m_status)
{
	if(m_status >= START && m_status <= FINISH)
	{
		status = m_status;
		return true;
	}

	return false;
}

bool Attribute::set_thread_num(int num)
{
	if(num >= 1)
	{
		thread_num = num;
		return true;
	}
	
	return false;
}
