
#ifndef  CONN_H_INC
#define  CONN_H_INC

#include	"dragon.h"

/** current task status  */
typedef enum
{
	START,
	WAIT,
	PAUSE,
	ERROR,
	CANCEL,
	FINISH
}status_t;	

typedef enum
{
	HTTP,
	FTP,
	P2P
}protocal_t;

typedef struct {
	int id;
	status_t STATUS;
	char url[MAX_STRING];
	char local_file_name[MAX_STRING];
	long long int size;
	char file_size[20];
	char progress[20];
	char speed[20];
	char time_right[20];
	int retry;

	char hostname[MAX_FILE_NAME];			// target host name
	char dir[MAX_STRING];
	char target_file_name[MAX_STRING];	
	int  port;

	char user[MAX_FILE_NAME];
	char passwd[MAX_FILE_NAME];

	int thread_no;
	bool auto_flag;
	double start_time;

}attr_t;

/*
 * =====================================================================================
 *        Class:  Conn
 *  Description:  connect operation
 * =====================================================================================
 */

class Conn
{
public:
	/* ====================  LIFECYCLE     ======================================= */
	Conn (void)
	{
		sockfd=0;
		avaliable=false;
	}                                           /* constructor */
	virtual ~Conn (void){}
	void create_conn(char *hostname, int port);

	/* ====================  ACCESSORS     ======================================= */

	/* ====================  OPERATORS     ======================================= */
	virtual void dis_conn(void){};

	/* ====================  DATA MEMBERS  ======================================= */
	int sockfd;
	gboolean avaliable;

protected:

private:

}; /* -----  end of class Conn  ----- */

#define AUTHOR			"dragon"
#define OFFICE_SITE		"www.gnumac.cn"
#define USER_AGENT		"fastget 1.0 (" AUTHOR ", " OFFICE_SITE ")"

#endif   /* ----- #ifndef CONN_H_INC  ----- */
