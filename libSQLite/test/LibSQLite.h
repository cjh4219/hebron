#ifndef LIBSQLITE_H
#define LIBSQLITE_H

#include <stdio.h>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include "sqlite3.h"

using namespace std;

static const wstring QUERY_DB_BEGIN = L"BEGIN;";
static const wstring QUERY_DB_COMMIT = L"COMMIT;";
static const wstring QUERY_DB_ROLLBACK = L"ROLLBACK;";

class LibSQLite
{
private:
	sqlite3 *db;

public:	
	bool sqlite_open(wstring dbFileName);
	bool sqlite_close();

	//bool upsert_data(wstring filePath, wstring tableName, wstring checksum, void* binaryData, int size);
	//bool delete_data(wstring key, wstring tableName, wstring filePath);
	//bool create_table(wstring tableName, map<wstring, wstring> columns);
	//bool drop_table(wstring tableName);
	//bool is_table_exist(wstring tableName);

	std::list<std::list<wstring>> query_return_data(wstring entered_query);
	bool query_return_no_data(wstring entered_query);
	
private:
	bool begin();
	bool commit();
	bool rollback();
} ;

#endif