/*
 * =====================================================================================
 *
 *       Filename:  task.h
 *
 *    Description:  task operation
 *
 *        Version:  1.0
 *        Created:  Friday, May 08, 2009 12:18:15 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon, chinagnu@gmail.com
 *        Company:  Fong's National Engineering Co. Ltd
 *
 * =====================================================================================
 */


#ifndef  TASK_H_INC
#define  TASK_H_INC

#include	"http.h"
#include	"ftp.h"

typedef struct {

	long long int start_address;
	long long int end_address;
	long long int limited;
	long long int total;
	attr_t m_attr;

}down_attr_t;

/*
 * =====================================================================================
 *        Class:  Task
 *  Description:  download task operation
 * =====================================================================================
 */
class Task
{
	public:
		/* ====================  LIFECYCLE     ======================================= */
		Task (attr_t m_attr);                             /* constructor */
		~Task (void);

		/* ====================  ACCESSORS     ======================================= */
		void set_task_id(int id){t_attr.id=id;}
		void set_task_thread_no(int num){t_attr.thread_no = num; }
		void set_url(char* url)
		{
//			t_attr.url = url;
			if(strlen(url)>MAX_STRING)
			{
				strncpy(t_attr.url,url,MAX_STRING);
				t_attr.url[MAX_STRING]=0;
			}
			else
			{
				sprintf(t_attr.url,"%s",url);
				t_attr.url[strlen(url)]=0;
			}
		}
		void set_task_status(status_t status){t_attr.STATUS=status;}
		void set_task_auto_flag(bool flag){t_attr.auto_flag=flag;}

		int get_task_id(void){return t_attr.id;}
		status_t get_task_status(void){return t_attr.STATUS;}
		char* get_task_url(void){return t_attr.url;}
		char* get_task_file(void){return t_attr.local_file_name;}
		long long int get_file_size(void){return t_attr.size;}
		bool get_task_auto_flag(void){return t_attr.auto_flag;}
		int get_retry_times(void){return t_attr.retry;}
		char* get_task_speed(void){return t_attr.speed;}
		char* get_task_progress(void){return t_attr.progress;}
		char* get_task_time_right(void){return t_attr.time_right;}

		/* ====================  OPERATORS     ======================================= */

		bool action_task(void);
        static void setup_http_thread(void *m_handle);
        static void setup_ftp_thread(void *m_handle);
        static void setup_p2p_thread(void *m_handle);


		/* ====================  DATA MEMBERS  ======================================= */
		Http *p_http;
		Ftp *p_ftp;

	protected:

	private:
		attr_t t_attr;
		protocal_t protocal_flag;
        long long int file_size;
		int thread_num;


}; /* -----  end of class Task  ----- */

#endif   /* ----- #ifndef TASK_H_INC  ----- */

