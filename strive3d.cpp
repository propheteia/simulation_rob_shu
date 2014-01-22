/***************************************************************************
 *   Copyright (C) 2008 by Zhu_Ming,Zheng Yonglei   *
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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>

#include <fcntl.h>

#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <errno.h>
#include <signal.h>

#include <stdlib.h>

#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif

#include <cstdlib>

#include "Net.h"
#include "Parser.h"
#include "StaticModule.h"
#include "types.h"
#include "nao.h"

#include "worldmodel.h"
#include "BaseAgent.h"
#include "xmlParser.h"
#include "AdvancedAgent.h"
#include "MessageStorage.h"


using namespace std;
using namespace boost;

// bool to indicate whether to continue the agent mainloop
static bool gLoop = true;
static bool debugMode = false;

// SIGINT handler prototype
extern "C" void handler ( int sig )
{
	if ( sig == SIGINT )
		gLoop = false;
}

void PrintGreeting()
{
    //cout<<"Strive3d .\n"
       // <<"Version 2.01\n";
}

void PrintHelp()
{
	cout << "\nusage: Strive3D [options]" << endl;
	cout << "\noptions:" << endl;
	cout << " --help      prints this message." << endl;
	cout << " --debug   start with debug mode." << endl;
	cout << " --host=IP   IP of the server." << endl;
	cout << " -t  the name of the team. " << endl;
	cout << "\n";
}

void initStaticModules( int argc, char* argv[])
{
	std::string s_num;
	//XMLParser must be the very first to initialized.
	StaticModule<XMLParser>::init();
	//Initialize connections
	StaticModule<NetManager>::init();
	StaticModule<NetManager>::getInstance().addNetwork("Main");
	StaticModule<NetManager>::getInstance().addNetwork("GuiDebugger");
	//WorldModel Initialize
	StaticModule<WorldModel>::init(); 
	for ( int i = 0; i < argc; i++ )
	{
		if ( strcmp ( argv[i], "--help" ) == 0 )
		{
			PrintHelp();
			exit ( 0 );
		}
		else if ( strcmp ( argv[i], "--debug" ) == 0 )
		{
			debugMode = true;
		}
		else if ( strncmp ( argv[i], "--host", 6 ) == 0 )
		{
			string tmp=argv[i];
	
			if ( tmp.length() <= 7 ) // minimal sanity check
			{
				PrintHelp();
				exit ( 0 );
			}
			NET.SetServer( tmp.substr ( 7 ));
		}
		else if ( strncmp ( argv[i], "-u", 2 ) == 0 )
		{
			string tmp=argv[i];
	
			if ( tmp.length() <= 3 ) // minimal sanity check
			{
				PrintHelp();
				exit ( 0 );
			}
			s_num = tmp.substr ( 3 );
			int num = std::atoi(s_num.c_str());
			WM.getSelf().setUnum(num);
			//cout<<"uninum = "<<num<<endl;
		}
		else if ( strncmp ( argv[i], "-t", 2 ) == 0 )
		{
			string tmp=argv[i];
	
			if ( tmp.length() <= 3 ) // minimal sanity check
			{
				PrintHelp();
				exit ( 0 );
			}
			s_num = tmp.substr ( 3 );
			WM.getFieldInfo().SetOurTeamName(s_num);
			cout<<"uninum = "<<s_num<<endl;
		}
	}
	
	StaticModule<Logger>::init();
	
	if (CONF.getXMLValue("/Strive/Logger/MainLog").asBool())
		StaticModule<Logger>::getInstance().add("MainLog", new LogFile("./Logs/MainLog" + s_num + ".log"));
	else
		StaticModule<Logger>::getInstance().add("MainLog", new LogNull());
	
	if (CONF.getXMLValue("/Strive/Logger/ParserLog").asBool())
		StaticModule<Logger>::getInstance().add("ParserLog", new LogFile("./Logs/ParserLog" + s_num + ".log"));
	else
		StaticModule<Logger>::getInstance().add("ParserLog", new LogNull());
	
	if (CONF.getXMLValue("/Strive/Logger/ActionLog").asBool())
		StaticModule<Logger>::getInstance().add("ActionLog", new LogFile("./Logs/ActionLog" + s_num + ".log"));
	else
		StaticModule<Logger>::getInstance().add("ActionLog", new LogNull());
		
	if (CONF.getXMLValue("/Strive/Logger/BehaviorControlLog").asBool())
		StaticModule<Logger>::getInstance().add("BehaviorControlLog", new LogFile("./Logs/BehaviorControlLog" + s_num + ".log"));
	else
		StaticModule<Logger>::getInstance().add("BehaviorControlLog", new LogNull());

	if (CONF.getXMLValue("/Strive/Logger/TrainerLog").asBool())
		StaticModule<Logger>::getInstance().add("TrainerLog", new LogFile("./Logs/TrainerLog" + s_num + ".log"));
	else
		StaticModule<Logger>::getInstance().add("TrainerLog", new LogNull());

	if (CONF.getXMLValue("/Strive/Logger/DebugLog").asBool())
		StaticModule<Logger>::getInstance().add("DebugLog", new LogFile("./Logs/Debug" + s_num + ".log"));
	else
		StaticModule<Logger>::getInstance().add("DebugLog", new LogNull());

	//! This Log is For Data Recording (Without Cycle, Time Infomation)
	if (CONF.getXMLValue("/Strive/Logger/OutputLog").asBool())
	{
		StaticModule<Logger>::getInstance().add("OutputLog", new LogFile("./Logs/OutputLog" + s_num + ".log", false));
	}
	else
		StaticModule<Logger>::getInstance().add("OutputLog", new LogNull());

	
	//LOG is defined as StaticModule<Logger>::getInstance()["MainLog"]
	LOG.newCycle(0);
	pLOG.newCycle(0);
	
	//Parser
	StaticModule<Parser>::init(); //need by pathplanning,worldmodel don't need

	//RobotModel
	StaticModule<Nao>::init(); 	

	//Initialize the AdvancedAgent plan system.
	StaticModule<AdvancedAgent>::init();
	StaticModule<PathPlanning>::init();
	StaticModule<MessageStorage>::init();
	

	LOG<<"initStaticModules-->All static modules are initialized."<<endl;
	LOG<<STARS<<endl;
}

int main ( int argc, char* argv[] )
{	
	PrintGreeting();
	initStaticModules(argc,argv);

    char s[]="0:1\n"; 

	//mkfifo("/tmp/trainer_pipe",0666);
    //int fd = open( "/home/wenbin/test-dir/mkfifo/pp", O_WRONLY );
    //write(fd, s, sizeof(s) );

    // registering the handler, catching SIGINT signals
	signal ( SIGINT, handler );
	BaseAgent *Agent;
	Agent = new BaseAgent();
	if ( ! NET.Init() )
	{
	    cout<<"int main-->Main network not initialized successfully."<<endl;
	    return 1;
	}
	while (gLoop)	
	    Agent->run();
	NET.Done();
	return 0;
}
