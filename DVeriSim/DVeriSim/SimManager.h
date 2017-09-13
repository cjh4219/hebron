#ifndef SIMMANAGER_H
#define SIMMANAGER_H

#include <iostream>
#include <string>
#include "EventInfo.h"
#include "PlayerInfo.h"
#include "DVeriInfo.h"
#include "tinyxml.h"

class SimManager
{
private:
	std::vector<PlayerInfo> _player_info_list;
	EventInfo _event_info;
public:
	SimManager();
	SimManager(std::vector<PlayerInfo> player_info_list, EventInfo event_info);
	~SimManager();

	bool SimReconstruct();
};


#endif