#include "SimManager.h"

SimManager::SimManager()
{
}

SimManager::SimManager(std::vector<PlayerInfo> player_info_list, EventInfo event_info) : _player_info_list(player_info_list), _event_info(event_info)
{
}

SimManager::~SimManager()
{
}

bool SimManager::SimReconstruct()
{
	std::cout<<"Simulation starts"<<std::endl;
	std::cout<<"SimTime"<<'\t'<<'\t'<<"Event"<<std::endl;

	for(int i=1;i<=_event_info.event_data.size();i++)
	{
		if(_event_info.event_data[i].type == 0)
		{
			//object initialize event
			std::cout.precision(2);
			std::cout<< _event_info.event_data[i].create_time<< '\t'<<'\t' <<_event_info.event_data[i].execute_player_object_id + " is initialized"<<std::endl;
		}
		else if(_event_info.event_data[i].type == 1)
		{
			//object update event
			std::cout.precision(2);
			std::cout<< _event_info.event_data[i].create_time<< '\t'<<'\t' <<_event_info.event_data[i].execute_player_object_id + " is updated"<<std::endl;

		}
		else if(_event_info.event_data[i].type == 2)
		{
			//send message event
			std::cout<<""<<std::endl;

		}
	}
	return true;
}