
#include	"http.h"

Http::Http(attr_t *attr)
{
	memset(request, 0, sizeof(request));
	memset(headers, 0, sizeof(headers));
	start_point=0;
	end_point=0;
	t_attr = attr;
}

Http::~Http(void)
{
	dis_conn();
}

void Http::dis_conn(void)
{
	if(avaliable)
	{
		close(sockfd);
		avaliable = false;
		sockfd = -1;
	}
}

void Http::parse_url(char *url)
{
	char *pA=0,*pB=0,*pC=0,*pD=0,*pE=0;

	if(!url)
		return ;

    http_decode(url);

begin:
	pA = url;
	if(!strncmp(pA, "http://", strlen("http://")))
	{
		pA = url+strlen("http://");
	}
	else if(!strncmp(pA, "https://", strlen("https://")))
	{	
		pA = url+strlen("https://");
	}

	pB = strchr(pA, '/');

	if(pB)
	{
		if((pD=strstr(pB,"http://")) != NULL || (pD=strstr(pB,"https://")) != NULL )
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
            http_encode(t_attr->dir);
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

        http_encode(t_attr->target_file_name);

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
		t_attr->port = 80;
}


void Http::add_header(char *format, ... )
{
	char s[MAX_STRING];
	va_list params;
	
	va_start( params, format );
	vsnprintf( s, MAX_STRING - 3, format, params );
	strcat( s, "\r\n" );
	va_end( params );

	strncat( request, s, MAX_QUERY );
}

void Http::encode_request(void )
{
	add_header( "GET %s%s HTTP/1.1", t_attr->dir,t_attr->target_file_name );
	add_header( "Accept: %s", "* / *");
	add_header( "Host: %s:%d", t_attr->hostname, t_attr->port );
	add_header( "User-Agent: %s", USER_AGENT );
//    add_header( "Accept-Encoding: %s", "gzip,deflate");
    add_header( "Accept-Charset: %s", "utf-8,gb2312");
	if( start_point >= 0 )
	{
		if( end_point > start_point )
			add_header( "Range: bytes=%lld-%lld", start_point, end_point );
		else
			add_header( "Range: bytes=%lld-", start_point );
	}
	add_header( "Connection: %s", "close" );
	add_header( "" );
}

gchar* Http::http_header( char *header )
{
	char s[32];
	int i;
	
	for( i = 1; headers[i]; i ++ )
	{
		if( headers[i-1] == '\n' )
		{
			sscanf( &headers[i], "%31s", s );
			if( strcasecmp( s, header ) == 0 )
				return( &headers[i+strlen(header)] );
		}
	}
	return( NULL );
}

long long int Http::get_target_size( void )
{
	char *i;
	long long int j;

	if( ( i = http_header("Content-Length:") ) == NULL )
		return( -2 );
	
	sscanf( i, "%lld", &j );
	return( j );
}

void Http::get_redirect_url(void)
{
	char *new_url=NULL;
	if((new_url = http_header("Location:")) != NULL )
	{
		gdk_threads_enter(); 

		char buffer[MAX_STRING];
		sscanf( new_url, "%s", buffer);

		if(strstr(buffer,"://") != NULL)
		{
			memset(t_attr->url,0,sizeof(t_attr->url));
			memset(t_attr->hostname,0,sizeof(t_attr->hostname));
			memset(t_attr->target_file_name,0,sizeof(t_attr->target_file_name));
			memset(t_attr->dir,0,sizeof(t_attr->dir));

			strncpy(t_attr->url,buffer,strlen(buffer));
			t_attr->url[strlen(buffer)] = 0;

			parse_url(t_attr->url);
		}
		else
		{
			char temp_dir[MAX_STRING];
			strcpy(temp_dir,t_attr->dir);
			memset(t_attr->dir,0,sizeof(t_attr->dir));
			strcpy(t_attr->dir,temp_dir);
			memset(t_attr->target_file_name,0,sizeof(t_attr->target_file_name));
			strncpy(t_attr->target_file_name,buffer,strlen(buffer));
			t_attr->target_file_name[strlen(buffer)] = 0;

            if(strncmp(t_attr->url,"https",5)== 0)
                sprintf(t_attr->url,"https://%s%s%s",t_attr->hostname,t_attr->dir,t_attr->target_file_name);
            else    
                sprintf(t_attr->url,"http://%s%s%s",t_attr->hostname,t_attr->dir,t_attr->target_file_name);
		}

		if(!avaliable)
		{
			dis_conn();
			create_conn(t_attr->hostname,t_attr->port);
		}

		memset(request, 0, sizeof(request));
		memset(headers, 0, sizeof(headers));

		gdk_threads_leave();

		get_resource();
	}
}

gboolean Http::get_resource(void)
{
	if(t_attr->retry == 0)
	{
		parse_url(t_attr->url);
    	create_conn(t_attr->hostname,t_attr->port);
	}

	if(avaliable)
	{
		encode_request();

		send(sockfd,request,strlen(request),0);
		int rs = recv(sockfd,headers,MAX_STRING,0);

		if(rs > 0)
		{
			if(strstr(headers,"HTTP/1.1 404") == NULL  && strstr(headers,"HTTP/1.1 301") == NULL)
            {
				if(strstr(headers,"HTTP/1.1 302") == NULL && strstr(headers,"HTTP/1.1 303") == NULL)
				{
					if(get_target_size()>0)
					{
						gdk_threads_enter(); 
						t_attr->size=get_target_size();
						gdk_threads_leave();
						return true;
					}
				}
				else
				{
					t_attr->retry++; 
					if(t_attr->retry < 5)
					{
						get_redirect_url();
					}
				}
            }
			// add error information into message queue if can't fond
		}
        else
			dis_conn();
	}
	return false;
}

gboolean Http::setup_download_thread(void)
{
	if(avaliable)
	{
		int i=0,nbytes=0,num=0;
		char buffer[MAX_STRING];

		string res_dir;
		res_dir.append(getenv("HOME"));
		res_dir.append("/");
		res_dir.append("Download");
		res_dir.append("/");
		res_dir.append(t_attr->local_file_name);

		char *body=strstr(headers,"\r\n\r\n")+4;  
		if(body != NULL && t_attr->size > 0)
		{
			FILE *fp = fopen(res_dir.c_str(), "wb+");
			if(!fp)
			{
				printf("create file error! %s\n", strerror(errno));
				return false;
			}

            gdk_threads_enter(); 

			nbytes = strlen(headers) - (body-headers);  
			fwrite(body,sizeof(char),nbytes,fp);  
			fflush(fp);

            gdk_threads_leave();

			end_point=t_attr->size;
			start_point=1;
			memset(request, 0, sizeof(request));
			encode_request();

			send(sockfd,request,strlen(request),0);
			while(nbytes < end_point)
			{
				memset(headers, 0, sizeof(headers));
				i = recv(sockfd,headers,MAX_STRING,0);

				if(i == 0)
				{
					dis_conn();
					return false;
				}

				fwrite(headers,sizeof(char),i,fp);  
				fflush(fp);  
				nbytes += i;  

                gdk_threads_enter(); 
				int h=0,m=0,s=0,speed=0;
				double progress=0.0;
				gchar rtime[20],cspeed[20],cprogress[20];

				progress = ((double)nbytes/end_point)*100;
				sprintf(cprogress,"%0.1f/100",progress);
				strcpy(t_attr->progress,cprogress);  

				speed=(int)((double)nbytes/(gettime() - t_attr->start_time));
				if(speed < 0) speed = 0;
				if(speed >= (MAX_STRING * MAX_STRING))
				{
					sprintf(cspeed,"%d m/s",speed/MAX_STRING/MAX_STRING);
				}
				else if(speed >= MAX_STRING)
				{
					sprintf(cspeed,"%d k/s",speed/MAX_STRING);
				}
				else
					sprintf(cspeed,"%d b/s",speed);
				strcpy(t_attr->speed,cspeed); 

				int time_right = (int)((double) ( end_point - nbytes ) / speed );
				if(time_right > 0)
				{
					h = time_right/3600;
					m = (time_right % 3600)/60;
					s = (time_right % 3600) % 60;
				}
				sprintf(rtime,"%dh:%dm:%ds",h,m,s);
				strcpy(t_attr->time_right,rtime);

//				cout<<"time right:"<<t_attr->time_right<<" speed:"<<t_attr->speed<<" progress:"<<t_attr->progress<<endl;
                gdk_threads_leave();

			}
			fclose(fp);
            dis_conn();
			return true;
		}
	}
	return false;
}

double Http::gettime(void)
{
	struct timeval time[1];
	
	gettimeofday( time, 0 );
	return( (double) time->tv_sec + (double) time->tv_usec / 1000000 );
}

void Http::http_decode( char *s )
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

void Http::http_encode( char *s )
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
