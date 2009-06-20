#include	"ftp.h"

Ftp::Ftp(attr_t *attr)
{
	t_attr = attr;
	memset(request,sizeof(request),0);
	memset(headers,sizeof(headers),0);
	dip = new char[16];
	dport = 0;
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

gboolean Ftp::encode_cmd(char *format, ... )
{
	memset(request,sizeof(request),0);

	va_list params;
	va_start( params, format );
	vsnprintf( request, MAX_STRING - 3, format, params );
	strcat( request, "\r\n" );
	va_end( params );

	if((unsigned int)send(sockfd,request,strlen(request),0) != strlen(request))
		return false;

	return true;
}

/** 
 * \fn get_resource(void)
 * \remarks get the target file size with bytes
 * 
 * @return 
 */
gboolean Ftp::get_resource(void)
{
	if(t_attr->retry == 0)
	{
		parse_url(t_attr->url);
    	create_conn(t_attr->hostname,t_attr->port);
	}

	if(avaliable)
	{
		if(login())
		{
			if(!set_bin_mode())
				return false;

			if(!set_pasv_mode())
				return false;

			if(get_size())
			{
				return true;
			}
		}
	}
	else
		dis_conn();		

	return false;
}

int Ftp::ftp_cwd( char *cwd )
{
	int rs;
	encode_cmd("CWD %s", cwd );
    rs = get_result_code( );

    if ( rs != 250 )
    {
		fprintf( stderr, _("Can't change directory to %s\n"), cwd );
		return false;
    }

	return true;
}

int Ftp::get_line_buf(char *buf, int movepos, char *line )
{
#define MAXLINE 256

    for ( int i = 0;i <= movepos - 2;i++ )
    {
        if ( buf[ i ] == '\r' && buf[ i + 1 ] == '\n' )
        {
            memcpy( line, buf, i > MAXLINE - 3 ? MAXLINE - 2 : i );
            line[ i > MAXLINE - 2 ? MAXLINE - 1 : i ] = 0;
            return i + 2;
        }
    }

    return -1;

#undef MAXLINE
}

int Ftp::get_result_code()
{

    int rs;
    char buffer[MAX_STRING];
    int movepos = 0;
    char line[ MAX_FILE_NAME ];
    int nlinelen;

nextline:

    while ( ( nlinelen = get_line_buf( buffer, movepos, line ) ) == -1 )
    { 

        if ( movepos > 1000 )
        {
            return -3; 
        }

		rs = recv(sockfd,buffer + movepos,MAX_STRING - movepos,0);

        if ( rs <= 0 )
        {
            return -2; 
        }

        movepos += rs;
    }

    if ( movepos > nlinelen )
    {  
        memmove( buffer, buffer + nlinelen, movepos - nlinelen );
    }

    movepos -= nlinelen;

    if ( strlen( line ) >= 3 )
    { 
        if ( strlen( line ) >= 4 )
        {
            if ( line[ 3 ] == '-' )
            { 
                goto nextline;
            }
            else
            { 
                line[ 3 ] = 0;

                if ( line[ 0 ] >= '0' && line[ 0 ] <= '9' &&
                        line[ 1 ] >= '0' && line[ 1 ] <= '9' &&
                        line[ 2 ] >= '0' && line[ 2 ] <= '9'
                   )
                {
                    return atoi( line );
                }
                else
                { 
                    goto nextline;
                }
            }
        }
        else
        {
            line[ 3 ] = 0;
            return atoi( line );
        }
    }
    else
    { 
        goto nextline;
    }
}

gboolean Ftp::login(void)
{
    int rs;

	encode_cmd("USER %s",t_attr->user);
    rs = get_result_code( );

    if ( rs >= 500 )
    {
        return false;
    }

    if ( rs == 230 )
    {
        return true; 
    }

	encode_cmd( "PASS %s", t_attr->passwd);	
	rs = get_result_code( );

	if ( rs != 230 )
	{
		return false;
	}

	return true;
}

gboolean Ftp::split_target( long long int pos )
{
    int rs;

	encode_cmd("REST %lld", pos);
    rs = get_result_code( );

    if ( rs == 350 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

gboolean Ftp::get_file(void)
{
    int rs;

	if(!ftp_cwd(t_attr->dir +1))
		return false;

	encode_cmd("RETR %s",t_attr->target_file_name);
    rs = get_result_code( );

    if ( rs == 150 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

gboolean Ftp::logout(void)
{
	int rs;
	encode_cmd("QUIT", 6 );
	rs = get_result_code( );

    if ( rs == 221 )
        return true;
    else
        return false;
}

gboolean Ftp::set_text_mode(void)
{
	encode_cmd("TYPE A");
    int rs = get_result_code( );

    if ( rs == 200 )
    {
        return true;
    }
    else
        return false;
}

gboolean Ftp::set_bin_mode(void)
{
	encode_cmd("TYPE I");
    int rs = get_result_code();

    if ( rs == 200 )
    {
        return true;
    }
    else
        return false;
}

gboolean Ftp::abort(void)
{
	int rs;
	encode_cmd("ABOR", 6 );
	rs = get_result_code( );

    if ( rs == 200 )
    {
        return true;
    }

    return false;
}

gboolean Ftp::set_pasv_mode(void)
{
    int rs;
    int i = 0;
    char buffer[MAX_STRING];
    int movepos = 0;
    char line[ MAX_FILE_NAME];

	encode_cmd("PASV", 6 );

    while ( get_line_buf( buffer, movepos, line ) == -1 )
    { 
        if ( movepos > 1000 )
        {
            return false; 
        }

		rs = recv(sockfd,buffer + movepos,MAX_STRING - movepos,0);

        if ( rs <= 0 )
        {
            return false;
        }

        movepos += rs;
    }



    if ( strlen( line ) < 5 )
    {
        return false;
    }

    char code[ 4 ];
    memcpy( code, line, 3 );
    code[ 3 ] = 0;

    if ( atoi( code ) != 227 )
    {
        return false;
    }

    int leftb, rightb = 0;
    char s[ 32 ] = {0};

    i = 4;

    while ( line[ i ] != '(' && line[ i ] != 0 )
        i++;

    if ( line[ i ] != '(' )
    {
        return false;
    }
    else
    {
        leftb = i;
    }

    i = strlen( line );

    while ( line[ i ] != ')' && i > 1 )
        i--;

    if ( line[ i ] != ')' )
    {
        return false;
    }
    else
    {
        rightb = i;
    }

    memcpy( s, &line[ leftb ], rightb - leftb + 1 );

    int a, b, c, d, p1, p2;

    if ( 6 != sscanf( s, "(%d,%d,%d,%d,%d,%d)", &a, &b, &c, &d, &p1, &p2 ) )
    {
        return false;
    }

    sprintf( dip, "%d.%d.%d.%d", a, b, c, d );
    dport = ( p1 << 8 ) + p2;

    return true;
}


gboolean Ftp::get_size(void)
{
    char buf[ MAX_STRING];
    int movepos = 0;
    char line[ MAX_FILE_NAME];
    int rs;

	encode_cmd("SIZE %s%s",t_attr->dir+1,t_attr->target_file_name);

    while ( get_line_buf( buf, movepos, line ) == -1 )
    { 
        if ( movepos > 1020 )
            return false; 

		rs = recv(sockfd,buf + movepos,MAX_STRING - movepos,0);

        if ( rs <= 0 )
        {
            return false; 
        }

        movepos += rs;
    }

	char codes[4];
	memcpy(codes,line,3);
	codes[3]=0;
	rs=atoi(codes);

	char lens[60];
	strcpy(lens,&line[4]);
	

	gdk_threads_enter();

//	t_attr->size=_atoi64(lens);
	t_attr->size=atoll(lens);

	if(t_attr->size >= (MAX_STRING * MAX_STRING * MAX_STRING))
	{
		sprintf(t_attr->file_size,"%0.1f G",(double)t_attr->size/MAX_STRING/MAX_STRING/MAX_STRING);
	}
	else if(t_attr->size >= (MAX_STRING * MAX_STRING))
	{
		sprintf(t_attr->file_size,"%0.1f M",(double)t_attr->size/MAX_STRING/MAX_STRING);
	}
	else if(t_attr->size >= MAX_STRING)
	{
		sprintf(t_attr->file_size,"%0.1f K",(double)t_attr->size/MAX_STRING);
	}
	else
		sprintf(t_attr->file_size,"%d B",t_attr->size);

	gdk_threads_leave();

	return rs==213;
}

