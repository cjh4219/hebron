#ifndef LIBSQLITE_H
#define LIBSQLITE_H

#include <stdio.h>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include "sqlite3.h"
#include "TypeWrapper.h"

static const std::wstring QUERY_DB_BEGIN = L"BEGIN;";
static const std::wstring QUERY_DB_COMMIT = L"COMMIT;";
static const std::wstring QUERY_DB_ROLLBACK = L"ROLLBACK;";

class LibSQLite
{
private:
	sqlite3 *db;

public:	
	bool sqlite_open(std::wstring dbFileName);
	bool sqlite_close();
	
	std::list<std::list<TypeWrapper>> query_return_data(std::wstring entered_query);
	bool query_return_no_data(std::wstring entered_query);
	
private:
	bool begin();
	bool commit();
	bool rollback();
} ;

#endif