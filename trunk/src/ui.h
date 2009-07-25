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

#include "conf.h"
#include "attribute.h"

class menutree
{
public:

	menutree(void){}
	~menutree(void){}
	void create(GtkWidget *vbox);
	void destory(void);

	GtkWidget *m_menutree;

private:

	GtkTreeStore *tree_model;
	static gboolean on_button_press(GtkWidget * widget, GdkEventButton * event, menutree *m_tree);
	static gboolean on_selection_changed(GtkTreeSelection *selection, menutree *m_tree);

};

class taskqueue
{
public:

	taskqueue(void){cur_type=WAIT;}
	~taskqueue(void){}

	static void update_task_list(taskqueue* m_queue);
//	static void get_task_attribute(GtkWidget *widget, taskqueue *m_queue);
	void create(GtkWidget *vbox);
	void show_list_with_type(gchar index);
/* 	static gboolean action_new_task(void *m_handle); */
/* 	gboolean attrbox(taskqueue *m_queue); */

	GtkWidget *m_list;
	status_t cur_type;

protected:

	
private:
	static gboolean on_selection_changed(GtkTreeSelection *selection, taskqueue *m_queue);
	static void destroy_attrbox(GtkWidget *widget, gpointer *data);
	GtkListStore *list_model;

};

class taskinfo
{
public:

	taskinfo(void){}
	~taskinfo(void){}
	void create(GtkWidget *vbox);
	void add_task_thread_page(const unsigned int thread_num);
	void clear_task_thread_page(void);

private:

	GtkWidget *info;
	void insert_basic_message (GtkTextBuffer *buffer);
};

class tasklist
{
public:

	taskqueue *m_queue;
	taskinfo *m_info;

	tasklist(void)
	{
		m_queue = new taskqueue();
		m_info = new taskinfo();
	}
	~tasklist(void)
	{
		delete m_queue;
		delete m_info;
	}

	void create(GtkWidget *hpaned);

private:
	GtkWidget* vpaned;

};

class toolbar
{
public:   

	toolbar(void){}
	~toolbar(void){}
	void create(GtkWidget *vbox);

	GtkWidget* ShowListButton;

private:

	static void search_callback(GtkWidget *widget, toolbar *m_toolbar);
	static void about_callback(GtkWidget *widget, GdkEvent *event,toolbar *m_toolbar);
	static gboolean on_add_task_callback(GtkWidget *window, GdkEvent *event, taskqueue *m_queue);
	static void setting_callback(GtkWidget *widget, GdkEvent *event, toolbar *m_toolbar);
	static void stop_task_callback(GtkWidget *widget, toolbar *m_toolbar);
};


class UI
{
public:

	menutree *m_tree;
	tasklist *m_list;

	UI ( vector<Task*>& n_task_list )
	{
		m_task_list = n_task_list;
		tooltips = gtk_tooltips_new();
		m_toolbar = new toolbar();
		m_tree = new menutree();
		m_list = new tasklist();
	}
	~UI ( void )
	{
		delete m_toolbar;
		delete m_tree;
		delete m_list;
	}

	void splash_screen(gchar *info);
	void show_url(const gchar *url);
	gboolean create ( gchar *title, guint width, guint height );
	static gboolean on_quit(GtkWidget *window, GdkEvent *event, UI *m_ui)
	{
		gtk_main_quit();
		return true;
	}

	GtkWidget *win;
	GtkWidget *hpaned;
	GtkTooltips *tooltips;
	gboolean show_hide_flag;
	vector<Task*> m_task_list;

protected:

	

private:

	static gboolean on_delete_event ( GtkWidget *window, GdkEvent *event, UI *m_ui )
	{
		gtk_main_quit();
		return true;
	}
	static gboolean on_destroy_event ( GtkWidget *window,GdkEvent *event, UI *m_ui )
	{
		gtk_main_quit();
		return true;
	}

	static void on_splash_screen(gpointer data)
	{
		gtk_widget_destroy(GTK_WIDGET(data));
	}

	GtkLabel *text;
	GtkProgressBar *progress;

	toolbar *m_toolbar;

};

#endif	/*  */
