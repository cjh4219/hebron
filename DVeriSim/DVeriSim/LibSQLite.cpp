#include "LibSQLite.h"

bool LibSQLite::sqlite_open(std::wstring dbFileName)
{
	int rc = sqlite3_open16(dbFileName.c_str(), &db);
	if(rc != SQLITE_OK)
	{
		sqlite3_close(db);
		return false;
	}
	return true;
}

bool LibSQLite::sqlite_close()
{
	return (sqlite3_close(db) == SQLITE_OK);
}

void remove_space(std::string& str) {
    int len = str.length();
    int j = 0, i = 0;
    while (i < len) {
        while (str.at(i) == ' ') i++;
        str.at(j) = str.at(i);
        i++;
        j++;
    }
}

std::list<std::list<TypeWrapper>> LibSQLite::query_return_data(std::wstring entered_query)
{
	std::list<std::list<TypeWrapper>> result;
	sqlite3_stmt * stmt;
	const wchar_t * query = entered_query.c_str();
	
	sqlite3_prepare16_v2(this->db, query, -1, &stmt, NULL);
	
	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::list<TypeWrapper> temp_list;

		for(int i = 0 ; ; i++)
		{
			if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
			{
				if(sqlite3_column_type(stmt, i+1) == SQLITE_NULL)
					break;
				else
				{
					const void * temp_pointer = NULL;
					TypeWrapper value_null(3, temp_pointer);
					temp_list.push_back(value_null);
				}
			}
			else if(sqlite3_column_type(stmt, i) == SQLITE_INTEGER) 
			{
				TypeWrapper value_number(1, sqlite3_column_int(stmt, i));
				temp_list.push_back(value_number);
			}
			else if(sqlite3_column_type(stmt, i) == SQLITE_FLOAT) 
			{
				TypeWrapper value_number(1, sqlite3_column_double(stmt, i));
				temp_list.push_back(value_number);
			}
			else if(sqlite3_column_type(stmt, i) == SQLITE_TEXT) 
			{
				std::wstring temp((wchar_t *)sqlite3_column_text16(stmt, i));
				std::string s(temp.begin(), temp.end());
				TypeWrapper value_text(2, s);
				temp_list.push_back(value_text);
			}
			else if(sqlite3_column_type(stmt, i) == SQLITE_BLOB)
			{
				TypeWrapper value_blob(3, sqlite3_column_blob(stmt, i));
				temp_list.push_back(value_blob);

				//size_t size = sqlite3_column_bytes16(stmt, i);
				//std::cout << size << std::endl;
				//const void* test = sqlite3_column_blob(stmt, i);
				/*short lockon;
				double Xad;
				double Yad;
				double Zad;
				double psid;
				double Vd;

				__int16 idx = 0;
				memcpy(&lockon, ((char*)test)+idx, sizeof(__int16));
				lockon = _byteswap_ushort(lockon);
				idx += sizeof(__int16);
				std::cout << lockon << std::endl;
				memcpy(&Xad, ((char*)test)+idx, 8);
				idx += 8;
				std::cout << Xad<< std::endl;
				memcpy(&Yad, ((char*)test)+idx, 8);
				idx += 8;
				std::cout << Yad << std::endl;
				memcpy(&Zad, ((char*)test)+idx, 8);
				idx += 8;
				std::cout << Zad << std::endl;
				memcpy(&psid, ((char*)test)+idx, 8);
				idx += 8;
				std::cout << psid << std::endl;
				memcpy(&Vd, ((char*)test)+idx, 8);
				idx += 8;
				std::cout << Vd << std::endl;
				int _size;
				memcpy(&_size, ((char*)test)+idx, 4);
				idx += 4;
				_size = _byteswap_ulong(_size);
				std::cout << _size << std::endl;
				char* buf = new char[_size];
				memcpy(buf, ((char*)test)+idx, _size);
				idx += _size;
				std::cout << buf << std::endl;*/

				


			}
			/*else if(sqlite3_column_type(stmt, i) == SQLITE_NULL)
			{
				sqlite3_column_text16(stmt, i);
				TypeWrapper value_null(3, NULL);
				temp_list.push_back(value_null);
			}*/
		}
		result.push_back(temp_list);
		temp_list.clear();
	}

	sqlite3_reset(stmt); 
	sqlite3_finalize(stmt);
	return result;
}

bool LibSQLite::query_return_no_data(std::wstring entered_query)
{
	sqlite3_stmt * stmt;    
	
	const wchar_t * query = entered_query.c_str(); 
	bool result = true;

	sqlite3_prepare16_v2(this->db, query, -1, &stmt, NULL);  

	// begin    
	this->begin();   
	if(sqlite3_step(stmt) != SQLITE_DONE) 
	{ 
		fwprintf(stderr, L"line %d: %s\n", __LINE__, sqlite3_errmsg16(this->db));
		result = false;
	}  

	// commit   
	this->commit();  
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	return result;
}

bool LibSQLite::begin()
{
	sqlite3_stmt * stmt;
	const wchar_t * query = QUERY_DB_BEGIN.c_str();
	bool result = true;

	sqlite3_prepare16_v2(this->db, query, 01, &stmt, NULL);
	if(sqlite3_step(stmt) != SQLITE_DONE)
	{
		fwprintf(stderr, L"line %d: %s\n", __LINE__, sqlite3_errmsg16(this->db));
		result = false;
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	return result;
}

bool LibSQLite::commit()
{
	sqlite3_stmt * stmt;
	const wchar_t * query = QUERY_DB_COMMIT.c_str();
	bool result = true;

	sqlite3_prepare16_v2(this->db, query, 01, &stmt, NULL);
	if(sqlite3_step(stmt) != SQLITE_DONE)
	{
		fwprintf(stderr, L"line %d: %s\n", __LINE__, sqlite3_errmsg16(this->db));
		result = false;
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	return result;
}

bool LibSQLite::rollback()
{
	sqlite3_stmt * stmt;
	const wchar_t * query = QUERY_DB_ROLLBACK.c_str();
	bool result = true;

	sqlite3_prepare16_v2(this->db, query, 01, &stmt, NULL);
	if(sqlite3_step(stmt) != SQLITE_DONE)
	{
		fwprintf(stderr, L"line %d: %s\n", __LINE__, sqlite3_errmsg16(this->db));
		result = false;
	}
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);

	return result;
}