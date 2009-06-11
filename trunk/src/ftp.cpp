#include	"ftp.h"

Ftp::Ftp(attr_t *attr)
{
	t_attr = attr;
}

Ftp::~Ftp(void)
{
	dis_conn();
}

void Ftp::dis_conn(void)
{
	if(sockfd > 0)
		close(sockfd);
	sockfd = -1;
}

void Ftp::parse_url(char *url)
{
	char *pA=0,*pB=0,*pC=0,*pD=0,*pE=0;

	if(!url)
		return ;

    ftp_decode(url);

begin:
	pA = url;
	if(!strncmp(pA, "ftp://", strlen("ftp://")))
	{
		pA = url+strlen("ftp://");
	}
	else if(!strncmp(pA, "ftps://", strlen("ftps://")))
	{	
		pA = url+strlen("ftps://");
	}

	pB = strchr(pA, '/');

	if(pB)
	{
		if((pD=strstr(pB,"ftp://")) != NULL || (pD=strstr(pB,"ftps://")) != NULL )
        {
            pE = strstr(pD,"&");
            if(pE)
            {
                int new_url_len = pE - pD;
                strncpy(t_attr->url,pD,new_url_len);
                t_attr->url[new_url_len]=0;

                goto begin;
            }
        }
		
		int hostname_len = strlen(pA) - strlen(pB);
		strncpy(t_attr->hostname,pA, hostname_len);
		t_attr->hostname[hostname_len]=0;
//		t_attr->hostname = new char[strlen(pA)-strlen(pB)];
//		memcpy(t_attr->hostname, pA, strlen(pA) - strlen(pB));

		pC = strrchr(pB,'/');
		if(pC)
		{
			int dir_len = strlen(pB) - strlen(pC)+1;
			strncpy(t_attr->dir,pB,dir_len);
			t_attr->dir[dir_len]=0;
            ftp_encode(t_attr->dir);
			if(pC+1)
			{
				sprintf(t_attr->target_file_name,"%s",pC+1);
				t_attr->target_file_name[strlen(pC)-1] = 0;
//				t_attr->target_file_name = new char[strlen(pC-1)];
//				memcpy(t_attr->target_file_name,pC+1,strlen(pC)-1);

			}
			else
			{
				sprintf(t_attr->target_file_name,"%s","index.html");
				t_attr->target_file_name[strlen("index.html")]=0;
//				t_attr->target_file_name = new char[MAX_FILE_NAME];
			}
		}
		else
		{
			strncpy(t_attr->dir,pB,1);
			t_attr->dir[1]=0;
			if(pB+1)
			{
				sprintf(t_attr->target_file_name,"%s",pB+1);	
				t_attr->target_file_name[strlen(pB) - 1] = 0;
//				t_attr->target_file_name = new char[strlen(pB)-1];
//				memcpy(t_attr->target_file_name, pB + 1, strlen(pB) - 1);
			}
		}

        ftp_encode(t_attr->target_file_name);

	}
	else 
	{
		sprintf(t_attr->hostname,"%s",pA);
		t_attr->hostname[strlen(pA)]=0;
//		t_attr->hostname = new char[strlen(pA)];
//		memcpy(t_attr->hostname, pA, strlen(pA));
	}

	if(pB)
		t_attr->hostname[strlen(pA) - strlen(pB)] = 0;
	else 
		t_attr->hostname[strlen(pA)] = 0;

	pA = strchr(t_attr->hostname, ':');
	if(pA)
		t_attr->port = atoi(pA + 1);
	else
		t_attr->port = 21;
}

double Ftp::gettime(void)
{
	struct timeval time[1];
	
	gettimeofday( time, 0 );
	return( (double) time->tv_sec + (double) time->tv_usec / 1000000 );
}

void Ftp::ftp_decode( char *s )
{
	char t[MAX_STRING];
	int i, j, k;
	
	for( i = j = 0; s[i]; i ++, j ++ )
	{
		t[j] = s[i];
		if( s[i] == '%' )
			if( sscanf( s + i + 1, "%2x", &k ) )
			{
				t[j] = k;
				i += 2;
			}
	}
	t[j] = 0;
	
	strcpy( s, t );
}

void Ftp::ftp_encode( char *s )
{
	char t[MAX_STRING];
	int i, j;
	
	for( i = j = 0; s[i]; i ++, j ++ )
	{
		t[j] = s[i];
		if( s[i] == ' ' )
		{
			strcpy( t + j, "%20" );
			j += 2;
		}
	}
	t[j] = 0;
	
	strcpy( s, t );
}
