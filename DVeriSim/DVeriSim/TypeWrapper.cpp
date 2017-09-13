#include "TypeWrapper.h"

TypeWrapper::TypeWrapper(){ }
TypeWrapper::~TypeWrapper(){ }
TypeWrapper::TypeWrapper(int flag, double n_data) : _flag(flag), _n_data(n_data)
{
	_s_data = "";
	_b_data = NULL;
}
TypeWrapper::TypeWrapper(int flag, std::string s_data) : _flag(flag), _s_data(s_data)
{
	_n_data = -1;
	_b_data = NULL;
}
TypeWrapper::TypeWrapper(int flag, const void* b_data) : _flag(flag), _b_data(b_data)
{
	_n_data = -1;
	_s_data = "";
}


double TypeWrapper::GetNumber()
{
	if(_flag == 1)	
		return _n_data;
	else
	{
		std::cout<<"This object contain another type data";
		return NULL;
	}
}

std::string TypeWrapper::GetString()
{
	if(_flag == 2)	
		return _s_data;
	else
	{
		std::cout<<"This object contain another type data";
		return NULL;
	}
}

const void* TypeWrapper::GetBlob()
{
	if(_flag == 3)	
		return _b_data;
	else
	{
		std::cout<<"This object contain another type data";
		return NULL;
	}
}