#include "trainer_client.h"
#include <assert.h>
#include <sstream>
#include <unistd.h>
#include "Logger.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

trainer_client::trainer_client()
{
	fd=0;
}

bool trainer_client::get_action(action *act)
{ 
	if(fd==0)
		fd=open("/tmp/trainer_pipe", O_RDWR | O_NONBLOCK);
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(fd,&readfds);
	timeval time;
	time.tv_sec=time.tv_usec=0;

	static int end=0, p=0;
	if(select(fd+1,&readfds, 0, 0, &time) > 0)
		end+=read(fd,buffer+end, (sizeof buffer)-end);
	assert(end < sizeof buffer);
	tLOG<<"dddddddddddd"<<buffer<<endl;
	while(p<end && buffer[p]!='#')
		p++;
	if(p<end)
	{
		buffer[p]='\0';
		istringstream iss(buffer);
		act->load(iss);
		p++;
		for(int i=0;i<end-p;i++)
			buffer[i]=buffer[i+p];
		end-=p;
		p=0;
		return true;
	}
	return false;
}
