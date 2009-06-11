/*
 * =====================================================================================
 *
 *       Filename:  taskctrl.cpp
 *
 *    Description:  codes of task manager and task list operation
 *
 *        Version:  1.0
 *        Created:  05/11/2009 03:52:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon
 *        Company:  www.gnumac.cn
 *
 * =====================================================================================
 */

#include	"taskctrl.h"

TaskCtrl::TaskCtrl(void)
{
	head=NULL;
	length=0;
	m_xml = new Conf();
}

TaskCtrl::~TaskCtrl(void)
{
	delete m_xml;
	task_list_erase();
}

bool TaskCtrl::task_list_add(attr_t attr)
{
    TaskNode*p,*s; 
    s=(TaskNode*)new(TaskNode); 
    s->data=new Task(attr);

    if(s->data == NULL)
    {
        delete s;
        return false;
    }

    p=head;
    if(head==NULL) 
    {
        head=s;
        s->next=NULL;
    }
    else
    {
        while(1)
        {
            if(p->next != NULL)
                p=p->next;
            else 
				break;
        }
        p->next=s;
        s->next=NULL;
    }
	length++;
	return true;
}

bool TaskCtrl::task_list_remove(int id)
{
	if(length == 0)
	{
		cout<<"task list is empty"<<endl;
		return false;
	}
	else if(id<1 || id> length)
	{
		cout<<"Not find postion of this index in Task List, can't delete"<<endl;
		return false;
	}

	TaskNode *current = head->next;
	for(int i=1;(i<id) && current; ++i)
	{
		current = current->next;
	}

	TaskNode *p=current;
	current = current->next;

	delete p->data;
	delete p;
	p=NULL;
	length--;

	return true;
}

bool TaskCtrl::is_empty()
{
    if(length>0)
        return false;
    else
        return true;
}

void TaskCtrl::output_list()
{
    TaskNode*current=head;
    while(current!=NULL)
    {
        cout<<current->data->get_task_id()<<" ";
        current=current->next;
    }
    cout<<endl;
}

bool TaskCtrl::task_list_insert(int index,attr_t attr)
{
	if(index>=0 && index<=length)
	{
		TaskNode *newnode =(TaskNode *)new(TaskNode);
		newnode->data = new Task(attr);

		TaskNode *current=head;
		for(int i=0;i<index; ++i)
		{
			current = current->next;
		}

		newnode->next = current->next;
		current->next = newnode;

		length++;

		return true;
	}
	else
	{
		cout<<"Can't find the postion of this index, Can't Insert!"<<endl;
	}

	return false;
}

Task* TaskCtrl::get_task(int index)
{
	if(length == 0)
	{
		cout<<"Task List is empty!"<<endl;
		return NULL;
	}
	else if(index<0 || index>length)
	{
		cout<<"Not find the postion of this index in task list"<<endl;
		return NULL;
	}

	TaskNode *current=head;
	for(int i=0;(i<index) && (current != NULL); ++i)
	{
		current = current->next;
	}

	if(current != NULL)
	{
        return current->data;
	}

	return NULL;
}

void TaskCtrl::task_list_erase(void)
{
	TaskNode *current=head;
	while(current!= NULL)
	{
		head=head->next;
		delete current->data;
		delete current;
		current = head;
	}
	length=0;
}
