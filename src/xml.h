/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/**
 * @file       xml.h
 * 
 * @brief      Header file of class CXMLFile
 * @version    0.1
 * @author     Dragon, Fong's National Engineering Co. Ltd
 * @date       Thu Jul  2 10:00:48 2009
 *
 * @bug        unknown
 * @todo       unknown
 */

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

