/*
 * =====================================================================================
 *
 *       Filename:  config.h
 *
 *    Description:  xml configure file operation
 *
 *        Version:  1.0
 *        Created:  Friday, May 15, 2009 03:55:36 HKT
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dragon, chinagnu@gmail.com
 *        Company:  Fong's National Engineering Co. Ltd
 *
 * =====================================================================================
 */

#ifndef  CONFIG_H_INC
#define  CONFIG_H_INC

#include	"task.h"
#include	"xml.h"

/*
 * =====================================================================================
 *        Class:  Conf
 *  Description:  xml configure file operation
 * =====================================================================================
 */
class Conf:public XMLFile
{
public:
	/* ====================  LIFECYCLE     ======================================= */
	Conf (void);                             /* constructor */

	/* ====================  ACCESSORS     ======================================= */
	void read_conf_file(char *filename)
	{
		read_xml_file(filename);
	}

	/* ====================  OPERATORS     ======================================= */
	gboolean import_task_list(void);

	/* ====================  DATA MEMBERS  ======================================= */


protected:

private:

}; /* -----  end of class Conf  ----- */

#endif   /* ----- #ifndef CONFIG_H_INC  ----- */

