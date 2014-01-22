/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei , Qu Junjun  *
 *   zhuming535984@gmail.com,zhengyonglei@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "XabslEngineExecutor.h"
#include <sys/timeb.h>
#include <fstream>
#include "Logger.h"

// returns the current system time in milliseconds 
static unsigned long getCurrentSystemTime() 
{
  timeb sysTime;
  ftime(&sysTime);
  return (sysTime.time * 1000 + sysTime.millitm);
}

XabslEngineExecutor::XabslEngineExecutor (SolutionRequest::xabslEngineID id,
                                                SolutionRequest::ModuleID module,
                                                const int& frameNumber) 
						: pEngine(0), 
						errorHandler(id), 
						id(id), 
						module(module)
{
}

XabslEngineExecutor::~XabslEngineExecutor()
{
  if (pEngine!=0) delete pEngine;
}

void XabslEngineExecutor::init(InputSource& input)
{
  	if (pEngine != 0) delete pEngine;
  
  	errorHandler.errorsOccurred = false; // reset the error handler
  
  	unsigned long startTime = getCurrentSystemTime();
  
  	pEngine = new Engine(errorHandler,&getCurrentSystemTime);
  
  	registerSymbolsAndBasicBehaviors();
  
  	pEngine->createOptionGraph(input);

  	if (!errorHandler.errorsOccurred)
  	{
    		errorHandler.message("created a new Engine (%li ms)", 
      		getCurrentSystemTime() - startTime );
  	}
}

void XabslEngineExecutor::executeEngine()
{
  	if (errorHandler.errorsOccurred)
  	{
    		executeIfEngineCouldNotBeCreated();
    		return;
  	}
  
  	/**switch (debugMode)
  	{
  		case executeRootOption:
    			pEngine->setRootOption();
    			pEngine->execute();
    		break;
  		case executeOption:
    		// execute the complete option graph beginning from the root option
    			pEngine->execute();
    		break;
  		case executeBasicBehavior:
    			// don't execute the option graph but only the basic behavior
    			pEngine->executeSelectedBasicBehavior();
    		break;
  	}*/

}


void XabslEngineExecutor::setSelectedAgent(const char* name)
{
  if (pEngine != 0) 
    pEngine->setSelectedAgent(name);
}

MyXabslErrorHandler::MyXabslErrorHandler(SolutionRequest::xabslEngineID id)
: id (id)
{
}

void MyXabslErrorHandler::printError(const char* text)
{
  	bLOG <<  *text << "(" << SolutionRequest::getXabslEngineIDName(id)
   		 << " Xabsl2Engine) error: " << *text << endl;
}

void MyXabslErrorHandler::printMessage(const char* text)
{
	bLOG << *text << "(" << SolutionRequest::getXabslEngineIDName(id)
   		 << " Xabsl2Engine): " << *text << endl;
}

MyXabslFileInputSource::MyXabslFileInputSource(const char* fileName)
: NamedItem(fileName), file(0)
{	 strcpy(filename,fileName);
}

MyXabslFileInputSource::~MyXabslFileInputSource()
{
  if (file!=0) delete file;
}

bool MyXabslFileInputSource::open()
{
  	if (file==0) file=new std::ifstream(filename);
  	return(file!=0);
}


void MyXabslFileInputSource::close()
{
  	if (file!=0) delete file;
  	file = 0;
}

double MyXabslFileInputSource::readValue()
{
 	char buf[20]; readFromFile(buf); return atof(buf);
}

bool MyXabslFileInputSource::readString(char* destination, int maxLength)
{
	readFromFile(destination); return true;
}

void MyXabslFileInputSource::readFromFile(char* value)
{
    	while(!file->eof() && isWhitespace()) 
    	{
      		if (theChar == '/') while(!file->eof() && theChar != '\n') file->read(&theChar,1);
      		file->read(&theChar,1);
    	}

    	while(!file->eof() && !isWhitespace())
    	{ *value++ = theChar; if(!file->eof()) file->read(&theChar,1); }
    		*value = 0;
}

bool MyXabslFileInputSource::isWhitespace()
{ return theChar == ' ' || theChar == '/' || theChar == '\n' || theChar == '\r' || theChar == '\t'; }