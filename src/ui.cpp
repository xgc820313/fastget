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
 * @file   ui.cpp
 * @author dragon <chinagnu@gmail.com>
 * @date   Thu Jul  9 16:27:05 2009
 * 
 * @brief  generate fastget GUI and operation this UI
 * 
 * 
 */

#include "ui.h"

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
	gchar *copyright = "Copyright (C) 2009-2011 dragon all rights reserved ® ";
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
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(setting_callback),NULL);
	//g_signal_connect(G_OBJECT(ShowListButton),"enter_notify_event", G_CALLBACK(stardict_on_enter_notify), NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("help on line"),NULL);	 
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
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(about_callback),NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("about fastget"),NULL);	 
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
	gtk_tooltips_set_tip(ui->tooltips,ShowListButton,_("fastget exit"),NULL);	 
	gtk_widget_show(ShowListButton);
}

void toolbar::setting_callback(GtkWidget *widget, toolbar *m_toolbar)
{
	;
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

void toolbar::about_callback(GtkWidget *widget, toolbar *m_toolbar)
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
//			"project site", "http://code.google.com/p/fastget/",
		"website", "http://www.gnumac.cn/project.php?id=2",
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
//	m_view->create(v_vbox);

	gtk_paned_set_position(GTK_PANED(vpaned), 140);
	gtk_box_pack_start(GTK_BOX(m_tasklist),vpaned,TRUE,TRUE,0);
	gtk_widget_show(vpaned);	
}


// create the gui of type tree of fastget
void menutree::create(GtkWidget *vbox)
{
	GtkTreeIter iter,item[4];
	gchar *items[] = {_("task running"),_("task wait"),_("task cannel"),_("task finish")};

	tree_model = gtk_tree_store_new(1,G_TYPE_STRING,G_TYPE_STRING);
	gtk_tree_store_append(tree_model, &iter, NULL); 
	gtk_tree_store_set(
		tree_model, 
		&iter,
		0, "fastget V0.4",
		-1);

	for(guint i=0;i<4;i++)
	{
		gtk_tree_store_append(tree_model, &item[i], &iter); 
		gtk_tree_store_set(tree_model, &item[i],0, items[i],-1);
	}	
	m_menutree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_model));

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes (
		_("task status types list"), 
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
//		ui->m_list->m_queue->show_list_with_type(*(index));
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
		"Status",
		"Name",
		"Progress",
		"Speed",
		"Time",
		"Size",
		"Redirect",
		"URL"};

	list_model = gtk_list_store_new(
		8,
		G_TYPE_STRING,
		G_TYPE_STRING,
//		G_TYPE_UINT64,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_STRING,
		G_TYPE_UINT,
		G_TYPE_STRING);

	m_list = gtk_tree_view_new_with_model(GTK_TREE_MODEL(list_model));

	GtkTreeViewColumn *column; 
	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	for(guint i=0;i<8;i++)
	{
		column= gtk_tree_view_column_new_with_attributes (types[i], renderer,"text", i, NULL);
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

	update_task_list(this);
}

void taskqueue::show_list_with_type(gchar index)
{
	switch(index)
	{
	case '0':
		cur_type=START;
		break;
	case '1':
		cur_type=PAUSE;
		break;
	case '2':
		cur_type=ERROR;
		break;
	case '3':
		cur_type=FINISH;
		break;
	default:
		cur_type=WAIT;
		break;
	}	
}

gboolean taskqueue::on_selection_changed(GtkTreeSelection *selection, taskqueue *m_queue)
{
	GtkTreeModel *model;
	GtkTreeIter iter;

	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		cout<<"hi,test"<<endl;
		gchar *path_str = gtk_tree_model_get_string_from_iter(model,&iter);
		if (!strchr(path_str, ':')) 
		{
			g_free(path_str);
			return false;
		}
		cout<<path_str<<endl;
		g_free(path_str);

		// 		 gchar *cur_item;
		// 		 gtk_tree_model_get(model, &iter, 0, &cur_item, -1);
		// 		 g_printf(cur_item);
		// 		 g_free(cur_item);
	}
	return true;
}

void taskqueue::update_task_list(taskqueue *m_queue)
{
//	GtkListStore *m_model = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(m_list)));

	bool display_enable=false;	
	gchar* task_status=NULL;
	GtkTreeIter iter;

//	gtk_list_store_clear(m_queue->list_model);
	for(unsigned int i=0;i<ui->m_task_list.size();i++)
	{
		if(ui->m_task_list[i] != NULL)
		{
			status_t flag = ui->m_task_list[i]->m_attr.get_status();
			switch(flag)
			{
			case START:
				task_status = "  >>   ";
				if(m_queue->cur_type == START)
				{
					display_enable=true;
				}
				break;
			case PAUSE:
				task_status=  "   | |  ";
				if(m_queue->cur_type == START || m_queue->cur_type == PAUSE)
				{
					display_enable=true;
				}
				break;
			case WAIT:
				task_status = "   [W]  ";
				if(m_queue->cur_type == START || m_queue->cur_type == PAUSE)
				{
					display_enable=true;
				}
				break;
			case ERROR:
				task_status = "   (X)  ";
				if(m_queue->cur_type == ERROR)
				{
					display_enable=true;
				}
				break;
			case CANCEL:
				task_status = "   [Q]  ";
				if(m_queue->cur_type == ERROR)
				{
					display_enable=true;
				}
				break;
			case FINISH:
				task_status = "   [V]  ";
				if(m_queue->cur_type == FINISH)
				{
					display_enable=true;
				}
				break;
			default:
				task_status = "   [W]  ";
				if(m_queue->cur_type == START || m_queue->cur_type == PAUSE)
				{
					display_enable=true;
				}
				break;
			}

			if(display_enable)
			{
				cout<<ui->m_task_list.size()<<endl;

				gtk_list_store_append(m_queue->list_model, &iter); 
				gtk_list_store_set(
					m_queue->list_model, 
					&iter,
					0,task_status,
					1,ui->m_task_list[i]->m_attr.get_file_name(),
					2,ui->m_task_list[i]->m_attr.get_progress(),
					3,ui->m_task_list[i]->m_attr.get_speed(),
					4,ui->m_task_list[i]->m_attr.get_time_right(),
					5,ui->m_task_list[i]->m_attr.get_size(),
					6,ui->m_task_list[i]->m_attr.get_target_name(),
					7,ui->m_task_list[i]->m_attr.get_url()->c_str(),
					-1);

				display_enable=false;
			}
		}
	}
}
