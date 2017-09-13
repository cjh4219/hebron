#ifndef EVENTINFO_H
#define EVENTINFO_H

#include <iostream>
#include <string>
#include "LibSQLite.h"

typedef struct EVENT{
	int type;
	double create_time;
	double execute_time;
	int priority;
	std::string execute_player_object_id;
	std::string interface_var_name;
	std::string create_player_object_id;
	const void* event_data;
	int brevoke;
	double revoke;
}EVENT;

class EventInfo
{
public:
	std::string _db_file_path;
	LibSQLite event_db;
	std::map<int, EVENT> event_data;

public:
	EventInfo();
	EventInfo(std::string db_file_path);
	~EventInfo();
	bool GetEventData();
};


#endif