#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "LibSQLite.h"
#include "TypeWrapper.h"
#include "tinyxml.h"

typedef char CHAR;
typedef std::string STRING;
typedef __int16 INT16;
typedef unsigned __int16 UINT16;
typedef __int32 INT32;
typedef unsigned __int32 UINT32;
typedef __int64 INT64;
typedef unsigned __int64 UINT64;
typedef float FLOAT;
typedef double DOUBLE;

// TO DO : additional type support needed
enum attr_type
{
	INT16_TYPE = 1,
	UINT16_TYPE = 2,
	INT32_TYPE = 3,
	UINT32_TYPE = 4,
	INT64_TYPE = 5,
	UINT64_TYPE = 6,
	FLOAT_TYPE = 7,
	DOUBLE_TYPE = 8,
	CHAR_TYPE = 9,
	STRING_TYPE = 10
};

int string_to_type(std::string str);

class PlayerInfo
{
private:
	std::string _db_file_path;
	LibSQLite player_db;

public:
	std::string _object_id;
	std::string _table_name;
	std::vector<std::string> column_structure_info;
	std::map<double, std::vector<TypeWrapper>> player_data;

private:
	int test;
	std::string object_name;
	TiXmlDocument	 doc;
	TiXmlHandle		 hDoc;
	TiXmlElement*	 pRoot;
	int				 level;				// hierarchy level
	std::map<std::string, int> type_info; // <position, type enum>	

public:
	PlayerInfo();
	PlayerInfo(std::string xml_file_name, std::string object_id, std::string db_file_path);
	~PlayerInfo();

public:
	int		load_type_info(TiXmlElement* pElem);
	void	print_typeInfo_map();
	bool GetStructureInfo();
	bool GetPlayerData();
	//object id, attributes

public:
	DOUBLE	string_to_double(STRING sDouble);
	INT32	string_to_int32(STRING sInt32);
	int string_to_type(std::string str);
};

#endif