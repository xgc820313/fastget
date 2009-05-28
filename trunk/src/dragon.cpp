/*
 * =====================================================================================
 *
 *       Filename:  fastget.cpp
 *
 *    Description:  basic connect operation
 *
 *        Version:  1.0
 *        Created:  Wednesday, May 07, 2009 01:45:11 HKT
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  dragon, chinagnu@gmail.com
 *        Company:  Fong's National Engineering Co. Ltd
 *
 * =====================================================================================
 */

#include	"ui.h"

fastget *m_fastget = (fastget* ) NULL;

int main ( int argc, char *argv[] )
{
	if ( !g_thread_supported() )
		g_thread_init ( NULL );
	gdk_threads_init();
	gtk_init ( &argc, &argv );

	fastget *m_start = new fastget();
	m_fastget = m_start;

	m_start->init ( "fastget Downloader V0.1",720,480);

	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
