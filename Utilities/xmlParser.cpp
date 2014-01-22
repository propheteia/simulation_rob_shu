#include "xmlParser.h"

Str::Str(std::string s_value):val(s_value)
{
}

bool Str::asBool()
{
	if (val == "False" || val == "0")
		return false;
	else
		return true;
}

int Str::asInt()
{
	int number = std::atoi(val.c_str());
	return number;
}

float Str::asFloat()
{
	float number = std::atof(val.c_str());
	return number;
}

std::string Str::asString()
{
	return val;
}

XMLParser::XMLParser()
{
	//LOG<<"XMLParser::XMLParser()-->A XMLParser is constructed."<<endl;
	readXMLFile("Configs/configure.xml");
	values.clear();
}

bool XMLParser::readXMLFile(std::string name)
{
	fileName=name;
	doc = xmlParseFile(fileName.c_str());
	if (doc == NULL)
	{
		cout<<"xmlParser::readXMLFile-->Document not parsed successfully"<<endl;
		isDocExist=false;
		return false;
	}
	else
	{
		isDocExist=true;
		//LOG<<"xmlParser::readXMLFile-->Reading "<<fileName<<" successfully"<<endl;
			
		rootNode = xmlDocGetRootElement(doc);
		if (rootNode == NULL)
		{
			isEmptyDoc=true;
			cout<<"xmlParser::readXMLFile-->Empty document"<<endl;
			return false;
		}
		else 
		{
			//cout<<"Not Empty document"<<endl;
			isEmptyDoc=false;
			curNode =rootNode;
			lastNode = rootNode;
			fatherNode = NULL;
			//cout<<"isEmptyDoc="<<isEmptyDoc<<endl;
			return true;
		}
	}
}

xmlXPathObjectPtr XMLParser::get_nodeset(const xmlChar *xpath) 
{
	context = xmlXPathNewContext(doc);
	if (context == NULL) 
	{
		cout<<"context is NULL\n";
		return NULL;
	}
	result = xmlXPathEvalExpression(xpath, context);
	xmlXPathFreeContext(context);
	
	if (result == NULL) 
	{
		cout<<"xmlXPathEvalExpression return NULL\n";
		return NULL;
	}
	if (xmlXPathNodeSetIsEmpty(result->nodesetval)) 
	{
		xmlXPathFreeObject(result);
		cout<<"nodeset is empty\n";
		return NULL;
	}
	return result;
}

void XMLParser::setCurNode(const char* Xpath, int index)
{
	const xmlChar* xpath = convertCharToXmlChar(Xpath);
	xmlXPathObjectPtr app_result= get_nodeset(xpath);
	if(app_result) 
	{
		xmlNodeSetPtr nodeset = app_result->nodesetval;	
		curNode = nodeset->nodeTab[index]; 
		xmlXPathFreeObject (app_result);
	}
	else
	{
		cout<<"Your xpath \""<<Xpath<<"\" returns an empty result!"<<endl;
	}
}

int XMLParser::getXMLValueCount(const char* Xpath)
{
	const xmlChar* xpath = convertCharToXmlChar(Xpath);
	xmlXPathObjectPtr app_result= get_nodeset(xpath);
	if(app_result) 
	{
		xmlNodeSetPtr nodeset = app_result->nodesetval;
		return nodeset->nodeNr;
	}
	else
		return 0;
}

Str& XMLParser::getXMLValue(const char* Xpath, int index)
{
	values.clear();
	
	setCurNode(Xpath,index);
	xmlChar *value;
	
	if (curNode!=NULL) 
	{	
		lastNode=curNode;
		curNode = curNode->xmlChildrenNode;
		//cout<<xmlNodeGetContent(curNode)<<endl;
		value = xmlNodeGetContent(curNode);
		if (value != NULL) 
		{
			char* cvalue=reinterpret_cast<char*>(value);
			values.push_back(Str(std::string(cvalue)));
			xmlFree(value);
		}
	}
	
	if (!values.empty())
		return values[0];
	else
		cout<<"XMLParser::getXMLVal-->returns an invalid value. Xpath = "<<Xpath<<" Exiting..."<<endl;
}


Str& XMLParser::getXMLPropVal(const char* Xpath, const char* prop, int index)
{
	values.clear();
	setCurNode(Xpath,index);
	xmlChar *value;

	if (curNode!=NULL) 
	{	
		lastNode=curNode;
		value = xmlGetProp(curNode,BAD_CAST prop);
		if (value != NULL) 
		{
			char* cvalue=reinterpret_cast<char*>(value);
			values.push_back(Str(std::string(cvalue)));
			xmlFree(value);
		}
	}
	
	if (!values.empty())
		return values[0];
	else
		cout<<"XMLParser::getXMLPropVal-->returns an invalid value. Xpath = "<<Xpath<<" prop = "<<prop<<" Exiting..."<<endl;
}

/*bool XMLParser::asBool()
{
	return values[0].asBool();
}

int XMLParser::asInt()
{
	return values[0].asInt();
}

float XMLParser::asFloat()
{
	return values[0].asFloat();
}

std::string XMLParser::asString()
{
	return values[0].asString();
}*/


