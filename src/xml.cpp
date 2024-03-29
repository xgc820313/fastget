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
 * @brief      This file contains a class for XML file decoding
 * @version    0.1
 * @author     Dragon, Fong's National Engineering Co. Ltd
 * @date       Thu Jul  2 10:00:48 2009
 *
 * @bug        unknown
 * @todo       unknown
 */

#include "xml.h"

/**
 *  handle element start case
 */
void XMLCALL startElement(void *userData, const char *name, const char **atts)
{
    XMLFile *pObj = (XMLFile*)(userData);
    pObj->StartXMLElement(name, atts);
}

/**
 *  handle element end case
 */
void XMLCALL endElement(void *userData, const char *name)
{
    XMLFile *pObj = (XMLFile*)(userData);

    pObj->EndXMLElement(name);
}

/**
 *  handle get value case
 */
void XMLCALL getvalue(void *userData, const XML_Char *s, int len)
{
    XMLFile *pObj = (XMLFile*)(userData);

    pObj->GetXMLValue(s, len);
}

/**
 *  Read and decode a XML file
 *
 *  @param[in] filename <b>XML file name</b> 
 *
 *  @return return 1 if success otherwise return 0
 */
unsigned char XMLFile::read_xml_file(const char *filename)
{
    FILE *fp=NULL;
    char buf[BUFSIZ];
    XML_Parser parser = XML_ParserCreate(NULL);
    int done;
 
    fp = fopen(filename,"r");

    if (fp == NULL) {
		fprintf(stderr,"Couldn't Read the xml file\n");
		return false;
    }

    XML_SetUserData(parser, this);
    XML_SetElementHandler(parser, startElement, endElement);
    XML_SetCharacterDataHandler(parser, getvalue);

    while (!feof(fp)) 
	{
		do {
			size_t len = fread(buf, 1, sizeof(buf), fp);
			done = len < sizeof(buf);
			if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
			    fprintf(stderr,"can't read anything in  %s xml file\n",filename);
			    return false;
			}
		} while (!done);
    }

    fclose(fp);
    XML_ParserFree(parser);

    return true;
}

