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
 * @file       attribute.h
 * @author     dragon <chinagnu@gmail.com>
 * @date       Thu Jul  2 10:28:01 2009
 * 
 * @brief      header define of task attribute
 * @version    0.1
 * 
 * @todo       unknow
 * @bug        unknow
 */

#ifndef  ATTRIBUTE_H_INC
#define  ATTRIBUTE_H_INC

#include "dragon.h"

typedef enum
{
	UNKNOW_PROTOCOL,
	HTTP_PROTOCOL,
	FTP_PROTOCOL,
	MMS_PROTOCOL,
	P2P_PROTOCOL

}proto_t;

typedef enum
{
	START,
	WAIT,
	PAUSE,
	ERROR,
	CANCEL,
	FINISH

}status_t;	

class Attribute
{
private:

	status_t status;
	string *url;
	string *speed;
	string *progress;
	string *time_right;
	string *target_size;
	int retry;
	string *file_name;

	string *hostname;
	string *dir;
	string *target_name;

	string *user;
	string *passwd;

	unsigned int port;
	uint32_t size;

	int thread_num;
	bool auto_falg;
	double start_time;


	bool url_parse(void);
	bool meta_file_parse(void);	/* parse the bitorrent file  */
	string resource;			/* get all information from Bit torrent file */

public:
	Attribute(void)
	{
		url=0;
		speed=0;
		progress=0;
		time_right=0;
		target_size=0;
		file_name=0;
		hostname=0;
		dir=0;
		target_name=0;
		user=0;
		passwd=0;		
	}
	~Attribute(void)
	{
		delete url;
		delete speed;
		delete progress;
		delete time_right;
		delete target_size;
		delete file_name;
		delete hostname;
		delete dir;
		delete target_name;
		delete user;
		delete passwd;
	}

	/* attribute setting operation */
	uint32_t get_size(void) const {return size;}
	void set_size(const uint32_t file_size){size = file_size;}
	status_t get_status(void) const {return status;}
	bool set_status(status_t m_status);
	string* get_url(void) const { return url;}
	bool set_url(const char* m_url);
	string* get_hostname(void) const { return hostname;}
	string* get_dir(void) const { return dir;}
	string* get_target_name(void) const {return target_name;}
	unsigned int get_port(void) const { return port;}
	string* get_user_name(void) const { return user;}
	bool set_user_name(const char* m_user);
	string* get_password(void) const { return passwd;}	
	bool set_password(const char* pass);
	int get_thread_num(void) const { return thread_num;}
	bool set_thread_num(int num);
	string* get_file_name(void) const { return file_name;}
	string* get_speed(void) const { return speed;}
	string* get_progress(void) const { return progress;}
	string* get_time_right(void) const { return time_right;}

	/* parse operation */
	bool resource_parse(void);

	proto_t current_protocol;
};

class P2PAttribute:public Attribute
{
private:

public:
	/* parse the url  */
	bool url_parse(const string& filename);
};

#endif	/*  */
