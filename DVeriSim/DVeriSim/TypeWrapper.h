#ifndef TYPEWRAPPER_H
#define TYPEWRAPPER_H

#include <iostream>
#include <string>

class TypeWrapper
{
public:
	int _flag;	//1: _n_data, 2: _s_data, 3: _b_data

	double _n_data;
	std::string _s_data;
	const void* _b_data;

public:
	TypeWrapper();
	TypeWrapper(int flag, double d_data);
	TypeWrapper(int flag, std::string s_data);
	TypeWrapper(int flag, const void* b_data);
	~TypeWrapper();

public:
	double GetNumber();
	std::string GetString();
	const void* GetBlob();
};


#endif