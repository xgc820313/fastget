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
 * @file   dragon.cpp
 * @author dragon <chinagnu@gmail.com>
 * @date   Wed Jul  1 16:50:12 2009
 * 
 * @brief  start point of this software
 * @ver    0.1
 * 
 */

#include "conf.h"

int main(int argc,char* argv[])
{
	// gtk environment check 
	if ( !g_thread_supported() )
		g_thread_init ( NULL );

	gdk_threads_init();
	gtk_init ( &argc, &argv );

	Conf m_conf;
	m_conf.import_task_list();

//	UI *m_ui = new UI();

	m_conf.save_task_list();

	// normal exit
	return EXIT_SUCCESS;
}
