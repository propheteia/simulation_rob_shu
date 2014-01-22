#include "common.h"
#include <fstream>
#include <cstring>
#include "Logger.h"

using namespace std;
int conv[]={2,0,3,1,5,4,7,6,9,8,13,11,12,10,15,14,18,16,19,17,20,21};
void joint_name_to_number(const string &n, int &a, int &b)
{
	if(n=="raj1")
		a=0;
	else if(n=="laj1")
		a=1;
	else if(n=="raj2")
		a=2;
	else if(n=="laj2")
		a=3;
	else if(n=="raj3")
		a=4;
	else if(n=="laj3")
		a=5;
	else if(n=="raj4")
		a=6;
	else if(n=="laj4")
		a=7;
	else if(n=="rlj1")
		a=8;
	else if(n=="llj1")
		a=9;
	else if(n=="rlj2")
		a=10;
	else if(n=="llj2")
		a=11;
	else if(n=="rlj3")
		a=12;
	else if(n=="llj3")
		a=13;
	else if(n=="rlj4")
		a=14;
	else if(n=="llj4")
		a=15;
	else if(n=="rlj5")
		a=16;
	else if(n=="llj5")
		a=17;
	else if(n=="rlj6")
		a=18;
	else if(n=="llj6")
		a=19;
	else if(n=="hj1")
		a=20;
	else if(n=="hj2")
		a=21;
	else
		cerr<<"joint_name_to_number"<<endl;
}

string joint_number_to_name[]={
	"rae1",
	"lae1",
	"rae2",
	"lae2",
	"rae3",
	"lae3",
	"rae4",
	"lae4",
	"rle1",
	"lle1",
	"rle2",
	"lle2",
	"rle3",
	"lle3",
	"rle4",
	"lle4",
	"rle5",
	"lle5",
	"rle6",
	"lle6",
	"he1",
	"he2"
};

state::state()
{
	min_time=max_time=-1;
	for (int i = 0; i < 20; i++)
	{
		angle[i] = 0;
		gain[i] = 0;
		precision[i] = 0;
		imp[i] = false;
		tLOG << angle[i] << ' ' << gain[i] << ' ' << precision[i] << ' ' << imp[i] << endl;
	}
}

action::action()
{
	repeat=0;
	name="_";
}

bool action::load(istream &is)
{
	int number;
	if(!(is>>name>>number>>repeat))
		return false;
	tLOG << "name: " << name << " number: " << number << " repeat: " << repeat << endl; 
	arr = vector<state>(number);
	for(int i=0;i<number;i++)
	{
		if(!(is>>arr[i].min_time>>arr[i].max_time))
			return false;
		else
			tLOG<<" min_time: "<<arr[i].min_time<<" arr[i]: "<<arr[i].max_time<<endl;
		for(int j=0;j<20;j++)
			if(!(is>>arr[i].angle[j]))
				return false;
			else
				tLOG<<"angle: "<<arr[i].angle[j]<<endl;
		for(int j=0;j<20;j++)
			if(!(is>>arr[i].gain[j]))
				return false;
			else
				tLOG<<" gain: "<<arr[i].gain[j]<<endl;
				
		for(int j=0;j<20;j++)
			if(!(is>>arr[i].precision[j]))
				return false;
			else
			{	tLOG<<" precision[i]: "<<arr[i].precision[j]<<endl;
                         }
		for(int j=0;j<20;j++)
			if(!(is>>arr[i].imp[j]))
			{	
				return false;
			}
			else
				tLOG<<" imp[i]: "<<arr[i].imp[j]<<endl;
		
	}
	char t;
	if(!(is>>t))
		return false;
	return t=='#';
}

void action::save(ostream &os)
{
	if(name=="")
		name="_";
	os<<name<<"\n"<<arr.size()<<" "<<repeat<<"\n";
	for(unsigned int i=0;i<arr.size();i++)
	{
		os<<arr[i].min_time<<" "<<arr[i].max_time<<"\n";
		for(int j=0;j<20;j++)
			os<<arr[i].angle[j]<<" ";
		os<<"\n";
		for(int j=0;j<20;j++)
			os<<arr[i].gain[j]<<" ";
		os<<"\n";
		for(int j=0;j<20;j++)
			os<<arr[i].precision[j]<<" ";
		os<<"\n";
		for(int j=0;j<20;j++)
			os<<arr[i].imp[j]<<" ";
		os<<"\n";
	}
	os<<"#\n"<<endl;
}

command::command()
{
	memset(power, 0, sizeof power);
}

