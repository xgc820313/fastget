#ifndef  FTP_H_INC
#define  FTP_H_INC

#include	"conn.h"

/*
 * =====================================================================================
 *        Class:  Ftp
 *  Description:  get target data with ftp protocal
 * =====================================================================================
 */
class Ftp:public Conn
{
public:
	/* ====================  LIFECYCLE     ======================================= */
	Ftp (attr_t *attr);                             /* constructor */
	~Ftp(void);

	/* ====================  ACCESSORS     ======================================= */
	gboolean get_resource(void);
	double gettime(void);
	void parse_url(char *url);
	gboolean encode_cmd(char *format, ... );
	int get_result_code();
	int ftp_cwd(char *cwd);
	gboolean login(void);
	gboolean logout(void);
	int get_line_buf( char *buf, int movepos, char *line );
	gboolean set_bin_mode(void);
	gboolean set_text_mode(void);
	gboolean abort(void);
	gboolean set_pasv_mode(void);
	gboolean get_file(void);
	gboolean split_target( long long int pos );
	gboolean get_size(void);
	void dis_conn(void);

	/* ====================  DATA MEMBERS  ======================================= */
	long long int nbytes;
	char request[MAX_STRING];
	char headers[MAX_STRING];
	long long int start_point,end_point;
	char *dip;
	int dport;

	attr_t *t_attr;

protected:

private:
	void ftp_decode( char *s );
	void ftp_encode( char *s );

}; /* -----  end of class Ftp  ----- */

#endif   /* ----- #ifndef FTP_H_INC  ----- */
