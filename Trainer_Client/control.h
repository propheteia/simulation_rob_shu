#ifndef _CONTROL_H
#define _CONTROL_H
using namespace std;
#include<sstream>
#include "common.h"
//#include "Enum.h"
class body;
//class world;
class connection;

class control
{
public:
	control();
	bool go_to_state(state *,command *);
	bool get_rate(float angle,float curAngle,float maxVel,float precision,float &rate);
	string do_command(command *c);
	string do_action(action *);
	void reset();

protected:

	int state_number;
	std::string previous_action_name;
	int         previous_action_size;
	int         previous_repeat_number;
	float state_time;
	command cmd;






};

#endif
