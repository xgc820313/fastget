/*
 * =====================================================================================
 *
 *       Filename:  task.cpp
 *
 *    Description:  task operation
 *
 *        Version:  1.0
 *        Created:  Friday, May 08, 2009 12:19:17 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon, chinagnu@gmail.com
 *        Company:  Fong's National Engineering Co. Ltd
 *
 * =====================================================================================
 */

#include	"task.h"

Task::Task(attr_t m_attr)
{
	p_http = 0;
	p_ftp = 0;
	t_attr = m_attr;

	if(t_attr.url != NULL && strstr(t_attr.url,"://") != NULL)
	{
		if(!strncmp(t_attr.url, "http://", strlen("http://")) || 
				!strncmp(t_attr.url,"https://",strlen("https://")))
		{
			p_http = new Http(&t_attr);
			protocal_flag = HTTP;
		}
		else if(!strncmp(t_attr.url, "ftp://", strlen("ftp://")) || 
				!strncmp(t_attr.url,"ftps://",strlen("ftps://")))
		{
			p_ftp = new Ftp(&t_attr);
			protocal_flag = FTP;
		}
	}
}

bool Task::action_task(void)
{
	if( t_attr.auto_flag)
	{
		switch(protocal_flag)
		{
			case HTTP:
				if(p_http != NULL)
				{
					if(p_http->get_resource())
					{
						t_attr.start_time = p_http->gettime();
						if(p_http->setup_download_thread())
						{
							set_task_status(FINISH);
							return true;
						}
						else
							set_task_status(ERROR);
					}
					else
						set_task_status(ERROR);
					set_task_auto_flag(false);
				}
				break;
			case FTP:
				if(p_ftp != NULL)
				{
					;
				}
				break;
			default:
				break;
		}
	}
	return false;
}

Task::~Task(void)
{
	delete p_http;
	delete p_ftp;
}

