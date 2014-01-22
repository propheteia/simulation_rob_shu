#ifndef _TRAINER_CLIENT_H
#define _TRAINER_CLIENT_H

#include <string>
#include "common.h"

class trainer_client
{
public:
	trainer_client();
	bool get_action(action *);
private:
	int fd;
	char buffer[10000];
};

#endif
