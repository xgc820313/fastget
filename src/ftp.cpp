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
	char *pA=0;
	char *pB=0;

	if(!(*url))
		return ;

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
//		t_attr->hostname = new char[strlen(pA)-strlen(pB)];
//		memcpy(t_attr->hostname, pA, strlen(pA) - strlen(pB));
		strncpy(t_attr->hostname,pA, strlen(pA) - strlen(pB));

		char *pC = strrchr(pB,'/');
		if(pC)
		{
			strncpy(t_attr->dir,pB,strlen(pB) - strlen(pC)+1);
			if(pC+1)
			{
//				t_attr->target_file_name = new char[strlen(pC-1)];
//				memcpy(t_attr->target_file_name,pC+1,strlen(pC)-1);
//				t_attr->target_file_name[strlen(pC)-1] = 0;
				strncpy(t_attr->target_file_name,pC+1,strlen(pC-1));
			}
			else
			{
//				t_attr->target_file_name = new char[MAX_FILE_NAME];
				strncpy(t_attr->target_file_name,"index.html",strlen("index.html"));
			}
		}
		else
		{
			strncpy(t_attr->dir,pB,1);
			if(pB+1)
			{
//				t_attr->target_file_name = new char[strlen(pB)-1];
//				memcpy(t_attr->target_file_name, pB + 1, strlen(pB) - 1);
//				t_attr->target_file_name[strlen(pB) - 1] = 0;
				strncpy(t_attr->target_file_name,pB+1,strlen(pB)-1);	
			}
		}
	}
	else 
	{
//		t_attr->hostname = new char[strlen(pA)];
//		memcpy(t_attr->hostname, pA, strlen(pA));
		strncpy(t_attr->hostname,pA,strlen(pA));
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
