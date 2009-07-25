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

#include "ui.h"
#include "conf.h"

UI *ui = (UI*)NULL;

int main(int argc,char* argv[])
{
	// gtk environment check 
	if ( !g_thread_supported() )
		g_thread_init ( NULL );

	gdk_threads_init();
	gtk_init ( &argc, &argv );

	Conf m_conf;
	m_conf.import_task_list();
	
	UI *m_ui = new UI(m_conf.get_task_list());
	if(m_ui != NULL)
		ui = m_ui;
	else
		return false;

	if(m_ui->create(_("(快活)fastget downloader V0.4"),720,480))
	{
		gdk_threads_enter();
		m_ui->splash_screen("Loading...");
		m_ui->m_list->m_queue->update_task_list(m_ui->m_list->m_queue);
		g_timeout_add(500,(GtkFunction)m_ui->m_list->m_queue->update_task_list,m_ui->m_list->m_queue);
		gtk_main();
		gdk_threads_leave();

// 		GThread *fd;
// 		GError *thread_error=NULL;
// 		if((fd=g_thread_create((GThreadFunc)m_ui->m_list->m_queue->update_task_list,(void*)m_ui->m_list->m_queue,true,&thread_error))==NULL)
// 		{
// 			g_error_free(thread_error);
// 		}
// 		g_thread_join(fd);
	}
	else
		return false;

	m_conf.save_task_list();

	// normal exit
	return EXIT_SUCCESS;
}
