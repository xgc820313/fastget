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
 * @file       ui.cpp
 *
 * @author     dragon <chinagnu@gmail.com>
 * @date       Thu Jul  24th 11:39:08 2009
 * @version    0.2
 * @brief      bug fixed for update_task_list
 *
 * @author     dragon <chinagnu@gmail.com>
 * @date       Thu Jul  9 16:27:05 2009
 * @version    0.1
 * @brief      generate fastget GUI and operation this UI
 * 
 * @todo       unknow
 * @bug        unknow
 */

#include "ui.h"
#include "rc.h"

extern UI* ui;

// splash in the start time
void UI::splash_screen(gchar *info)
{
	gtk_window_set_auto_startup_notification(FALSE);

	GtkWidget *window = gtk_window_new(GTK_WINDOW_POPUP);
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);

	text = GTK_LABEL(gtk_label_new(info));
	gtk_label_set_line_wrap(text, TRUE); 
	gtk_label_set_justify(text, GTK_JUSTIFY_CENTER);
	gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(text),false,false,0);

	progress = GTK_PROGRESS_BAR(gtk_progress_bar_new());
	gtk_box_pack_start(GTK_BOX(vbox),GTK_WIDGET(progress),false,false,0);
	gtk_widget_set_size_request(GTK_WIDGET(progress), -1, 10);
	gtk_progress_bar_pulse(progress);

	gtk_widget_show_all(vbox);
	gtk_container_add(GTK_CONTAINER(window), vbox);

	gtk_window_set_title (GTK_WINDOW (window), "loading");
	gtk_window_set_position(GTK_WINDOW (window), GTK_WIN_POS_CENTER);

	gtk_widget_show(window);

	while(gtk_events_pending())
		gtk_main_iteration();	 

	// it will be called after gtk_main() is called.
	gtk_init_add((GtkFunction)on_splash_screen,(gpointer)window);

	gtk_window_set_auto_startup_notification(TRUE);
}

void UI::show_url(const gchar *url)
{
#ifdef _CONFIG_GPE
	 gchar *command = g_strdup_printf("gpe-mini-browser %s", url);
	 system(command);
	 g_free(command);
else
	 gnome_url_show(url, NULL);

#endif
}

gboolean UI::create(gchar *title, guint width, guint height )
{
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(win), title);
	gtk_container_set_border_width(GTK_CONTAINER(win), 10);
	gtk_window_set_default_size(GTK_WINDOW(win), width, height);
	gtk_window_set_position(GTK_WINDOW(win), GTK_WIN_POS_CENTER);
	gtk_widget_realize(win);

	g_signal_connect( 
		G_OBJECT(win),
		"delete_event",
		G_CALLBACK(on_delete_event),
		this);

	g_signal_connect( 
		G_OBJECT(win),
		"destroy_event",
		G_CALLBACK(on_destroy_event),
		this);

	// create menu bar on here
	GtkWidget *box = gtk_vbox_new(FALSE,0);
	gtk_widget_show(box);
	m_toolbar->create(box);	

	// build work space on here
	hpaned = gtk_hpaned_new();
	m_list->create(hpaned);

	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(vbox);
	gtk_paned_pack1(GTK_PANED(hpaned),vbox,FALSE,TRUE);
	m_tree->create(vbox);

	gtk_paned_set_position(GTK_PANED(hpaned), 140);
	gtk_box_pack_start(GTK_BOX(box),hpaned,TRUE,TRUE,0);
	gtk_widget_show(hpaned);

	// create statusbar
	gchar *copyright = "Copyright (C) 2009-2011 dragon all rights reserved ®   (http://www.gnumac.cn/)";
	GtkWidget *status_bar = gtk_statusbar_new ();      
	gtk_box_pack_start (GTK_BOX(box), status_bar, false, false, 0);
	gtk_widget_show (status_bar);

	guint context_id = gtk_statusbar_get_context_id(
		GTK_STATUSBAR (status_bar), copyright);
	gtk_statusbar_push(GTK_STATUSBAR(status_bar),context_id, copyright);

	gtk_container_add(GTK_CONTAINER(win),box);
	gtk_widget_show(win);

	return true;
}

void toolbar::create(GtkWidget *vbox)
{
	GtkWidget *hbox;
	hbox = gtk_hbox_new(false,0);
	gtk_widget_show(hbox);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(vbox),hbox,false,false,3);
#endif

	GtkWidget *image;
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_HOME,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(setting_callback),this);
	//g_signal_connect(G_OBJECT(ShowListButton),"enter_notify_event", G_CALLBACK(stardict_on_enter_notify), NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("indroduction"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_ADD,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(on_add_task_callback),ui->m_list->m_queue);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("add new task"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_GO_UP,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
//	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(on_task_up_callback),ui->m_list->m_queue);
	//	 g_signal_connect(G_OBJECT(ShowListButton),"enter_notify_event", G_CALLBACK(stardict_on_enter_notify), NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("task up"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_GO_DOWN,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
//	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(on_task_down_callback),ui->m_list->m_queue);
	//	 g_signal_connect(G_OBJECT(ShowListButton),"enter_notify_event", G_CALLBACK(stardict_on_enter_notify), NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("task down"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_INFO,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(stop_task_callback),NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("action all tasks"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_HELP,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(about_callback),this);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("help online"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_CANCEL,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(ui->on_quit),ui);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("system exit"),NULL);	 
	gtk_widget_show(ShowListButton);
}

void toolbar::setting_callback(GtkWidget *widget, GdkEvent *event, toolbar *m_toolbar)
{
	ui->show_url("http://code.google.com/p/fastget/");
}

void toolbar::stop_task_callback(GtkWidget *widget, toolbar *m_toolbar)
{
	;
}

gboolean toolbar::on_add_task_callback(GtkWidget *window, GdkEvent *event, taskqueue *m_queue)
{
	if(m_queue != NULL)
	{
		return true;
	}
	return false;
}

void toolbar::about_callback(GtkWidget *widget, GdkEvent *event, toolbar *m_toolbar)
{
	const gchar *authors[] = {
		"dragon <chinagnu@gmail.com>",
		NULL
	};
	gchar *documenters[] = {
		"dragon <chinagnu@gmail.com>",
		NULL
	};
	gchar *translator_credits = _("translator_credits");

	gtk_show_about_dialog(
		GTK_WINDOW(ui->win),
		"name", _("fastget"),
		"version", VERSION,
		"website", "http://code.google.com/p/fastget/",
// 		"website", "http://www.gnumac.cn/project.php?id=2",
		"comments", _("fastget is a cross-platform multi-thread downloader which works with http,ftp,mms and p2p protocol\n\n"),
		"copyright", _("Copyright (C) 2009-2011 by dragon(肖皓天)"),
		"authors", (const char **)authors,
		"documenters", (const char **)documenters,
		"translator-credits", strcmp(translator_credits,"translator_credits") != 0 ? translator_credits : NULL,
		NULL);
}


// crate the UI of task list area
void tasklist::create(GtkWidget *hpaned)
{
	GtkWidget *m_tasklist = gtk_vbox_new(FALSE, 0);
	gtk_paned_pack2(GTK_PANED(hpaned),m_tasklist,false,false);
	gtk_widget_show(m_tasklist);

	vpaned = gtk_vpaned_new();
	GtkWidget *q_hbox = gtk_hbox_new(FALSE, 0);
	gtk_widget_show(q_hbox);
	gtk_paned_pack1(GTK_PANED(vpaned),q_hbox,FALSE,FALSE);
	m_queue->create(q_hbox);

	GtkWidget *v_vbox = gtk_vbox_new(FALSE, 0);
	gtk_widget_show(v_vbox);
	gtk_paned_pack2(GTK_PANED(vpaned),v_vbox,FALSE,FALSE);
	m_info->create(v_vbox);

	gtk_paned_set_position(GTK_PANED(vpaned), 140);
	gtk_box_pack_start(GTK_BOX(m_tasklist),vpaned,TRUE,TRUE,0);
	gtk_widget_show(vpaned);	
}


// create the gui of type tree of fastget
void menutree::create(GtkWidget *vbox)
{
	GtkTreeIter iter,item[4];
	gchar *items[] = {"work","error","finish"};

	tree_model = gtk_tree_store_new(1,G_TYPE_STRING,G_TYPE_STRING);
	gtk_tree_store_append(tree_model, &iter, NULL); 
	gtk_tree_store_set(
		tree_model, 
		&iter,
		0, "fastget V0.4",
		-1);

	for(guint i=0;i<3;i++)
	{
		gtk_tree_store_append(tree_model, &item[i], &iter); 
		gtk_tree_store_set(tree_model, &item[i],0, items[i],-1);
	}	
	m_menutree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_model));

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes (
		_("task status list"), 
		renderer,
		"text", 0, NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(m_menutree), column);

	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(m_menutree), TRUE);
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(m_menutree));
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(m_menutree), FALSE);
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(m_menutree), TRUE);
	gtk_tree_view_expand_all(GTK_TREE_VIEW(m_menutree));

	GtkTreeSelection *m_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(m_menutree));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(m_selection),GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(m_selection), "changed", G_CALLBACK(on_selection_changed), this);	

	GtkWidget *scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show(scrolledwindow);
	gtk_scrolled_window_set_shadow_type(
		GTK_SCROLLED_WINDOW (scrolledwindow),
		GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy(
		GTK_SCROLLED_WINDOW(scrolledwindow),
		GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);	
	gtk_container_add(
		GTK_CONTAINER(scrolledwindow),
		m_menutree);

	gtk_box_pack_start(GTK_BOX(vbox),scrolledwindow,TRUE,TRUE,0);	
	gtk_widget_show(m_menutree);
}

void menutree::destory(void)
{
	g_object_unref (tree_model);
}

gboolean menutree::on_selection_changed(GtkTreeSelection *selection, menutree *m_tree)
{
	GtkTreeModel *model;
	GtkTreeIter iter;

	if (gtk_tree_selection_get_selected (selection, &model, &iter))
	{
		gchar *path_str = gtk_tree_model_get_string_from_iter(model,&iter);
		if (!strchr(path_str, ':')) 
		{
			g_free(path_str);
			return false;
		}
		gchar *index = (strchr(path_str,':')+1);
		ui->m_list->m_queue->show_list_with_type(*(index));
		g_free(path_str);
	}

	return true;
}


void taskqueue::create(GtkWidget *hbox)
{
	// 	m_list = gtk_clist_new(8);
	// 	gtk_clist_set_selection_mode(GTK_CLIST(m_list),GTK_SELECTION_SINGLE);
	// 	gtk_clist_column_titles_active(GTK_CLIST(m_list));
	// 	gtk_clist_column_titles_show(GTK_CLIST(m_list));

	// create column titles
	gchar *types[]= {
		"I",
		"status",
		"name",
		"task progress",
		"speed",
		"time right",
		"size",
		"redirect",
		"url"};

	list_model = gtk_list_store_new(
		9,
		G_TYPE_UINT,
		G_TYPE_OBJECT,
		G_TYPE_STRING,
		G_TYPE_UINT,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_UINT,
		G_TYPE_STRING);

	m_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_model));

	GtkTreeViewColumn *column; 
	GtkCellRenderer *renderer;
	for(guint i=0;i<9;i++)
	{
		if( i == 1)
		{
			renderer = gtk_cell_renderer_pixbuf_new();
			column= gtk_tree_view_column_new_with_attributes (types[i], renderer,"pixbuf", i, NULL);
		}
		else if( i == 3)
		{
			renderer = gtk_cell_renderer_progress_new();
			column= gtk_tree_view_column_new_with_attributes (types[i], renderer,"value", i, NULL);
		}
		else
		{
			renderer = gtk_cell_renderer_text_new();
			column= gtk_tree_view_column_new_with_attributes (types[i], renderer,"text", i, NULL);
		}
		gtk_tree_view_column_set_resizable(column,TRUE);
		gtk_tree_view_append_column(GTK_TREE_VIEW(m_list), column);
	}

	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(m_list), TRUE);
	gtk_tree_view_columns_autosize(GTK_TREE_VIEW(m_list));
	gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(m_list), TRUE);
	gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(m_list), TRUE);

	GtkTreeSelection *m_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(m_list));
	gtk_tree_selection_set_mode(GTK_TREE_SELECTION(m_selection),GTK_SELECTION_SINGLE);
	g_signal_connect(G_OBJECT(m_selection), "changed", G_CALLBACK(on_selection_changed), this);	

	GtkWidget *scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show(scrolledwindow);
	gtk_scrolled_window_set_shadow_type(
		GTK_SCROLLED_WINDOW (scrolledwindow),
		GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy(
		GTK_SCROLLED_WINDOW(scrolledwindow),
		GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);	
	gtk_container_add(
		GTK_CONTAINER(scrolledwindow),
		m_list);

	gtk_box_pack_start(GTK_BOX(hbox),scrolledwindow,TRUE,TRUE,0);	
	gtk_widget_show(m_list);
}

void taskqueue::show_list_with_type(gchar index)
{
	switch(index)
	{
	case '0':
		cur_type=START;
		break;
	case '1':
		cur_type=ERROR;
		break;
	case '2':
		cur_type=FINISH;
		break;
	default:
		cur_type=WAIT;
		break;
	}

	if(ui->m_list->m_queue->cur_type == ERROR ||
	 ui->m_list->m_queue->cur_type == CANCEL ||
	 ui->m_list->m_queue->cur_type == FINISH)
	{
		ui->m_list->m_info->clear_task_thread_page();
	}
}

gboolean taskqueue::on_selection_changed(GtkTreeSelection *selection, taskqueue *m_queue)
{
	if(m_queue == NULL)
		return false;

	GtkTreeModel *model;
	GtkTreeIter iter;

	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
// 		gchar *path_str = gtk_tree_model_get_string_from_iter(model,&iter);
// 		cout<<path_str<<endl;
// 		g_free(path_str);

		unsigned int num=0;
		gtk_tree_model_get(model, &iter, 0, &num, -1);
		if(ui->m_task_list[num-1]->m_attr.get_status() == START)
			ui->m_list->m_info->add_task_thread_page(ui->m_task_list[num-1]->m_attr.get_thread_num());
		else
			ui->m_list->m_info->clear_task_thread_page();
	}
	return true;
}

void taskqueue::update_task_list(taskqueue *m_queue)
{
//	GtkListStore *m_model = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(m_list)));
//	while(gtk_events_pending())
//		gtk_main_iteration();

	bool display_enable=false;
	GtkTreeIter iter;
	const char **task_status=NULL;

	gtk_list_store_clear(m_queue->list_model);
	for(unsigned int i=0;i<ui->m_task_list.size();i++)
	{
		if(ui->m_task_list[i] != NULL)
		{
			status_t flag = ui->m_task_list[i]->m_attr.get_status();
			switch(flag)
			{
			case START:
				task_status = (const char**)start_task_xpm;
				if(m_queue->cur_type == START || m_queue->cur_type == WAIT)
				{
					display_enable=true;
				}
				break;
			case PAUSE:
				task_status= (const char**)stop_task_xpm;
				if(m_queue->cur_type == START || m_queue->cur_type == WAIT)
				{
					display_enable=true;
				}
				break;
			case WAIT:
				task_status = (const char**)waiting_task_xpm;
				if(m_queue->cur_type == START || m_queue->cur_type == WAIT)
				{
					display_enable=true;
				}
				break;
			case ERROR:
				task_status = (const char**)error_info_xpm;
				if(m_queue->cur_type == ERROR)
				{
					display_enable=true;
				}
				break;
			case CANCEL:
				task_status = (const char**)cancel_task_xpm;
				if(m_queue->cur_type == ERROR)
				{
					display_enable=true;
				}
				break;
			case FINISH:
				task_status = (const char**)success_xpm;
				if(m_queue->cur_type == FINISH)
				{
					display_enable=true;
				}
				break;
			default:
				task_status = (const char**)waiting_task_xpm;
				if(m_queue->cur_type == START || m_queue->cur_type == WAIT )
				{
					display_enable=true;
				}
				break;
			}

			if(display_enable)
			{
				GdkPixbuf *status_icon = gdk_pixbuf_new_from_xpm_data(task_status);
				gtk_list_store_append(m_queue->list_model, &iter); 
				gtk_list_store_set(
					m_queue->list_model, 
					&iter,
					0,ui->m_task_list[i]->m_attr.get_index(),
					1,status_icon,
					2,ui->m_task_list[i]->m_attr.get_file_name()->c_str(),
					3,ui->m_task_list[i]->m_attr.get_progress(),
					4,ui->m_task_list[i]->m_attr.get_speed()->c_str(),
					5,ui->m_task_list[i]->m_attr.get_time_right()->c_str(),
					6,ui->m_task_list[i]->m_attr.get_target_size()->c_str(),
					7,ui->m_task_list[i]->m_attr.get_target_name(),
					8,ui->m_task_list[i]->m_attr.get_url()->c_str(),
					-1);

				display_enable=false;
			}
		}
	}
}

void taskinfo::create(GtkWidget *vbox)
{
	info = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(info),GTK_POS_TOP);
	gtk_notebook_set_show_border(GTK_NOTEBOOK(info),FALSE);
	gtk_notebook_set_show_tabs(GTK_NOTEBOOK(info),TRUE);
	gtk_notebook_set_scrollable(GTK_NOTEBOOK(info),TRUE);


	GtkWidget *m_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(m_view),FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(m_view),FALSE);

	GtkTextBuffer *buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (m_view));
	insert_basic_message(buffer);

	GtkWidget *scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
	gtk_widget_show(scrolledwindow);
	gtk_scrolled_window_set_shadow_type(
		GTK_SCROLLED_WINDOW (scrolledwindow),
		GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy(
		GTK_SCROLLED_WINDOW(scrolledwindow),
		GTK_POLICY_AUTOMATIC, 
		GTK_POLICY_AUTOMATIC);	
	gtk_container_add(
		GTK_CONTAINER(scrolledwindow),
		m_view);

	GtkWidget *head = gtk_label_new(_("fastget"));
	gtk_notebook_append_page(GTK_NOTEBOOK(info),scrolledwindow,head);


	gtk_box_pack_start(GTK_BOX(vbox),info,TRUE,TRUE,0);
	gtk_widget_show_all(info);
}

void taskinfo::insert_basic_message (GtkTextBuffer *buffer)
{
   GtkTextIter iter;
   gtk_text_buffer_get_iter_at_offset (buffer, &iter, 0);
   gtk_text_buffer_insert (buffer, &iter,
    " fastget is a cross-platform multi-thread downloader.\n"
    " It has been write by pure C++ and GTK graphic user interface develop library.\n"
	"\n"
    " fastget is also an open source and free software release with GPL3\n"
    " current version of this software is V0.4 which is a devel version.\n"
	" version 0.4 of fastget will work with a lot of network protocol!\n"
    " such as http, ftp, mms and p2p.\n"
    "\n"						   
    " I am hard work on it day after night ^_^\n"
    "\n"
    "\t\t\t                                                                 ---- dragon\n"
    "\t\t\t                                                              July 21th, 2009\n",
	-1);
}

void taskinfo::clear_task_thread_page(void)
{
	gint current_page=1;
	do
	{
 		gtk_notebook_remove_page(GTK_NOTEBOOK(info),current_page);
		gtk_notebook_next_page(GTK_NOTEBOOK(info));
		current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(info));
	}while(current_page != 0);
}

void taskinfo::add_task_thread_page(const unsigned int thread_num)
{
	clear_task_thread_page();

	// add task thread pages
	for(unsigned int i=0;i<thread_num;i++)
	{
		GtkListStore *list_model = gtk_list_store_new(1,G_TYPE_STRING);
		
		GtkWidget *msg_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_model));

		GtkTreeViewColumn *column; 
		GtkCellRenderer *renderer;

		renderer = gtk_cell_renderer_text_new();
		column= gtk_tree_view_column_new_with_attributes("message", renderer,"text", 0, NULL);
		gtk_tree_view_append_column(GTK_TREE_VIEW(msg_list), column);
		gtk_tree_view_column_set_resizable(column,TRUE);

		gtk_tree_view_set_rules_hint(GTK_TREE_VIEW(msg_list), TRUE);
		gtk_tree_view_columns_autosize(GTK_TREE_VIEW(msg_list));
 		gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(msg_list), FALSE);
 		gtk_tree_view_set_headers_clickable(GTK_TREE_VIEW(msg_list), FALSE);

		GtkWidget *scrolledwindow = gtk_scrolled_window_new (NULL, NULL);
		gtk_widget_show(scrolledwindow);
		gtk_scrolled_window_set_shadow_type(
			GTK_SCROLLED_WINDOW (scrolledwindow),
			GTK_SHADOW_NONE);
		gtk_scrolled_window_set_policy(
			GTK_SCROLLED_WINDOW(scrolledwindow),
			GTK_POLICY_AUTOMATIC, 
			GTK_POLICY_AUTOMATIC);	
		gtk_container_add(
			GTK_CONTAINER(scrolledwindow),
			msg_list);	
		gtk_widget_show(msg_list);

		// insert message for every thread
		for(int j=0;j<5;j++)
		{
			GtkTreeIter iter;
			gtk_list_store_append(list_model, &iter); 
			gtk_list_store_set(
				list_model, 
				&iter,
				0,"test information hi",
				-1);
		}

		gchar *titles = g_strdup_printf("thread %d",i+1);
		GtkWidget *title = gtk_label_new(titles);
		gtk_notebook_append_page(GTK_NOTEBOOK(info),scrolledwindow,title);
		g_free(titles);
	}
}
