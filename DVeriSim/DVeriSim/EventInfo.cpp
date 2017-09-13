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

	std::list<std::list<TypeWrapper>> returned_data = event_db.query_return_data(w_query);

	for(std::list<std::list<TypeWrapper>>::iterator iter1 = returned_data.begin();iter1!=returned_data.end();iter1++)
	{
		std::list<TypeWrapper>::iterator iter2 = (*iter1).begin();
		EVENT temp;

		int event_id = (*iter2).GetNumber();
		iter2++;
		
		temp.type = (*iter2).GetNumber();
		iter2++;
		
		temp.create_time = (*iter2).GetNumber();
		iter2++;
		
		temp.execute_time = (*iter2).GetNumber();
		iter2++;
		
		temp.priority= (*iter2).GetNumber();
		iter2++;
		
		temp.execute_player_object_id = (*iter2).GetString();
		iter2++;
		
		temp.interface_var_name = (*iter2).GetString();
		iter2++;
		
		temp.create_player_object_id = (*iter2).GetString();
		iter2++;
		
		temp.event_data = (*iter2).GetBlob();
		iter2++;

		temp.brevoke = (*iter2).GetNumber();
		iter2++;
		
		temp.revoke = (*iter2).GetNumber();
		iter2++;

		event_data.insert(std::pair<int, EVENT>(event_id, temp));
	}

	return true;
}