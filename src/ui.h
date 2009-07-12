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
 * @file       ui.h
 * @author     dragon <chinagnu@gmail.com>
 * @date       Fri Jul  3 15:38:44 2009
 * 
 * @brief      header define of GUI
 * @version    0.1
 * 
 * @todo       unknow
 * @bug        unknow
 */

#ifndef  UI_H_INC
#define  UI_H_INC

#include "dragon.h"


class splash
{
private:
	GtkLabel *text;
	GtkProgressBar *progress;

	static void on_splash_screen(gpointer data)
	{
		gtk_widget_destroy(GTK_WIDGET(data));
	}
public:
	splash(void)
	{
		text=NULL;
		progress=NULL;
	}
	~splash(void){}
	void splash_screen(gchar *info);
};


class menutree
{
private:

	GtkTreeStore *tree_model;
	static gboolean on_button_press(GtkWidget * widget, GdkEventButton * event, menutree *m_tree);
	static gboolean on_selection_changed(GtkTreeSelection *selection, menutree *m_tree);

public:
	menutree(void){}
	~menutree(void){}
	void create(GtkWidget *vbox);
	void destory(void);

	GtkWidget *m_menutree;
};

class taskinfo
{
private:

public:
	taskinfo(void){}
//	~taskview(void){}
	void create(GtkWidget *vbox);
};

class tasklist
{
private:
	GtkWidget* vpaned;

public:

//	taskqueue *m_queue;
	taskinfo *m_info;

	tasklist(void)
	{
//		m_queue = new taskqueue();
		m_info = new taskinfo();
	}
	~tasklist(void)
	{
//		delete m_queue;
		delete m_info;
	}

	void create(GtkWidget *hpaned);
};

class toolbar
{
private:
	static void search_callback(GtkWidget *widget, toolbar *m_toolbar);
	static void about_callback(GtkWidget *widget, toolbar *m_toolbar);
//	static void on_task_up_callback(GtkWidget *widget, GdkEvent *event, taskqueue *m_queue);
//	static void on_task_down_callback(GtkWidget *widget, GdkEvent *event, taskqueue *m_queue);
//	static gboolean on_add_task_callback(GtkWidget *window, GdkEvent *event, taskqueue *m_queue);
	static void setting_callback(GtkWidget *widget, toolbar *m_toolbar);
	static void stop_task_callback(GtkWidget *widget, toolbar *m_toolbar);

public:   
	toolbar(void){}
	~toolbar(void){}
	void create(GtkWidget *vbox);

	GtkWidget* ShowListButton;
};


class UI
{
private:
	static gboolean on_delete_event ( GtkWidget *window, GdkEvent *event , UI *m_ui )
	{
		gtk_main_quit();
		return true;
	}
	static gboolean on_destroy_event ( GtkWidget *window,GdkEvent *event,UI *m_ui )
	{
		gtk_main_quit();
		return true;
	}

	toolbar *m_toolbar;
	gboolean create ( gchar *title, guint width, guint height );
protected:

public:

	splash *m_splash;
	menutree *m_tree;

	UI ( void )
	{
		tooltips = gtk_tooltips_new();
		m_splash = new splash();
		m_toolbar = new toolbar();
		m_tree = new menutree();
	}
	~UI ( void )
	{
		delete m_splash;
		delete m_toolbar;
		delete m_tree;
	}

	void init(gchar *title, guint width, guint height );
	static gboolean on_quit(GtkWidget *window, GdkEvent *event, UI *m_ui)
	{
		gtk_main_quit();
		return true;
	}

	GtkWidget *win;
	GtkWidget* hpaned;
	GtkTooltips *tooltips;
	gboolean show_hide_flag;
};

#endif	/*  */
