/*
 * =====================================================================================
 *
 *       Filename:  taskctrl.h
 *
 *    Description:  task list operation and task manager
 *
 *        Version:  1.0
 *        Created:  05/11/2009 03:48:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon  
 *        Company:  www.gnumac.cn
 *
 * =====================================================================================
 */

#ifndef  TASKCTRL_H_INC
#define  TASKCTRL_H_INC

#include	"task.h"
#include	"conf.h"

struct TaskNode
{
    Task *data;
	TaskNode *next;
};

/*
 * =====================================================================================
 *        Class:  TaskCtrl
 *  Description:  task list operation, configure of task operation 
 * =====================================================================================
 */
class TaskCtrl
{
public:
	/* ====================  LIFECYCLE     ======================================= */
	TaskCtrl (void);                           /* constructor */
	~TaskCtrl(void);

	/* ====================  ACCESSORS     ======================================= */
	bool task_list_add(attr_t attr);
	bool task_list_insert(int index,attr_t attr);
	bool task_list_remove(int id);
	void task_list_erase(void);
	bool is_empty(void);
	Task *get_task(int index);
	int get_task_num(void){return length;}
	TaskNode *get_task_list(){return head;}

	void output_list(void);
	bool task_status_change(int id,status_t status)
	{
		Task *tmp = get_task(id);
		if(tmp != NULL)
		{
			tmp->set_task_status(status);
			return true;
		}
		return false;
	};

	/* ====================  DATA MEMBERS  ======================================= */
protected:
	Conf *m_xml;
private:
	TaskNode *head;
	int length;

}; /* -----  end of class TaskCtrl  ----- */


#endif   /* ----- #ifndef TASKCTRL_H_INC  ----- */
