#include <stdio.h> 
#include "PlayerInfo.h"
#include "EventInfo.h"
#include "LibSQLite.h"


int main(int argc, char **argv)
{ 
	PlayerInfo pi("component.xml","PO_0", "journal_0001.db");	
	pi.GetStructureInfo();
	pi.GetPlayerData();

	EventInfo ei("journal_0001.db");
	ei.GetEventData();
		
	return 0;
}