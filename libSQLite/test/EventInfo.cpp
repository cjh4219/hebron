#include "EventInfo.h"

EventInfo::EventInfo()
{
}

EventInfo::EventInfo(std::string db_file_path) : _db_file_path(db_file_path)
{
	std::wstring wstr;
	wstr.assign(_db_file_path.begin(), _db_file_path.end());
	if(!event_db.sqlite_open(wstr))
		std::cout<<"Fails to open DB file"<<std::endl;
}

EventInfo::~EventInfo()
{
	event_db.sqlite_close();
}

bool EventInfo::GetEventData()
{
	std::wstring w_query;
	std::string query;
	query = "SELECT * FROM Event_List ORDER BY event_id ASC;";
	w_query.assign(query.begin(), query.end());

	std::list<std::list<std::wstring>> returned_data = event_db.query_return_data(w_query);

	for(list<list<wstring>>::iterator iter1 = returned_data.begin();iter1!=returned_data.end();iter1++)
	{
		list<wstring>::iterator iter2 = (*iter1).begin();
		int evt_id = std::stoi(*iter2, 0);
		
	}

	return true;
}