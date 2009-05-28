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
	res_dir.append("Download");

	if(access(res_dir.c_str(),X_OK) != 0)
	{
		mkdir(res_dir.c_str(),S_IRWXU|S_IRWXG);
	}
}
