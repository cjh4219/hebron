#ifndef SCENARIOINFO_H
#define SCENARIOINFO_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include "PlayerInfo.h"
#include "tinyxml.h"

class ScenarioInfo
{
public:
	//pair(id, (name, uuid))
	std::map<std::string, std::vector<std::string>> _object_info_list;

public:
	ScenarioInfo();
	ScenarioInfo(std::string xml_file_name);
	~ScenarioInfo();
		
private:
	int test;
	std::string object_name;
	TiXmlDocument	 doc;
	TiXmlHandle		 hDoc;
	TiXmlElement*	 pRoot;
	std::map<std::string, int> type_info; // <position, type enum>	

public:
	int load_scenario_info(TiXmlElement* pElem);
	
public:
	DOUBLE	string_to_double(STRING sDouble);
	INT32	string_to_int32(STRING sInt32);
	int string_to_type(std::string str);
};

#endif