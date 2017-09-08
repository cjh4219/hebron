#include "PlayerInfo.h"

PlayerInfo::PlayerInfo()
{
}

PlayerInfo::PlayerInfo(std::string xml_file_name, std::string object_id, std::string db_file_path) : _object_id(object_id), _db_file_path(db_file_path)
{
	level = -1;

	// XML load
	bool loadOkay = doc.LoadFile(xml_file_name.c_str());
	if (!loadOkay)
	{
		printf("Could not load test file 'SimModelStructure.xml'. Error='%s'. Exiting.\n", doc.ErrorDesc());
		exit(1);
	}

	hDoc = TiXmlHandle(&doc);
	pRoot = hDoc.FirstChildElement().Element();

	if (!pRoot)
	{
		std::cout << " Read root element error" << std::endl;
		exit(1);
	}

	// DB load

	std::wstring wstr;
	wstr.assign(_db_file_path.begin(), _db_file_path.end());
	if(!player_db.sqlite_open(wstr))
		std::cout<<"Fails to open DB file"<<std::endl;

	// parse XML
	load_type_info(pRoot);
}

PlayerInfo::~PlayerInfo()
{
	player_db.sqlite_close();
}

bool PlayerInfo::GetStructureInfo()
{
	std::wstring table_name;
	std::wstring w_query;
	std::string query;
	query = "SELECT table_name FROM Object_Info WHERE object_id == \"" + _object_id + "\";";
	w_query.assign(query.begin(), query.end());

	std::list<std::list<std::wstring>> returned_data = player_db.query_return_data(w_query);

	for(list<list<wstring>>::iterator iter1 = returned_data.begin();iter1!=returned_data.end();iter1++)
	{
		for(list<wstring>::iterator iter2 = (*iter1).begin(); iter2!=(*iter1).end(); iter2++)
		{
			table_name = *iter2;
		}
	}
	
	_table_name.assign(table_name.begin(), table_name.end());
	query = "SELECT attribute_name FROM Column_Info WHERE table_name == \""+ _table_name +"\" ORDER BY LENGTH(column_name), column_name ASC;";
	w_query.assign(query.begin(), query.end());
	
	returned_data = player_db.query_return_data(w_query);

	for(list<list<wstring>>::iterator iter1 = returned_data.begin();iter1!=returned_data.end();iter1++)
	{
		for(list<wstring>::iterator iter2 = (*iter1).begin(); iter2!=(*iter1).end(); iter2++)
		{
			std::string str;
			std::wstring wstr = *iter2;
			str.assign(wstr.begin(), wstr.end());
			column_structure_info.push_back(str);
		}
	}
	
	return true;
}

bool PlayerInfo::GetPlayerData()
{
	std::wstring w_query;
	std::string query;
	query = "SELECT * FROM " + _table_name + " ORDER BY CAST(s_time AS DOUBLE) ASC;";
	w_query.assign(query.begin(), query.end());

	std::list<std::list<std::wstring>> returned_data = player_db.query_return_data(w_query);

	for(list<list<wstring>>::iterator iter1 = returned_data.begin();iter1!=returned_data.end();iter1++)
	{
		list<wstring>::iterator iter2 = (*iter1).begin();
		double s_time=std::stod(*iter2, 0);
		std::vector<std::string> temp;

		for(iter2 = ++(*iter1).begin(); iter2!=(*iter1).end(); iter2++)
		{
			std::string str;
			std::wstring wstr = *iter2;
			str.assign(wstr.begin(), wstr.end());
			temp.push_back(str);
		}
		player_data.insert(std::pair<double, std::vector<std::string>>(s_time, temp));
	}

	return true;
}

int PlayerInfo::load_type_info(TiXmlElement* pElem)
{
	level++;
	TiXmlHandle hRoot(0);
	TiXmlElement* pSubElem = pElem;
	TiXmlAttribute * pAttrib = NULL;

	hRoot = TiXmlHandle(pSubElem);
	pSubElem = hRoot.FirstChildElement().Element();
	if (!pSubElem)
	{
		//std::cout << " Read first element error" << std::endl;
		return -1;
	}
	std::string node;
	std::string attr_name;
	std::string attr_value;
	std::string node_data;
	std::string tName;
	std::string tType;

	for (; pSubElem; pSubElem = pSubElem->NextSiblingElement())
	{
		node = std::string((char*)pSubElem->Value());
		
		if (!node.empty())
		{
			/*
			std::cout << level;
			for (int h = 0; h < level * 4; h++)
				std::cout << " ";
			std::cout << "Node : " << node << std::endl;
			*/
		}
		

		pAttrib = pSubElem->FirstAttribute();

		for (; pAttrib; pAttrib = pAttrib->Next())
		{
			std::string attrib_name((char*)pAttrib->Name());
			std::string attrib_value((char*)pAttrib->Value());

			if (!attrib_name.empty() && !attrib_value.empty())
			{
				// extract object name
				if (attrib_name.compare("name") == 0 && level == 0)
				{
					object_name = attrib_value;
				}

				// extract variable name
				if (attrib_name.compare("name") == 0 && level == 1)
				{
					tName = attrib_value;
				}

				// extract variable type
				if (attrib_name.compare("type") == 0 && level == 1)
				{
					tType = attrib_value;
				}

				/*
				std::cout << level;
				for (int h = 0; h < level * 4; h++)
					std::cout << " ";
				std::cout << "  attr name : " << attrib_name << std::endl;
				std::cout << level;
				for (int h = 0; h < level * 4; h++)
					std::cout << " ";
				std::cout << "  attr value : " << attrib_value << std::endl;
				*/
				type_info[tName] = string_to_type(tType);
			}
		}

		if (pSubElem->GetText() != NULL)
		{
			node_data = std::string((char*)pSubElem->GetText());
			/*
			if (!node_data.empty())
			{
				std::cout << level;
				for (int h = 0; h < level * 4; h++)
					std::cout << " ";
				std::cout << "Node data : " << node_data << std::endl;
				
			}
			*/
		}
		PlayerInfo::load_type_info(pSubElem);
		level--;
	}
	return false;
}

void PlayerInfo::print_typeInfo_map()
{
	std::map<std::string, int>::iterator typeInfo_iter;

	for (typeInfo_iter = type_info.begin(); typeInfo_iter != type_info.end(); typeInfo_iter++)
	{
		std::cout << typeInfo_iter->first << " " << typeInfo_iter->second << std::endl;
	}

}

DOUBLE PlayerInfo::string_to_double(STRING sDouble)
{
	double value = std::stod(sDouble);

	return value;
}

INT32 PlayerInfo::string_to_int32(STRING sInt32)
{
	INT32 value = (INT32)std::stoi(sInt32);

	return value;
}

int string_to_type(std::string str)
{
	if ( str.compare("INT16_TYPE") == 0 || str.compare("UINT16_TYPE") == 0 ||
		str.compare("INT32_TYPE") == 0 || str.compare("UINT32_TYPE") == 0 ||
		str.compare("INT64_TYPE") == 0 || str.compare("UINT64_TYPE") == 0)
	{
		return INT32_TYPE;
	}
	else if (str.compare("FLOAT_TYPE") == 0 || str.compare("DOUBLE_TYPE") == 0)
	{
		return DOUBLE_TYPE;
	}
	else
	{
		return STRING_TYPE;
	}
}