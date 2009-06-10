
#include	"conn.h"

void Conn::create_conn(char *hostname, int port)
{
	struct hostent *host = NULL;
	struct sockaddr_in addr;
	struct sockaddr_in local;
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
		}

		if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
		{
			sockfd = -1;
		}

		if(sockfd > 0)
		{
			struct timeval tv; /* timeval and timeout stuff added by davekw7x */
			tv.tv_sec = 2;
			tv.tv_usec = 0;
			socklen_t tv_len = sizeof(tv);

			if(setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, tv_len)<0)
				sockfd = -1;

			if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, tv_len)<0)
				sockfd = -1;

			bzero(&addr,sizeof(addr));
			addr.sin_family = AF_INET;
			addr.sin_port = htons( port );
			addr.sin_addr = *( (struct in_addr *) host->h_addr );
			//    addr.sin_addr.s_addr = htonl(INADDR_ANY);

// 			if(bind(sockfd,(struct sockaddr*)&local,sizeof(local))<0)
// 			{  
// 				cout<<"bind error"<<endl;
// 			} 

			if( connect( sockfd, (struct sockaddr *) &addr, sizeof( struct sockaddr_in ) ) == -1 )
			{
				close( sockfd );
				sockfd = -1;
			}
			else
				avaliable = true;
		}
	}
}
