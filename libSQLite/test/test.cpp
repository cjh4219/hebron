#include <stdio.h> 
#include <LibSQLite.h>
#include "PlayerInfo.h"


int main(int argc, char **argv)
{ 
	PlayerInfo pi("component.xml","PO_0", "journal_0001.db");
	pi.print_typeInfo_map();
	pi.GetStructureInfo();
	pi.GetPlayerData();
		
	return 0;
}