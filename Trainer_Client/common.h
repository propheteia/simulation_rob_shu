#ifndef _COMMON_H
#define _COMMON_H

#include <iostream>
#include <string>
#include <vector>

void joint_name_to_number(const std::string &, int &, int &);
extern std::string joint_number_to_name[];

struct command
{
	command();
	float power[20];
};

struct state
{
	state();
	double min_time;
	double max_time;
	float angle[20];
	float gain[20];
	float precision[20];
	bool imp[20];
};

struct action
{
	action();
	bool load(std::istream &);
	void save(std::ostream &);

	int repeat;
	std::string name;
	std::vector<state> arr;
};

#endif
