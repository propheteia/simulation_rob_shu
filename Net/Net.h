/***************************************************************************
 *   
 *   Copyright (C) 2008 by Neil Zhu                                      *
 *   zhuming535984@gmail.com                                                               *
 *   Utility Test                                                          *
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
#ifndef NET_H
#define NET_H

#include <cstring>
#include <iostream>
#include <map>
#include <string>

#include "tcpsocket.hpp"
#include "exception.hpp"
#include <boost/scoped_ptr.hpp>

#include "StaticModule.h"
#include "Logger.h"
#include "xmlParser.h"

//#define NET_M StaticModule<NetManager>::getInstance()
#define NET StaticModule<NetManager>::getInstance().selectNetwork("Main")
#define DEBUG_NET StaticModule<NetManager>::getInstance().selectNetwork("GuiDebugger")


using namespace rcss::net;
using namespace std;
using namespace boost;

class Net
{
public:
	Net(string host, int port);
	~Net();

	bool Init();
	void Done();
	bool GetMessage(string& msg);

	void PutMessage(const string& msg);
	bool SelectInput();
	void SetServer(string host) { mHost = host; }
	void SetPort(int port) { mPort = port; }
	
	bool  isValid(){return mIsValid;}

private:
	TCPSocket mSocket;
	string mHost;
	int mPort;
	bool  mIsValid;
};

class NetManager
{
private:
	map<string, Net*>nets;

public:
	NetManager(){nets.clear();}
//void Init();
	void addNetwork(string name);
	Net& selectNetwork(string name);
};

#endif
