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
 * @file       conf.cpp
 *
 * @author     dragon <chinagnu@gmail.com>
 * @date       Fri Jul  24 08:55:14 2009
 * @brief      bug fixed of import configure file and create tha task object
 * @version    0.3
 *
 * @author     dragon <chinagnu@gmail.com>
 * @date       Wed Jul  22 11:55:27 2009
 * @brief      import and save the thread number of tasks
 * @version    0.2
 *
 * @author     dragon <chinagnu@gmail.com>
 * @date       Mon Jul  13 10:03:24 2009
 * @brief      configure file and task list operation
 * @version    0.1
 * 
 * @todo       unknow
 * @bug        unknow
 */

#include	"conf.h"

Conf::Conf(void)
{
	string res_dir;
	res_dir.append(getenv("HOME"));
	res_dir.append("/");
	res_dir.append("Downloads");

	if(access(res_dir.c_str(),X_OK) != 0)
	{
		mkdir(res_dir.c_str(),S_IRWXU|S_IRWXG);
	}

	string cfg = get_task_list_conf_file();
	if(access(cfg.c_str(),X_OK) != 0)
	{
		// create a empty file named .task_list 
	}
}

Conf::~Conf(void){}

/** 
 * @fn get_task_list_conf_file(void)
 * @desc get configure file full path
 * 
 * @return string of full path if successfull 
 */
string Conf::get_task_list_conf_file(void)
{
	string task_list_cfg;
	task_list_cfg.append(getenv("HOME"));
	task_list_cfg.append("/");
	task_list_cfg.append("Downloads");
	task_list_cfg.append("/");
	task_list_cfg.append(".task_list");

	return task_list_cfg;
}

bool Conf::import_task_list(void)
{
	string cfg_file = get_task_list_conf_file();
	read_conf_file(cfg_file.c_str());

	return true;
}

bool Conf::save_task_list(void)
{
	string cfg_file = get_task_list_conf_file();
	FILE *fp = fopen(cfg_file.c_str(),"w");
	if(fp != NULL)
	{
		char *xml_head="<?xml version='1.0' encoding='UTF-8'?>";
		fwrite(xml_head,sizeof(char),strlen(xml_head),fp);

		fwrite("<task_list>",sizeof(char),strlen("<task_list>"),fp);		
		for(unsigned int i = 0; i < m_task_list.size(); i++)
		{
			fwrite("<task",sizeof(char),strlen("<task"),fp);

			fwrite(" status='",sizeof(char),strlen(" status='"),fp);
			char status[10];
			sprintf(status,"%d",m_task_list[i]->m_attr.get_status());
			fwrite(status,sizeof(char),strlen(status),fp);
			fwrite("' ",sizeof(char),strlen("' "),fp);
			
			fwrite(" url='",sizeof(char),strlen(" url='"),fp);
			fwrite(m_task_list[i]->m_attr.get_url()->c_str(),sizeof(char),strlen(m_task_list[i]->m_attr.get_url()->c_str()),fp);
			fwrite("' ",sizeof(char),strlen("' "),fp);

			fwrite(" thread='",sizeof(char),strlen(" thread='"),fp);
			char num[10];
			sprintf(num,"%d",m_task_list[i]->m_attr.get_thread_num());
			fwrite(num,sizeof(char),strlen(num),fp);
			fwrite("' ",sizeof(char),strlen("' "),fp);

			fwrite(" name='",sizeof(char),strlen(" name='"),fp);
			fwrite(m_task_list[i]->m_attr.get_file_name()->c_str(),sizeof(char),strlen(m_task_list[i]->m_attr.get_file_name()->c_str()),fp);
			fwrite("' ",sizeof(char),strlen("' "),fp);

			fwrite(" user='",sizeof(char),strlen(" user='"),fp);
			fwrite(m_task_list[i]->m_attr.get_user_name()->c_str(),sizeof(char),strlen(m_task_list[i]->m_attr.get_user_name()->c_str()),fp);
			fwrite("' ",sizeof(char),strlen("' "),fp);

			fwrite(" passwd='",sizeof(char),strlen(" passwd='"),fp);
			fwrite(m_task_list[i]->m_attr.get_password()->c_str(),sizeof(char),strlen(m_task_list[i]->m_attr.get_password()->c_str()),fp);
			fwrite("' ",sizeof(char),strlen("' "),fp);

			fwrite("></task>",sizeof(char),strlen("></task>"),fp);

			delete m_task_list[i];
		}
		fwrite("</task_list>",sizeof(char),strlen("</task_list>"),fp);		

		fclose(fp);
		return true;
	}
	return false;
}

void Conf::StartXMLElement(const char *name, const char **atts)
{
    if (strcmp(name, "task") == 0)
	{
		add_task(atts);
	}
}

#define    TAGNUM    6
bool Conf::add_task(const char **atts)
{
	if(atts == NULL || atts == 0)
		return false;

	bool flag = true,check_flag=true;
	Task *m_task = new Task();
	
	if(m_task == NULL)
		return false;

	m_task->m_attr.set_index(m_task_list.size()+1);
	m_task->m_attr.set_progress(0);
	m_task->m_attr.set_speed(0,0);
	m_task->m_attr.set_time_right(0,0);
	m_task->m_attr.set_target_size();

	unsigned int m_times=0;
    for(int i=0; atts[i]; i+=2) 
	{

        if (strcmp(atts[i], "status") == 0) 
		{
			m_times++;
            if(!m_task->m_attr.set_status((status_t)atoi(atts[i+1])))
				flag = false;
        }
		else if(strcmp(atts[i], "url") == 0) 
		{
			m_times++;
			if(!m_task->m_attr.set_url(atts[i+1]))
				flag = false;
        }
		else if(strcmp(atts[i], "thread") == 0)
		{
			m_times++;
			if(!m_task->m_attr.set_thread_num(atoi(atts[i+1])))
				flag = false;
		}
		else if(strcmp(atts[i], "name") == 0)
		{
			m_times++;
			if(!m_task->m_attr.set_file_name(atts[i+1]))
			{
				cout<<atts[i+1]<<endl;
				flag = false;
			}
		}		
		else if (strcmp(atts[i], "user") == 0)
		{
			m_times++;
			if(strlen(atts[i+1])>0)
			{
				if(!m_task->m_attr.set_user_name(atts[i+1]))
					flag = false;
			}
			else
			{
				m_task->m_attr.set_user_name("anonymous");
			}
        } 
		else if (strcmp(atts[i], "passwd") == 0)
		{
			m_times++;
			if(strlen(atts[i+1])>0)
			{
				if(!m_task->m_attr.set_password(atts[i+1]))
					flag = false;
			}
			else
			{
				m_task->m_attr.set_password("chinagnu@gmail.com");
			}
        }
    }

	if(m_times != TAGNUM)
		check_flag = false;

	if(flag && check_flag)
	{
		m_task_list.push_back(m_task);
//		m_task->init();
		return true;
	}

	return false;
}
