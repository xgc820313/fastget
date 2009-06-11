/***************************************************************************************************/
/**
 *
 * @file       xml.cpp
 * @brief      Header file of class XMLFile
 * This file contains a class for XML file decoding
 * @version    0.01
 * @author     Dragon, Fong's National Engineering Co. Ltd
 * @date       2009-05-06
 * @remarks    none
 * @bug        unknown
 * @todo       unknown
 */
/***************************************************************************************************/

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
unsigned char XMLFile::read_xml_file(char *filename)
{
    FILE *fp=NULL;
    char buf[BUFSIZ];
    XML_Parser parser = XML_ParserCreate(NULL);
    int done;
 
    fp = fopen(filename,"r");

    if (fp == NULL) {
		fprintf(stderr,"Couldn't Read the xml file\n");
		return 0;
    }

    XML_SetUserData(parser, this);
    XML_SetElementHandler(parser, startElement, endElement);
    XML_SetCharacterDataHandler(parser, getvalue);

    //printf("Decoding XML file...\n");

    while (!feof(fp)) {
		do {
			size_t len = fread(buf, 1, sizeof(buf), fp);
			done = len < sizeof(buf);
			if (XML_Parse(parser, buf, len, done) == XML_STATUS_ERROR) {
			    fprintf(stderr,"can't read anything in  %s xml file\n",filename);
			    return 0;
			}
		} while (!done);
    }

    fclose(fp);
    XML_ParserFree(parser);

    return 1;
}

