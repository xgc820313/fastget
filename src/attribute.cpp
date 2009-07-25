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
 * @author     dragon <chinagnu@gmail.com>
 * @version    0.3
 * @remarks    bug fixed for set_time_right when speed is zero
 *
 * @todo       unknow
 * @bug        unknow
 */

#include "attribute.h"

double Attribute::gettime(void)
{
	struct timeval time[1];
	gettimeofday( time, 0 );
	
	return( (double) time->tv_sec + (double) time->tv_usec / 1000000 );
}

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

bool Attribute::set_speed(const uint32_t total, const uint32_t finish_total)
{
	char *cspeed=NULL;
	uint32_t m_speed=(uint32_t)((double)(total - finish_total)/(gettime() - start_time));
	if(m_speed >= (MAX_STRING * MAX_STRING))
	{
		cspeed = g_strdup_printf("%d m/s",m_speed/MAX_STRING/MAX_STRING);
	}
	else if(m_speed >= MAX_STRING)
	{
		cspeed = g_strdup_printf("%d k/s",m_speed/MAX_STRING);
	}
	else
		cspeed = g_strdup_printf("%d b/s",m_speed);

	if(cspeed != NULL)
	{
		if(speed == NULL)
			speed = new string(cspeed);
		else
		{
			speed->clear();
			speed->append(cspeed);
		}
		return true;
	}

	return false;
}

bool Attribute::set_time_right(const uint32_t total,const uint32_t finish_total)
{
	uint32_t m_speed = (uint32_t)((double)(total - finish_total)/(gettime() - start_time));
	uint32_t m_time = 0;

	if(m_speed > 0)
		m_time = (uint32_t)((double) ( size - finish_total ) / m_speed );
	else
	{
		if(time_right != NULL)
			return false;
	}

	uint32_t h=0,m=0,s=0;
	if(m_time > 0)
	{
		h = m_time/3600;
		m = (m_time % 3600)/60;
		s = (m_time % 3600) % 60;
	}
	gchar *rtime = g_strdup_printf("%dh:%dm:%ds",h,m,s);
	
	if(rtime != NULL)
	{
		if(time_right == NULL)
		{
			time_right = new string(rtime);
		}
		else
		{
			time_right->clear();
			time_right->append(rtime);
		}
		return true;
	}

	return false;
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

bool Attribute::set_target_size(void)
{
	char *m_size=NULL;
	if(size >= (MAX_STRING * MAX_STRING * MAX_STRING))
	{
		m_size = g_strdup_printf("%0.2f G",(double)size/MAX_STRING/MAX_STRING/MAX_STRING);
	}
	else if(size >= (MAX_STRING * MAX_STRING))
	{
		m_size = g_strdup_printf("%0.2f M",(double)size/MAX_STRING/MAX_STRING);
	}
	else if(size >= MAX_STRING)
	{
		m_size = g_strdup_printf("%0.2f K",(double)size/MAX_STRING);
	}
	else
		m_size = g_strdup_printf("%d Bp",size);

	if(m_size != NULL)
	{
		if(target_size == NULL)
		{
			target_size = new string(m_size);
		}
		else
		{
			target_size->clear();
			target_size->append(m_size);
		}
		
		return true;
	}

	return false;
}

void Attribute::set_progress(const uint32_t finish_total)
{
	int m_progress = 0;
	if(size > 0)
	{
		m_progress = (finish_total/size)*100;
	}
	progress = m_progress;
}

bool Attribute::set_file_name(const char* filename)
{
	if(filename != NULL)
	{
		if(file_name == NULL)
		{
			file_name = new string(filename);
		}
		else
		{
			file_name->clear();
			file_name->append(filename);

		}
		return true;
	}
	else
	{
		if(target_name != NULL)
		{
			file_name = target_name;
			return true;
		}
	}

	return false;
}
