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
	double gettime(void);
	void parse_url(char *url);


	/* ====================  DATA MEMBERS  ======================================= */
	attr_t *t_attr;

protected:

private:
	void dis_conn(void);
	void ftp_decode( char *s );
	void ftp_encode( char *s );


}; /* -----  end of class Ftp  ----- */

#endif   /* ----- #ifndef FTP_H_INC  ----- */

