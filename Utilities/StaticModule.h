#ifndef __STATICMODULE_H
#define __STATICMODULE_H

#include <iostream>
#include <cassert>

template <class T>
class StaticModule
{
protected:
	static T* s_instance;

public:
	static void init()
	{
		if (s_instance)
			assert(0);
		
		s_instance=new T;
	}

	static T& getInstance()
	{
		if (s_instance == 0)
		{
			//cout<<"Module "<<s_instance->myName()<<" is not initialized."<<endl;
			
			assert(0);
		}
		
		//boost::mutex mutex;
		//boost::mutex::scoped_lock scoped_lock(mutex);
		return *s_instance;
	}
	
	static T* getInstancePtr()
	{	
		if (s_instance == 0)
		{
			assert(0);
		}
		return s_instance;
	}
};

template <class T>
T* StaticModule<T>::s_instance = 0;

#endif //__STATICMODULE_H
