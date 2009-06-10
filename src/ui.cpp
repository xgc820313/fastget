/*
 * =====================================================================================
 *
 *       Filename:  ui.cpp
 *
 *    Description:  build the desktop of fastget GUI
 *
 *        Version:  1.0
 *        Created:  Friday, May 08, 2009 01:41:27 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon, chinagnu@gmail.com
 *        Company:  Fong's National Engineering Co. Ltd
 *
 * =====================================================================================
 */
#include	"ui.h"
extern fastget* m_fastget;

// create task type tree view
void menutree::create(GtkWidget *vbox)
{
	GtkTreeIter iter,item[4];
	gchar *items[] = {"runing","waiting","cancel","finish"};

	tree_model = gtk_tree_store_new(1,G_TYPE_STRING,G_TYPE_STRING);
	gtk_tree_store_append(tree_model, &iter, NULL); 
	gtk_tree_store_set(
			tree_model, 
			&iter,
			0, "FastGet",
			-1);

	for(guint i=0;i<4;i++)
	{
		gtk_tree_store_append(tree_model, &item[i], &iter); 
		gtk_tree_store_set(tree_model, &item[i],0, items[i],-1);
	}	
	m_menutree = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_model));

	GtkCellRenderer *renderer = gtk_cell_renderer_text_new ();
	GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes (
			"Task Status List", 
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
		m_fastget->m_list->m_queue->show_list_with_type(*(index));
		g_free(path_str);

		// 		 gchar *cur_item;
		// 		 gtk_tree_model_get(model, &iter, 0, &cur_item, -1);
		// 		 g_printf(cur_item);
		// 		 g_free(cur_item);
	}

	return true;
}

// create task list view 
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
	m_view->create(v_vbox);

	gtk_paned_set_position(GTK_PANED(vpaned), 160);
	gtk_box_pack_start(GTK_BOX(m_tasklist),vpaned,TRUE,TRUE,0);
	gtk_widget_show(vpaned);	
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
		"Size",
		"Progress",
		"Speed",
		"Time",
		"Retry",
		"URL"};

	list_model = gtk_list_store_new(
			8,
			G_TYPE_STRING,
			G_TYPE_STRING,
			G_TYPE_UINT64,
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
	//	g_signal_connect(G_OBJECT(m_selection), "changed", G_CALLBACK(on_selection_changed), this);	

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

	m_fastget->ui_update_flag = g_timeout_add(500,(GtkFunction)m_fastget->m_list->m_queue->update_task_list,m_fastget->m_list->m_queue);
//    g_idle_add((GtkFunction)m_fastget->m_list->m_queue->update_task_list,m_fastget->m_list->m_queue);
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
	return true;
}

void taskqueue::update_task_list(taskqueue *m_queue)
{
//	GtkListStore *m_model = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(m_list)));

	int i=0; 
	bool display_enable=false;	
	gchar* task_status=NULL;
	GtkTreeIter iter;
	TaskNode *task_list = m_queue->get_task_list();

	gtk_list_store_clear(m_queue->list_model);
	while(task_list != NULL)
	{
		Task *cur_task = m_queue->get_task(i);
		if(cur_task != NULL)
		{
			status_t flag = cur_task->get_task_status();
			switch(flag)
			{
				case START:
					task_status = "   >>   ";
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
				gtk_list_store_append(m_queue->list_model, &iter); 
				gtk_list_store_set(m_queue->list_model, &iter,
						0,task_status,
						1,cur_task->get_task_file(),
						2,cur_task->get_file_size(),
						3,cur_task->get_task_progress(),
						4,cur_task->get_task_speed(),
						5,cur_task->get_task_time_right(),
						6,cur_task->get_retry_times(),
						7,cur_task->get_task_url(),
						-1);

				display_enable=false;
			}
		}
		task_list = task_list->next;
		i++;
	}
}

void taskview::create(GtkWidget *vbox)
{
	GtkWidget *m_view = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(m_view),FALSE);
	gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(m_view),FALSE);

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

	gtk_box_pack_start(GTK_BOX(vbox),scrolledwindow,TRUE,TRUE,0);
	gtk_widget_show(m_view);
}

void taskqueue::destroy_attrbox(GtkWidget *widget, gpointer *data)
{
    gtk_widget_destroy(GTK_WIDGET(data));
}

gboolean taskqueue::action_new_task(void* m_handle)
{
	taskqueue *m_queue = (taskqueue *)m_handle;
	if(m_queue->get_task_list() != NULL)
	{
		int last_task_id = m_queue->get_task_num();
		Task *cur_task = m_queue->get_task(last_task_id-1);
		if(cur_task != NULL)
		{
			if(cur_task->action_task())
			{
				return true;
			}
		}
	}
    return false;
}

void taskqueue::get_task_attribute(GtkWidget *widget, taskqueue *m_queue)
{
    memset(&m_queue->m_attr, 0, sizeof(m_queue->m_attr));

    gboolean a_flag = false;
    if(GTK_TOGGLE_BUTTON(m_queue->m_attrbox.auto_flag)->active)
        a_flag = true;
    const gchar *url_text = gtk_entry_get_text(GTK_ENTRY(m_queue->m_attrbox.url));
    const gchar *file_name = gtk_entry_get_text(GTK_ENTRY(m_queue->m_attrbox.local_name));
    const gchar *thread_no = gtk_entry_get_text(GTK_ENTRY(m_queue->m_attrbox.num));

	if(strlen(url_text)>0 && strlen(file_name)>0 && atoi(thread_no) > 0) 
	{
		m_queue->m_attr.id=m_queue->get_task_num();

		int url_length = strlen(g_strdup(url_text));
		if(url_length < MAX_STRING)
		{
			sprintf(m_queue->m_attr.url,"%s",g_strdup(url_text));
            m_queue->m_attr.url[url_length]=0;
		}
		else
		{
			strncpy(m_queue->m_attr.url,g_strdup(url_text),MAX_STRING);
		}
//		m_queue->m_attr.url = g_strdup(url_text);

		if(!a_flag)
			m_queue->m_attr.STATUS = WAIT;
		else
			m_queue->m_attr.STATUS = START;
		m_queue->m_attr.thread_no=atoi(thread_no);
		m_queue->m_attr.size=0;
		strncpy(m_queue->m_attr.local_file_name,g_strdup(file_name),strlen(g_strdup(file_name)));
		m_queue->m_attr.auto_flag=a_flag;
		m_queue->m_attr.retry=0;
		strncpy(m_queue->m_attr.speed,"0k/s",strlen("0k/s"));
		strncpy(m_queue->m_attr.progress,"0%",strlen("0%"));
		strncpy(m_queue->m_attr.time_right,"0h:0m:0s",strlen("0h:0m:0s"));
		m_queue->m_attr.start_time=0;

		if(m_queue->task_list_add(m_queue->m_attr))
		{
			GThread *fd;
			GError *thread_error=NULL;

			if((fd=g_thread_create((GThreadFunc)m_fastget->m_list->m_queue->action_new_task,(void*)m_fastget->m_list->m_queue,true,&thread_error))==NULL)
			{
				g_error_free(thread_error);
			}
//			g_thread_join(fd);
		}
	}
    gtk_widget_destroy(GTK_WIDGET(m_queue->m_attrbox.dialog));
}

gboolean taskqueue::attrbox(taskqueue *m_queue)
{
	GtkWidget *box,*label,*filename,*url_box, *file_box;
    memset(&m_queue->m_attrbox,0,sizeof(m_queue->m_attrbox));

    m_queue->m_attrbox.dialog = gtk_dialog_new();
    gtk_window_set_title(GTK_WINDOW(m_queue->m_attrbox.dialog), "task attribute");

//    dialog = gtk_dialog_new_with_buttons ("Task Attribute",
//            NULL,
//            GTK_DIALOG_MODAL,
//            GTK_STOCK_CANCEL, 
//            GTK_RESPONSE_REJECT,
//            GTK_STOCK_OK, 
//            GTK_RESPONSE_OK,
//            NULL);
	gtk_window_set_default_size(GTK_WINDOW(m_queue->m_attrbox.dialog), 360,200);
	gtk_window_set_position(GTK_WINDOW(m_queue->m_attrbox.dialog), GTK_WIN_POS_CENTER);
    gtk_window_set_resizable(GTK_WINDOW(m_queue->m_attrbox.dialog), FALSE);
    gtk_window_set_modal(GTK_WINDOW(m_queue->m_attrbox.dialog),true); 

    GtkWidget *ok_button = gtk_button_new_with_label("OK");
    gtk_box_pack_end(GTK_BOX(GTK_DIALOG(m_queue->m_attrbox.dialog)->action_area),ok_button,true,true,5);
	g_signal_connect( 
			G_OBJECT(ok_button),
			"clicked",
			G_CALLBACK(get_task_attribute),
			m_fastget->m_list->m_queue);
    GtkWidget *cancel_button = gtk_button_new_with_label("Cancel");
    gtk_box_pack_end(GTK_BOX(GTK_DIALOG(m_queue->m_attrbox.dialog)->action_area),cancel_button,true,true,5);
	g_signal_connect( 
			G_OBJECT(cancel_button),
			"clicked",
			G_CALLBACK(destroy_attrbox),
			m_queue->m_attrbox.dialog);

	box = gtk_vbox_new(false,5);
	url_box = gtk_hbox_new(false,5);

	//url 
	label = gtk_label_new ("URL: ");
	gtk_box_pack_start(GTK_BOX(url_box),label,false,false,5);

	m_queue->m_attrbox.url = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(m_queue->m_attrbox.url),256);
	gtk_box_pack_start(GTK_BOX(url_box),m_queue->m_attrbox.url,true,true,15);
	gtk_box_pack_start(GTK_BOX(box),url_box,false,false,5);

	//save file as 
	file_box = gtk_hbox_new(false,5);
	filename = gtk_label_new ("Save AS: ");
	gtk_box_pack_start(GTK_BOX(file_box),filename,false,false,5);

	m_queue->m_attrbox.local_name = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(m_queue->m_attrbox.local_name),256);
	gtk_box_pack_start(GTK_BOX(file_box),m_queue->m_attrbox.local_name,true,true,15);
	gtk_box_pack_start(GTK_BOX(box),file_box,false,false,5);


	// thread number setting
	GtkWidget *thread_box = gtk_hbox_new(false,5);

	m_queue->m_attrbox.auto_flag = gtk_check_button_new_with_label(" Auto ");
	gtk_box_pack_start(GTK_BOX(thread_box),m_queue->m_attrbox.auto_flag,false,false,5);

	GtkWidget *label2 = gtk_label_new ("Thread Num: ");
	gtk_box_pack_start(GTK_BOX(thread_box),label2,false,false,5);

	m_queue->m_attrbox.num = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(m_queue->m_attrbox.num),2);
	gtk_entry_set_text(GTK_ENTRY(m_queue->m_attrbox.num),"10");
	gtk_box_pack_start(GTK_BOX(thread_box),m_queue->m_attrbox.num,false,false,5);
	gtk_box_pack_start(GTK_BOX(box),thread_box,false,false,5);


	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(m_queue->m_attrbox.dialog)->vbox),box);
	gtk_widget_show_all (m_queue->m_attrbox.dialog);

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
	//	 g_signal_connect(G_OBJECT(ShowListButton),"enter_notify_event", G_CALLBACK(stardict_on_enter_notify), NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(m_fastget->tooltips,ShowListButton,_("Go back to home interface"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_ADD,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(on_add_task_callback),m_fastget->m_list->m_queue);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(m_fastget->tooltips,ShowListButton,_("Go back to home interface"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_GOTO_FIRST,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(on_task_up_callback),m_fastget->m_list->m_queue);
	//	 g_signal_connect(G_OBJECT(ShowListButton),"enter_notify_event", G_CALLBACK(stardict_on_enter_notify), NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(m_fastget->tooltips,ShowListButton,_("Go back to home interface"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_GOTO_LAST,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(on_task_down_callback),m_fastget->m_list->m_queue);
	//	 g_signal_connect(G_OBJECT(ShowListButton),"enter_notify_event", G_CALLBACK(stardict_on_enter_notify), NULL);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(m_fastget->tooltips,ShowListButton,_("Go back to home interface"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_CANCEL,GTK_ICON_SIZE_SMALL_TOOLBAR);
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
	gtk_tooltips_set_tip(m_fastget->tooltips,ShowListButton,_("Stop all tasks"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_ABOUT,GTK_ICON_SIZE_SMALL_TOOLBAR);
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
	gtk_tooltips_set_tip(m_fastget->tooltips,ShowListButton,_("About this software and FAQ"),NULL);	 
	gtk_widget_show(ShowListButton);

	// 
	ShowListButton=gtk_button_new();
	image = gtk_image_new_from_stock(GTK_STOCK_QUIT,GTK_ICON_SIZE_SMALL_TOOLBAR);
	gtk_widget_show(image);
	gtk_container_add(GTK_CONTAINER(ShowListButton),image);
	gtk_button_set_relief (GTK_BUTTON (ShowListButton), GTK_RELIEF_NONE);
	GTK_WIDGET_UNSET_FLAGS (ShowListButton, GTK_CAN_FOCUS);
	g_signal_connect(G_OBJECT(ShowListButton),"clicked", G_CALLBACK(m_fastget->on_quit),m_fastget);
#ifdef CONFIG_GPE
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,0);
#else
	gtk_box_pack_start(GTK_BOX(hbox),ShowListButton,false,false,5);
#endif
	gtk_tooltips_set_tip(m_fastget->tooltips,ShowListButton,_("Quit fastget downloader"),NULL);	 
	gtk_widget_show(ShowListButton);

}

gboolean toolbar::on_add_task_callback(GtkWidget *window, GdkEvent *event, taskqueue *m_queue)
{
	if(m_queue != NULL)
	{
        if(m_queue->attrbox(m_fastget->m_list->m_queue))
        {
            return true;
        }
	}
	return false;
}

void toolbar::on_task_up_callback(GtkWidget *window, GdkEvent *event, taskqueue *m_queue)
{
	g_printf("test for task up \n");
}

void toolbar::on_task_down_callback(GtkWidget *window, GdkEvent *event, taskqueue *m_queue)
{
	g_printf("test for task down \n");
}

void toolbar::setting_callback(GtkWidget *widget, toolbar *m_toolbar)
{
	;
}

void toolbar::stop_task_callback(GtkWidget *widget, toolbar *m_toolbar)
{
	;
}

void toolbar::about_callback(GtkWidget *widget, toolbar *m_toolbar)
{
	const gchar *authors[] = {
		"Dragon <chinagnu@gmail.com>",
		NULL
	};
	gchar *documenters[] = {
		"Dragon <chinagnu@gmail.com>",
		NULL
	};
	gchar *translator_credits = _("translator_credits");

	gtk_show_about_dialog(
			GTK_WINDOW(m_fastget->win),
			"name", _("fastget"),
			"version", VERSION,
//			"project site", "http://code.google.com/p/fastget/",
			"website", "http://www.gnumac.cn/project.php?id=2",
			"comments", _("fastget is a cross-platform multi-thread downloader\n\n"),
			"copyright", "Copyright (C) 2009-2011 by Dragon",
			"authors", (const char **)authors,
			"documenters", (const char **)documenters,
			"translator-credits", strcmp(translator_credits,"translator_credits") != 0 ? translator_credits : NULL,
//			"logo", m_fastget->m_fastget.icon.get(),
			NULL);
}

gboolean fastget::create(gchar *title, guint width, guint height )
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

	gtk_paned_set_position(GTK_PANED(hpaned), 120);
	gtk_box_pack_start(GTK_BOX(box),hpaned,TRUE,TRUE,0);
	gtk_widget_show(hpaned);

	// create statusbar
	gchar *copyright = "Copyright (C) 2009-2011 dragon all rights reserved ®  \t\t\t http://www.gnumac.cn/project.php \t\t\t free software, free world! less is more ! dragon means china, not evil  !";
	GtkWidget *status_bar = gtk_statusbar_new ();      
	gtk_box_pack_start (GTK_BOX(box), status_bar, false, false, 5);
	gtk_widget_show (status_bar);

	guint context_id = gtk_statusbar_get_context_id(
			GTK_STATUSBAR (status_bar), copyright);
	gtk_statusbar_push(GTK_STATUSBAR(status_bar),context_id, copyright);

	gtk_container_add(GTK_CONTAINER(win),box);
	gtk_widget_show(win);

	return true;
}

void fastget::init(gchar *title, guint width, guint height)
{
	if(create(title,width,height))
	{
		gdk_threads_enter();
		m_splash->splash_screen("Loading...");
		gtk_main();
		gdk_threads_leave();
	}
}


// splash in the start time
void splash::splash_screen(gchar *info)
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

