/***************************************************************************************************/
/**
 *
 * @file       xml.h
 * @brief      Header file of class CXMLFile
 * This file contains a class for XML file decoding
 * @version    0.01
 * @author     Dragon, Fong's National Engineering Co. Ltd
 * @date       2009-05-06
 * @remarks    none
 * @bug        unknown
 * @todo       unknown
 */
/***************************************************************************************************/

#ifndef _XMLFILE_H_
#define _XMLFILE_H_

#include <stdio.h>
#include <expat.h>

/**
 *
 *  @brief Class XMLFile
 *
 *  This file contains a class for XML file decoding
 *
 */
class XMLFile 
{
private:

protected:

    friend void XMLCALL startElement(void *userData, const char *name, const char **atts);
    friend void XMLCALL endElement(void *userData, const char *name);
    friend void XMLCALL getvalue(void *userData, const XML_Char *s, int len);
    
public:
    virtual void StartXMLElement(const char *name, const char **atts) {};
    virtual void EndXMLElement(const char *name) {};
    virtual void GetXMLValue(const XML_Char *s, int len) {};

    unsigned char read_xml_file(char *filename);
};

#endif

