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
		gboolean conn_flag;

		/* ====================  DATA MEMBERS  ======================================= */
	protected:

	private:
		void dis_conn(void);
		void parse_url(char *url);

		attr_t *t_attr;

}; /* -----  end of class Ftp  ----- */

#endif   /* ----- #ifndef FTP_H_INC  ----- */

