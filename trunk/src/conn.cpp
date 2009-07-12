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
 * @file       conn.cpp
 *
 * @author     Dragon, Fong's National Engineering Co. Ltd
 * @date       Thu Jul  9 11:09:17 2009
 * @version    0.1
 * @brief      network socket leave operation
 *
 * @author     dragon, <chinagnu@gmail.com>
 * @date       Thu Jul  10 15:20:14 2009
 * @version    0.2
 * @brief      add time out for send and recv operation
 *
 * @bug        unknown
 * @todo       unknown
 */

#include "conn.h"

/** 
 * @brief set time out with second for send/recived data
 * 
 * @param nsec 
 * 
 * @return 
 */
void Conn::set_timeout(const unsigned int nsec)
{
    if ( nsec <= 5 || nsec > 300 )
		m_timeout = 5;
	else
		m_timeout = nsec;
}

/** 
 * @brief create source connect with target hostname and port
 * 
 * @param hostname 
 * @param port 
 * 
 * @return 
 */
bool Conn::open_conn(const char *hostname, const int port)
{
	struct hostent *host = NULL;
	struct sockaddr_in addr;
//	struct sockaddr_in local;
	int fd;

	for( fd = 0; fd < 5; fd ++ )
	{
		if( ( host = gethostbyname( hostname ) ) == NULL )
		{
            sockfd = -1;
			break;
		}
		if( *host->h_name ) break;
	}

	if(sockfd != -1)
	{
		if( !host || !host->h_name || !*host->h_name )
		{
			sockfd = -1;
			return false;
		}

		if ( host->h_addrtype != AF_INET && host->h_addrtype != AF_INET6 )
			return false;

		if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
		{
			sockfd = -1;
			return false;
		}

		if(sockfd > 0)
		{
			bzero(&addr,sizeof(addr));
			addr.sin_family = host->h_addrtype;
			addr.sin_port = htons( (short) port );
			addr.sin_addr = *( (struct in_addr *) host->h_addr );

// 			if(bind(sockfd,(struct sockaddr*)&local,sizeof(local))<0)
// 			{  
// 				cout<<"bind error"<<endl;
// 			} 

			if( connect( sockfd, (struct sockaddr *) &addr, sizeof( struct sockaddr_in ) ) == -1 )
			{
				close( sockfd );
				sockfd = -1;
				return false;
			}
			else
			{
				avaliable = true;
				return true;
			}

			struct timeval tv; /* timeval and timeout stuff */
			tv.tv_sec = m_timeout;
			tv.tv_usec = 0;
			socklen_t tv_len = sizeof(tv);

			if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, tv_len)<0)
				sockfd = -1;

			if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, tv_len)<0)
				sockfd = -1;

		}
	}	
	
	return false;
}

/** 
 * @brief close socket connect
 * 
 * 
 * @return 
 */
bool Conn::close_conn(void)
{
	if(avaliable)
	{
		close(sockfd);
		avaliable = false;
	}
}

void Conn::check_status(void)
{
	if(sockfd <= 0)
	{
		close_conn();
	}
}

/** 
 * @brief send data with socket
 * 
 * @param buf 
 * @param len 
 * 
 * @return 
 */
int Conn::send_data(const char* buf,int len)
{	
    if ( len <= 0 || buf == NULL )
        return false;

	check_status();
	if(!avaliable)
		return false;

	struct timeval tv; /* timeval and timeout stuff */
	tv.tv_sec = m_timeout;
	tv.tv_usec = 0;
	socklen_t tv_len = sizeof(tv);

	if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, tv_len)<0)
	{
		sockfd = -1;
        close_conn();
        return false;
	}

	int total = send(sockfd,buf,len,0);
	while(total < len)
	{
		check_status();
		if(!avaliable)
			return false;

		int total_tmp = send(sockfd,buf+total,len-total,0);
		total += total_tmp;
	}

	return total;
}

/** 
 * @brief recived data from socket and save to buf
 * 
 * @param buf 
 * @param len 
 * 
 * @return total of recived or 0
 */
int Conn::recv_data(char* buf,int len)
{
    if ( len <= 0 || buf == NULL )
        return false;

	check_status();

	if(!avaliable)
		return false;

	struct timeval tv; /* timeval and timeout stuff */
	tv.tv_sec = m_timeout;
	tv.tv_usec = 0;
	socklen_t tv_len = sizeof(tv);

	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, tv_len)<0)
	{
		sockfd = -1;
        close_conn();
        return false;
	}

    int total = recv(sockfd, buf, len, 0);

    if ( total <= 0 )
    {
        close_conn();
        return false;
    }

	if(total <= len)
		buf[total]=0;
	else
		buf[len]=0;

    return total;
}
