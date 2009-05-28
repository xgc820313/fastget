/*
 * =====================================================================================
 *
 *       Filename:  ui.h
 *
 *    Description:  Create GUI of fastget
 *
 *        Version:  1.0
 *        Created:  Friday, May 08, 2009 01:43:04 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon, chinagnu@gmail.com
 *        Company:  Fong's National Engineering Co. Ltd
 *
 * =====================================================================================
 */

#ifndef  UI_H_INC
#define  UI_H_INC

#include	"taskctrl.h"

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

typedef struct 
{
    GtkWidget *dialog;
    GtkWidget *local_name;
    GtkWidget *url;
    GtkWidget *num;
    GtkWidget *auto_flag;

}attrbox_t;

class taskqueue:public TaskCtrl
{
	private:
		static gboolean on_selection_changed(GtkTreeSelection *selection, taskqueue *m_queue);
        static void destroy_attrbox(GtkWidget *widget, gpointer *data);
		GtkListStore *list_model;
        attr_t m_attr;
        attrbox_t m_attrbox;

	protected:
		Conf *m_xml;
	public:
		taskqueue(void)
		{
			m_xml = new Conf();
		}
		~taskqueue(void)
		{
			delete m_xml;
		}

		static void update_task_list(taskqueue* m_queue);
        static void get_task_attribute(GtkWidget *widget, taskqueue *m_queue);
		void create(GtkWidget *vbox);
		void show_list_with_type(gchar index);
		static gboolean action_new_task(void *m_handle);
        gboolean attrbox(taskqueue *m_queue);

		GtkWidget *m_list;
		status_t cur_type;
};

class taskview
{
	private:

	public:
		taskview(void){}
		~taskview(void){}
		void create(GtkWidget *vbox);
};

class tasklist
{
	private:
		GtkWidget* vpaned;

	public:

		taskqueue *m_queue;
		taskview *m_view;

		tasklist(void)
		{
			m_queue = new taskqueue();
			m_view = new taskview();
		}
		~tasklist(void)
		{
			delete m_queue;
			delete m_view;
		}

		void create(GtkWidget *hpaned);
};

class toolbar
{
	private:
		static void search_callback(GtkWidget *widget, toolbar *m_toolbar);
		static void about_callback(GtkWidget *widget, toolbar *m_toolbar);
		static void on_task_up_callback(GtkWidget *widget, GdkEvent *event, taskqueue *m_queue);
		static void on_task_down_callback(GtkWidget *widget, GdkEvent *event, taskqueue *m_queue);
		static gboolean on_add_task_callback(GtkWidget *window, GdkEvent *event, taskqueue *m_queue);
		static void setting_callback(GtkWidget *widget, toolbar *m_toolbar);
		static void stop_task_callback(GtkWidget *widget, toolbar *m_toolbar);

	public:   
		toolbar(void){}
		~toolbar(void){}
		void create(GtkWidget *vbox);

		GtkWidget* ShowListButton;
};

class fastget
{
	private:
		static gboolean on_delete_event ( GtkWidget *window, GdkEvent *event , fastget *m_fastget )
		{
			if(m_fastget->ui_update_flag >0)
			{
				gtk_timeout_remove(m_fastget->ui_update_flag);
			}
			gtk_main_quit();
			return true;
		}
		static gboolean on_destroy_event ( GtkWidget *window,GdkEvent *event,fastget *m_fastget )
		{
			if(m_fastget->ui_update_flag >0)
			{
				gtk_timeout_remove(m_fastget->ui_update_flag);
			}
			gtk_main_quit();
			return true;
		}

		toolbar *m_toolbar;
		gboolean create ( gchar *title, guint width, guint height );
	protected:

	public:

		splash *m_splash;
		tasklist *m_list;
		menutree *m_tree;
		int ui_update_flag;

		fastget ( void )
		{
			m_splash = new splash();
			m_toolbar = new toolbar();
			m_tree = new menutree();
			m_list = new tasklist();
			tooltips = gtk_tooltips_new();

            mutex = g_mutex_new();
            cond = g_cond_new(); 
		}
		~fastget ( void )
		{
			if(ui_update_flag >0 )
			{
				gtk_timeout_remove(ui_update_flag);
			}
			delete m_splash;
			delete m_toolbar;
			delete m_tree;
			delete m_list;

            g_mutex_free(mutex);
            g_cond_free(cond); 
		}
		void init(gchar *title, guint width, guint height );
		static gboolean on_quit(GtkWidget *window, GdkEvent *event, fastget *m_fastget)
		{
			if(m_fastget->ui_update_flag >0)
			{
				gtk_timeout_remove(m_fastget->ui_update_flag);
			}
			gtk_main_quit();
			return true;
		}

		GtkWidget *win;
		GtkWidget* hpaned;
		GtkTooltips *tooltips;
		gboolean show_hide_flag;

        GMutex *mutex;
        GCond *cond;
};

#endif   /* ----- #ifndef UI_H_INC  ----- */
