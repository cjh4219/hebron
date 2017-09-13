#include "ScenarioInfo.h"

ScenarioInfo::ScenarioInfo()
{
}

ScenarioInfo::~ScenarioInfo()
{
}

ScenarioInfo::ScenarioInfo(std::string xml_file_name)
{	
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

	// parse XML
	//load_type_info(pRoot);
	load_scenario_info(pRoot);
}

int ScenarioInfo::load_scenario_info(TiXmlElement* pElem)
{
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

	pSubElem = pSubElem->NextSiblingElement();
	std::string tag_objects;
	tag_objects = std::string((char*)pSubElem->Value());

	if(tag_objects.compare("objects") == 0)
	{
		TiXmlElement* pObjectElem = pSubElem->FirstChildElement();
		//search objects tag
		for(; pObjectElem; pObjectElem = pObjectElem->NextSiblingElement())
		{
			std::string tag_object;
			tag_object = std::string((char*)pObjectElem->Value());
			if(tag_object.compare("object") == 0)
			{
				TiXmlElement* pObjectInfoElem = pObjectElem->FirstChildElement();
			
				std::string tag_objectInfo;
				tag_objectInfo = std::string((char*)pObjectInfoElem->Value());

				if(tag_objectInfo.compare("object_info") == 0)
				{
					std::vector<std::string> temp;

					pAttrib = pObjectInfoElem->FirstAttribute();

					for (; pAttrib; pAttrib = pAttrib->Next())
					{
						attr_name = std::string((char*)pAttrib->Name());
						attr_value = std::string((char*)pAttrib->Value());

						if (!attr_name.empty() && !attr_value.empty())
						{
							// extract component name
							if (attr_name.compare("component_name") == 0)
							{
								temp.push_back(attr_value);
							}
							// extract component uuid
							else if (attr_name.compare("component_uuid") == 0)
							{
								temp.push_back(attr_value);
							}
							// extract id
							else if (attr_name.compare("id") == 0)
							{
								object_name = attr_value;
							}
						}
					}
					_object_info_list.insert(std::pair<std::string, std::vector<std::string>>(object_name, temp));
				}
			}
		}
	}
	return false;
}

DOUBLE ScenarioInfo::string_to_double(STRING sDouble)
{
	double value = std::stod(sDouble);

	return value;
}

INT32 ScenarioInfo::string_to_int32(STRING sInt32)
{
	INT32 value = (INT32)std::stoi(sInt32);

	return value;
}

int ScenarioInfo::string_to_type(std::string str)
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