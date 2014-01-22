#include <math.h>
#include <iostream>
#include <NaoWalkingEngine.h>

#include "control.h"
#include "common.h"
#include "worldmodel.h"
#include "nao.h"
#include "gmath.h"
#include "Logger.h"

using namespace std;
using namespace boost;


control::control()
{
	state_number = 0;
	previous_action_name = "";
	previous_action_size = 0;
	previous_repeat_number = 0;
	state_time = 0;

}

int transJointAngle(int a)
{
	if(a%2==1) a+=3;
	else a+=5;
	return a;
}

bool control::go_to_state(state *st, command *cm)//agar reside bashe true bar migardoone
{
	shared_array<Robot::Link> JointAngle = NAO->GetLink();
	bool flag=true;
	for(int i=0;i<20;i++)
	{	if(	st->imp[i] && !get_rate(st->angle[i],gRadToDeg(JointAngle[transJointAngle(i)].q),
				st->gain[i],st->precision[i],cm->power[i]))
			flag=false;
	tLOG << "JointID:"<< transJointAngle(i) << " "<<gRadToDeg(JointAngle[transJointAngle(i)].q) << "power"<<cm->power[i]<<endl;
	}
	tLOG<<"time"<<st->max_time<<" "<<WM.getMySimTime()<<" "<<state_time<<" "<<flag<<endl;
	return st->max_time > 0 && WM.getMySimTime() - state_time > st->max_time
		|| flag && (st->min_time < 0|| WM.getMySimTime() - state_time > st->min_time);
}

string control::do_command(command *c)
{	tLOG<<"end-cmd!!!!!!!!!!!!!!"<< c->power[1] <<endl;
	char s[100];
	string Command;

	for(int i=0;i<22;i++)
	{	tLOG<<"finalpower"<<cmd.power[i]<<endl;
		sprintf(s,"(%s %.2lf)",joint_number_to_name[i].c_str(),c->power[i]);
		tLOG << "c->power[" << i << "]: " << c->power[i] << endl;
		Command += s;
	}
	return Command;
}
//residan be zavie morede nazar         final_angle current_angle gain  precision   rate
bool control::get_rate(float angle,float curAngle,float maxVel,float precision,float &rate)
{	//tLOG<<"getrate ok"<<endl;
    if (maxVel < 0)
    {
       tLOG << "NaoAction ERROR: (CalculateVel) "
                          << "maxVel < 0" << endl;
       rate = 0.0f;
    }

    float minus = gNormalizeDeg(angle - curAngle);
    rate = 0.0;

    rate = gAbs(minus) > maxVel ? maxVel * gSign(minus) : minus;
    rate = std::min(gDegToRad(rate) * 10.0f, 100.0f);
	tLOG << "minus: " << minus << "       rate: " << rate << endl;

	return fabs(rate)<precision;
}

void control::reset()
{
	state_number = 0;
	state_time = WM.getMySimTime();
}

string control::do_action(action *act)
{
	if(act->name == "_"||act->name == "")
		;
		//cerr<< "Action with no name" << endl;
	previous_action_name = act->name;
	previous_action_size = act->arr.size();
	previous_repeat_number = act->repeat;
	int t = state_number;
//	act->save(cout);
	tLOG<<"statenum&arrsize"<<state_number<<" "<<act->arr.size()<<endl;
	while(state_number < act->arr.size() && go_to_state(&act->arr[state_number],&cmd))
	{	tLOG<<"start-cmd!!!!!!!!!!!!!!"<< cmd.power[1] <<endl;
 		state_time = WM.getMySimTime();
		for(int i=0;i<20;i++) tLOG << act->arr[state_number].imp[i] << endl;
 		state_number++;
 		if(state_number >= act->arr.size())
 			state_number = act->repeat;
		if(state_number == t)
			break;
	}
		string Command = do_command(&cmd);
	tLOG << "Command: " << Command << endl;
	tLOG << "************************************do_action end*********************************" << endl;
	return Command;
}
