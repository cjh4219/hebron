#ifndef SIMMANAGER_H
#define SIMMANAGER_H

#include <iostream>
#include <string>
#include "EventInfo.h"
#include "PlayerInfo.h"

class SimManager
{
private:
	PlayerInfo pi;
	EventInfo ei;
public:
	SimManager();
	~SimManager();
};


#endif