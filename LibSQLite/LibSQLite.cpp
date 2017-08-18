#include "LibSQLite.h"

extern "C" __declspec(dllexport) bool LibSQLite::sqlite_open(wstring dbFileName)
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

bool LibSQLite::upsert_data(wstring filePath, wstring tableName, wstring checksum, void* binaryData, int size)
{
	sqlite3_stmt * stmt;  

	wstring QUERY_TABLE_UPSERT_RECORD = L"INSERT OR REPLACE INTO ";
	QUERY_TABLE_UPSERT_RECORD += tableName;
	QUERY_TABLE_UPSERT_RECORD += L" VALUES(?, ?, ?);";

	const wchar_t * query = QUERY_TABLE_UPSERT_RECORD.c_str();  
	bool result = true; 

	sqlite3_prepare16_v2(this->db, query, -1, &stmt, NULL);  
	sqlite3_bind_text16(stmt, 1, filePath.c_str(), -1,SQLITE_STATIC);   
	sqlite3_bind_text16(stmt, 22, checksum.c_str(), -1,SQLITE_STATIC);  
	
	if(binaryData == NULL) 
	{        
		sqlite3_bind_blob(stmt, 23, NULL, -1,SQLITE_STATIC);
	} 
	else 
	{
		sqlite3_bind_blob(stmt, 23, binaryData, size, SQLITE_TRANSIENT);
	}   
	
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

bool LibSQLite::delete_data(wstring key, wstring tableName, wstring filePath)
{
	sqlite3_stmt * stmt;    

	wstring QUERY_TABLE_DELETE_RECORD = L"DELETE FROM ";
	QUERY_TABLE_DELETE_RECORD += tableName;
	QUERY_TABLE_DELETE_RECORD += L" WHERE FilePath=?;";

	const wchar_t * query = QUERY_TABLE_DELETE_RECORD.c_str(); 
	bool result = true;
	
	sqlite3_prepare16_v2(this->db, query, -1, &stmt, NULL);  
	sqlite3_bind_text16(stmt, 1, filePath.c_str(), -1,SQLITE_STATIC);       
	
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

bool LibSQLite::create_table(wstring tableName, map<wstring, wstring> columns)
{
	sqlite3_stmt * stmt;

	wstring QUERY_TABLE_CREATE = L"CREATE TABLE ";
	QUERY_TABLE_CREATE += tableName;
	QUERY_TABLE_CREATE += L" (";
	for(std::map<wstring, wstring>::iterator it = columns.begin(); it != columns.end(); ++it)
	{
		QUERY_TABLE_CREATE += it->first;
		QUERY_TABLE_CREATE += L"";
	}
	QUERY_TABLE_CREATE += L");";

	const wchar_t * query = QUERY_TABLE_CREATE.c_str();
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

bool LibSQLite::drop_table(wstring tableName)
{
	sqlite3_stmt * stmt;

	wstring QUERY_TABLE_DROP = L"INSERT OR REPLACE INTO ";
	QUERY_TABLE_DROP += tableName;
	QUERY_TABLE_DROP += L" VALUES(?, ?, ?);";

	const wchar_t * query = QUERY_TABLE_DROP.c_str();
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

bool LibSQLite::is_table_exist(wstring tableName)
{
	sqlite3_stmt * stmt;

	wstring QUERY_TABLE_EXIST = L"INSERT OR REPLACE INTO ";
	QUERY_TABLE_EXIST += tableName;
	QUERY_TABLE_EXIST += L" VALUES(?, ?, ?);";

	const wchar_t * query = QUERY_TABLE_EXIST.c_str();
	bool result = false;

	sqlite3_prepare16_v2(this->db, query, -1, &stmt, NULL); 

	if(sqlite3_step(stmt) == SQLITE_ROW)
	{
		result = true; 
	}
	sqlite3_reset(stmt);  
	sqlite3_finalize(stmt);
	return result;
}

std::list<std::list<wstring>> LibSQLite::query_return_data(wstring entered_query)
{
	std::list<std::list<std::wstring>> result;
	sqlite3_stmt * stmt;
	const wchar_t * query = entered_query.c_str();
	
	sqlite3_prepare16_v2(this->db, query, -1, &stmt, NULL);
	
	while(sqlite3_step(stmt) == SQLITE_ROW)
	{
		std::list<wstring> temp_list;
		int i;
		for(i = 0 ; sqlite3_column_type(stmt, i) != SQLITE_NULL ; i++)
		{
			if(sqlite3_column_type(stmt, i) == SQLITE_TEXT) 
			{
				std::wstring value_text = (wchar_t *)sqlite3_column_text16(stmt, i);
				temp_list.push_back(value_text); 
			}
			else if(sqlite3_column_type(stmt, i) == SQLITE_INTEGER) 
			{
				std::wstring value_integer = std::to_wstring((_Longlong)sqlite3_column_int(stmt, i));  
				temp_list.push_back(value_integer); 
			}
			else if(sqlite3_column_type(stmt, i) == SQLITE_FLOAT) 
			{
				std::wstring value_double = std::to_wstring((long double)sqlite3_column_double(stmt, i));
				temp_list.push_back(value_double); 
			}
		}
		result.push_back(temp_list);
		temp_list.clear();
	}

	sqlite3_reset(stmt); 
	sqlite3_finalize(stmt);
	return result;
}

bool LibSQLite::query_return_no_data(wstring entered_query)
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