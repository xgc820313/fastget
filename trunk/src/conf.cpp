/*
 * =====================================================================================
 *
 *       Filename:  conf.cpp
 *
 *    Description:  Configure file operation
 *
 *        Version:  1.0
 *        Created:  Friday, May 15, 2009 03:57:50 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon, chinagnu@gmail.com
 *        Company:  Fong's National Engineering Co. Ltd
 *
 * =====================================================================================
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
}

gboolean Conf::import_task_list(void)
{
	string task_list_cfg;
	task_list_cfg.append(getenv("HOME"));
	task_list_cfg.append("/");
	task_list_cfg.append("Downloads");
	task_list_cfg.append("/");
	task_list_cfg.append(".task_list");

	FILE *cfg = fopen(task_list_cfg.c_str(),"r");

	if(cfg != NULL)
	{

		fclose(cfg);
	}

	return false;
}
