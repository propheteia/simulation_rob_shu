#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <vector>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "Logger.h"

#ifndef __XMLPARSER_H
#define __XMLPARSER_H

#define CONF StaticModule<XMLParser>::getInstance()

using namespace std;

class Str
{
protected:
	std::string val;
	
public:
	Str(std::string s_value);
	
	bool asBool();
	int asInt();
	float asFloat();
	std::string asString();
};


class XMLParser
{
protected:
	xmlDocPtr doc;
	xmlNodePtr rootNode;
	xmlNodePtr curNode;
	xmlNodePtr lastNode;
	xmlNodePtr fatherNode;
	xmlNodePtr content;
	
	xmlXPathContextPtr context;
	xmlXPathObjectPtr result;
	xmlChar *xpath;
	
	std::string fileName;
	bool isEmptyDoc;
	bool isDocExist;
	
	vector<Str>values;
public:
	XMLParser();
	
	bool readXMLFile(std::string name);
	
	xmlDocPtr getXMLDocPtr()
	{	
		return doc;
	}
	
	const xmlChar* convertCharToXmlChar(const char* path)
	{
		return reinterpret_cast<const xmlChar*>(path);
	}
	
	std::string getFileName()
	{
		return fileName;
	}
	
	xmlXPathObjectPtr get_nodeset(const xmlChar *xpath);

	//int getXMLValueCount(const xmlChar* xpath);
	int getXMLValueCount(const char* Xpath);
	//std::string getXMLValue(const char* Xpath, int index=0);
	//std::string getXMLPropVal(const char* Xpath, const char* prop, int index=0);
	
	Str& getXMLValue(const char* Xpath, int index=0);
	Str& getXMLPropVal(const char* Xpath, const char* prop, int index=0);
	
	void setCurNode(const char* Xpath, int index=0);
	void setCurNode(xmlNodePtr node){curNode=node;}
	void setLastNode(xmlNodePtr node){lastNode=node;}
	
	xmlNodePtr getFatherNode(){return fatherNode;}
	xmlNodePtr getRootNode(){return rootNode;}
	xmlNodePtr getCurNode(){return curNode;}
	
	/*bool asBool();
	int asInt();
	float asFloat();
	std::string asString();*/
};

#endif //__XMLPARSER_H

