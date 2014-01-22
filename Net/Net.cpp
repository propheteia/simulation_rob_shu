/***************************************************************************
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

#include "Net.h"
#include <errno.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

Net::Net(string host, int port):mHost(host),mPort(port),mIsValid(false)
{
}

/*Net::Net()
{
    mHost = "127.0.0.1";
    mPort = 3100;
    
    mHost = CONF.getXMLValue("/Strive/server/host").asString();
    mPort = CONF.getXMLValue("/Strive/server/port").asInt();
}*/

Net::~Net()
{
}

bool Net::Init()
{
	cout << "connecting to TCP " << mHost << ":" << mPort << "\n";
	//cout << "connecting to UDP " << gHost << ":" << gPort << "\n";

	try
	{
		Addr local ( INADDR_ANY,INADDR_ANY );
		mSocket.bind ( local );
	}

	catch ( BindErr error )
	{
		cerr << "failed to bind socket with '"
		<< error.what() << "'" << endl;
		
		cout << "failed to bind socket with '" << error.what() << "'" << endl;

		mSocket.close();
		return false;
	}

	try
	{
		Addr server ( mPort,mHost );
		mSocket.connect ( server );
	}

	catch ( ConnectErr error )
	{
		cerr << "connection failed with: '"
		<< error.what() << "'" << endl;
		
		cout << "connection failed with: '" << error.what() << "'" << endl;
		
		mSocket.close();
		return false;
	}

	mIsValid = true;
	return true;
}

void Net::Done()
{
	mSocket.close();
	mIsValid = false;
	cout << "closed connection to " << mHost << ":" << mPort << "\n";
}

bool Net::SelectInput()
{
	fd_set readfds;
	struct timeval tv = {60,0};
	FD_ZERO ( &readfds );
	FD_SET ( mSocket.getFD(),&readfds );

	while ( 1 )
	{
		switch ( select ( mSocket.getFD() +1,&readfds, 0, 0, &tv ) )
		{
			case 1:
				return 1;
			case 0:
				cerr << "(SelectInput) select failed " << strerror ( errno ) << endl;
				abort();
				return 0;
			default:
				if ( errno == EINTR )
					continue;
				cerr << "(SelectInput) select failed " << strerror ( errno ) << endl;
				abort();
				return 0;
		}
	}
}

void Net::PutMessage ( const string& msg )
{
	if ( msg.empty() )
	{
		return;
	}

	// prefix the message with it's payload length
	unsigned int len = htonl ( msg.size() );
	string prefix ( ( const char* ) &len,sizeof ( unsigned int ) );
	string str = prefix + msg;
	mSocket.send ( str.data(),str.size() );
}

bool Net::GetMessage ( string& msg )
{
	static char buffer[16 * 1024];

	unsigned int bytesRead = 0;
	while ( bytesRead < sizeof ( unsigned int ) )
	{
		SelectInput();
		int readResult = mSocket.recv ( buffer + bytesRead, sizeof ( unsigned int ) - bytesRead );
		if ( readResult < 0 )
			continue;
		bytesRead += readResult;
	}

	//cerr << "buffer = |" << string(buffer+1) << "|\n";
	//cerr << "bytesRead = |" << bytesRead << "|\n";
	//cerr << "Size of buffer = |" << sizeof(buffer) << "|\n";
	//cerr << "buffer = |" << buffer << "|\n";
	//cerr << "buffer[5] = |" << buffer[5] << "|\n";
	//printf ("xxx-%s\n", buffer+5);

	// msg is prefixed with it's total length
	unsigned int msgLen = ntohl ( * ( unsigned int* ) buffer );
	// cerr << "GM 6 / " << msgLen << " (bytesRead " << bytesRead << ")\n";
	if ( sizeof ( unsigned int ) + msgLen > sizeof ( buffer ) )
	{
		cerr << "message too long; aborting" << endl;
		abort();
	}

	// read remaining message segments
	unsigned int msgRead = bytesRead - sizeof ( unsigned int );

	//cerr << "msgRead = |" << msgRead << "|\n";

	char *offset = buffer + bytesRead;

	while ( msgRead < msgLen )
	{
		if ( ! SelectInput() )
		{
			return false;
		}

		int readLen = sizeof ( buffer ) - msgRead;
		if ( readLen > msgLen - msgRead )
			readLen = msgLen - msgRead;

		int readResult = mSocket.recv ( offset, readLen );
		if ( readResult < 0 )
			continue;
		msgRead += readResult;
		offset += readResult;
		//cerr << "msgRead = |" << msgRead << "|\n";
	}

	// zero terminate received data
	( *offset ) = 0;

	msg = string ( buffer+sizeof ( unsigned int ) );

	// DEBUG
	//cout << msg << endl;

	return true;
}

void NetManager::addNetwork(string name)
{
	string host;
	int port;
	
	if (name == "Main")
	{
		host = "127.0.0.1";
		port = 3100;
		
		host = CONF.getXMLValue("/Strive/server/host").asString();
		port = CONF.getXMLValue("/Strive/server/port").asInt();
	}
	else if (name == "GuiDebugger")
	{
		host = "127.0.0.1";
		port = 6300;
	}
	else
	{
		cout<<"NetManager::addNetwork-->network name \""<<name<<"\" is invalid name."<<endl;
	}
	
	Net* net = new Net(host, port);
	nets.insert(make_pair(name, net));
}

Net& NetManager::selectNetwork(string name)
{
	map<string, Net*>::iterator map_it;
	map_it = nets.find(name);
	if (map_it == nets.end())
	{
		cout<<"NetManager::selectNetwork-->network \""<<name<<"\" not found."<<endl;
		assert(0);
	}
	else
		return *(map_it->second);
}
