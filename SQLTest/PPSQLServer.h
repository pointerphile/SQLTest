#pragma once
#include <iostream>
#include <string>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <stdio.h>
#include <clocale>

class PPSQLServer {
private:
	enum m_enumCursor { None, Select, Insert, Update, Delete, Exit};
	SQLHENV hEnv;
	SQLHDBC hDBC;
	SQLHSTMT hSTMT;
	TCHAR tInCon[255];
	std::wstring wInCon;
	SQLTCHAR OutCon[1024];
	SQLSMALLINT cbOutCon;
	TCHAR Dir[MAX_PATH];
	SQLRETURN Ret;
public:
	PPSQLServer();
	virtual ~PPSQLServer();
public:
	int Init();
	int Run();
	int Release();
public:
	int SelectAll();
	int InsertUser(std::wstring wstrUsername, std::wstring wstrPassword);
	int DeleteUser(std::wstring wstrUsername, std::wstring wstrPassword);
	int UpdateUser(std::wstring wstrUsername, std::wstring wstrPassword);
	static int DisplayError(const char *fn, SQLHANDLE handle, SQLSMALLINT handletype);
};

