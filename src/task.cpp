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

Task::~Task(void)
{
	delete p_http;
	delete p_ftp;
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
						string task_cfg;
						task_cfg.append(getenv("HOME"));
						task_cfg.append("/");
						task_cfg.append("Download");
						task_cfg.append("/");
						task_cfg.append(t_attr.local_file_name);
						task_cfg.append(".fg");
						
						FILE *f=fopen(task_cfg.c_str(),"r");
						if(f == NULL)
						{
							file_size=t_attr.size;
							if(file_size > MAX_STRING * MAX_STRING)
								thread_num = t_attr.thread_no;
							else
								thread_num = 1;
						}
						else
						{
							int num_of_thread=0;
							fscanf(f,"%d",&file_size);
							fscanf(f,"%d",&num_of_thread);

							if(file_size == 0)
								file_size = t_attr.size;

							if(num_of_thread == 0)
								num_of_thread = t_attr.thread_no;

							if(file_size > MAX_STRING * MAX_STRING)
								thread_num = num_of_thread;
							else
								thread_num = 1;

						}

						int block_size = file_size / thread_num;
						down_attr_t thread_attr[thread_num];
						GThread *down_fd[thread_num];
						GError *thread_error[thread_num];

						for(int i=0;i<thread_num;i++)
						{  
							thread_attr[i].m_attr = t_attr;
							thread_attr[i].start_address=block_size*i;  
							thread_attr[i].end_address=block_size*(i+1);
							thread_attr[i].limited=block_size;
							if(f != NULL)
								fscanf(f,"%d",&thread_attr[i].total);
							else
								thread_attr[i].total = 0;
							
							if(i==thread_num-1)  
							{
								thread_attr[i].end_address += (file_size % thread_num);  
								thread_attr[i].limited += (file_size % thread_num);
							}

							if((down_fd[i]=g_thread_create((GThreadFunc)setup_http_thread,(void*)&thread_attr[i],true,&thread_error[i]))==NULL)
							{
								g_error_free(thread_error[i]);
							}
						}

						if(f != NULL)
							fclose(f);
                        t_attr.start_time = p_http->gettime();

                        long long int nbytes=0;
                        while(nbytes < file_size)
                        {
							gdk_threads_enter();
							FILE *fp = fopen(task_cfg.c_str(), "w");
							
							if(fp)
							{
								fprintf(fp,"%d\n",file_size);
								fprintf(fp,"%d\n",thread_num);
							}

                            long long int m_total = 0;
                            for(int j=0;j<thread_num;j++)
                            {
								fprintf(fp,"%d\n",thread_attr[j].total);
                                m_total += thread_attr[j].total;
                            }
                            nbytes = m_total;
							fclose(fp);
							gdk_threads_leave();							

                            gdk_threads_enter(); 
                            int h=0,m=0,s=0,speed=0;
                            double progress=0.0;
                            gchar rtime[20],cspeed[20],cprogress[20];

                            progress = ((double)nbytes/file_size)*100;
                            sprintf(cprogress,"%0.1f/100.0",progress);
                            strcpy(t_attr.progress,cprogress);  

                            speed=(int)((double)nbytes/(p_http->gettime() - t_attr.start_time));
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
                            strcpy(t_attr.speed,cspeed); 

                            int time_right = (int)((double) ( file_size - nbytes ) / speed );
                            if(time_right > 0)
                            {
                                h = time_right/3600;
                                m = (time_right % 3600)/60;
                                s = (time_right % 3600) % 60;
                            }
                            sprintf(rtime,"%dh:%dm:%ds",h,m,s);
                            strcpy(t_attr.time_right,rtime);

                            gdk_threads_leave();

                        }

//                         for(int k=0;k<thread_num;k++)
//                         {
// 							g_thread_join(down_fd[k]);
//                         }
                        set_task_status(FINISH);
						remove(task_cfg.c_str());
                        return true;
                    }
                }
                set_task_status(ERROR);
                set_task_auto_flag(false);
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

void Task::setup_http_thread(void *m_handle)
{
	down_attr_t *m_down = (down_attr_t *)m_handle;

	string res_dir;
	res_dir.append(getenv("HOME"));
	res_dir.append("/");
	res_dir.append("Download");
	res_dir.append("/");
	res_dir.append(m_down->m_attr.local_file_name);

	FILE *fp = fopen(res_dir.c_str(), "wb+");
	if(!fp)
	{
		cout<<"create file error! %s\n"<<strerror(errno)<<endl;
		exit(1);
	}

start_down:

	if(m_down->total < m_down->limited)
	{
		Http *m_http = new Http(&m_down->m_attr);
		m_http->start_point = m_down->start_address + m_down->total;
		m_http->end_point = m_down->end_address;
		m_http->nbytes = m_down->total;

		m_http->parse_url(m_http->t_attr->url);
		m_http->create_conn(m_http->t_attr->hostname,m_http->t_attr->port);
		if(m_http->avaliable)
		{
			fseek(fp,m_http->start_point,SEEK_SET);
			m_http->encode_request();
			send(m_http->sockfd,m_http->request,strlen(m_http->request),0);
			int rs = recv(m_http->sockfd,m_http->headers,MAX_STRING,0);
		
			if(rs > 0)
			{
				int i=0; long long int nbytes=0;

				if(strstr(m_http->headers,"HTTP/1.1 206") != NULL || strstr(m_http->headers,"HTTP/1.1 200") != NULL )
				{
					char *body=strstr(m_http->headers,"\r\n\r\n")+4;  
					if(body != NULL && m_http->t_attr->size > 0)
					{
						gdk_threads_enter(); 

						nbytes = rs - (body-m_http->headers);  
						fwrite(body,sizeof(char),nbytes,fp);  
						fflush(fp);
						m_http->nbytes += nbytes;
						m_down->total = m_http->nbytes; 

						gdk_threads_leave();

						while(m_down->total < m_down->limited)
						{
							memset(m_http->headers, 0, sizeof(m_http->headers));
							i = recv(m_http->sockfd,m_http->headers,MAX_STRING,0);

							if(i == 0)
							{
								m_http->dis_conn();
								delete m_http;
								goto start_down;
							}

							gdk_threads_enter(); 
							fwrite(m_http->headers,sizeof(char),i,fp);  
							fflush(fp);  
							m_http->nbytes += i; 
							m_down->total = m_http->nbytes; 
							gdk_threads_leave();
						}
						fclose(fp);
						m_http->dis_conn();
						delete m_http;
					}
				}
			}
			else
			{
				m_http->dis_conn();
				delete m_http;
				goto start_down;
			}
		}
		else
		{
			delete m_http;
			goto start_down;
		}
	}
	else
		fclose(fp);
}

void Task::setup_ftp_thread(void *m_handle)
{
    ;
}

void Task::setup_p2p_thread(void *m_handle)
{
    ;
}
