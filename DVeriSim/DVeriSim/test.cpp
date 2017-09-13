#include <stdio.h> 
#include "PlayerInfo.h"
#include "EventInfo.h"
#include "SimManager.h"
#include "LibSQLite.h"
#include "ScenarioInfo.h"


int main(int argc, char **argv)
{ 
	std::vector<PlayerInfo> player_info_list;
	ScenarioInfo scenario_info("scenario.xml");
	
	std::map<std::string, std::vector<std::string>>::iterator iter;
	PlayerInfo temp;
	for(iter = scenario_info._object_info_list.begin();iter != scenario_info._object_info_list.end();iter++)
	{
		std::string xml_name = iter->second[0] + "_" + iter->second[1] + ".xml";
		temp = PlayerInfo(xml_name, iter->first, "journal_0001.db");

		temp.GetStructureInfo();
		temp.GetPlayerData();

		player_info_list.push_back(temp);		
	}

	EventInfo ei("journal_0001.db");
	ei.GetEventData();
	
	SimManager sm(player_info_list, ei);
	sm.SimReconstruct();
	return 0;
}