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
 * @file       conf.h
 * @author     dragon <chinagnu@gmail.com>
 * @date       Mon Jul  13 09:54:09 2009
 * 
 * @brief      header define of configure operation
 * @version    0.1
 * 
 * @todo       unknow
 * @bug        unknow
 */

#ifndef  CONFIG_H_INC
#define  CONFIG_H_INC

#include	"task.h"
#include	"xml.h"


class Conf:public XMLFile
{
public:

	Conf (void);
	~Conf(void);

	void read_conf_file(const char *filename)
	{
		read_xml_file(filename);
	}

	string get_task_list_conf_file(void);
	bool import_task_list(void);
	bool save_task_list(void);

	void StartXMLElement(const char *name, const char **atts);
	vector<Task*>& get_task_list(void){return m_task_list;};

protected:

private:
	vector<Task*> m_task_list;

	bool add_task(const char **atts);
};

#endif	/*  */

