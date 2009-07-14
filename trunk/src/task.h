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
 * @file       task.h
 * @author     dragon <chinagnu@gmail.com>
 * @date       Mon Jul  13 10:01:15 2009
 * 
 * @brief      header define of signal task
 * @version    0.1
 * 
 * @todo       unknow
 * @bug        unknow
 */

#ifndef  TASK_H_INC
#define  TASK_H_INC

#include "protocol.h"

class Task
{
private:

public:

	Attribute m_attr;

	Task(void){};
	~Task(void){};

	void init(void);
};

#endif	/*  */
