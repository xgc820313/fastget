#ifndef  HTTP_H_INC
#define  HTTP_H_INC

#include	"conn.h"

#define MAX_QUERY	2048		

/*
 * =====================================================================================
 *        Class:  Http
 *  Description:  send request , get target data with http protocal
 * =====================================================================================
 */
class Http:public Conn
{
public:
	/* ====================  LIFECYCLE     ======================================= */
	Http (attr_t *attr);                             /* constructor */
	~Http (void);

	/* ====================  ACCESSORS     ======================================= */
	void encode_request(void );
	gboolean get_resource(void);
	gboolean setup_download_thread(void);
	double gettime(void);
	void dis_conn(void);
	void parse_url(char *url);
	void get_redirect_url(void);

	/* ====================  DATA MEMBERS  ======================================= */
	long long int nbytes;
	char request[MAX_QUERY];
	char headers[MAX_QUERY];
	long long int start_point,end_point;

	attr_t *t_attr;

protected:

private:
	void http_decode( char *s );
	void http_encode( char *s );
	void add_header(char *format, ... );

	long long int get_target_size( void );
	char* http_header( char *header );

}; /* -----  end of class Http  ----- */

#endif   /* ----- #ifndef HTTP_H_INC  ----- */
