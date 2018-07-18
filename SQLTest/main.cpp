#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <stdio.h>

SQLHENV hEnv;
SQLHDBC hDBC;
SQLHSTMT hSTMT;

int main(int argc, char* argv[]) {
	TCHAR InCon[255];
	SQLTCHAR OutCon[1024];
	SQLSMALLINT cbOutCon;
	TCHAR Dir[MAX_PATH];
	SQLRETURN Ret;

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return -1;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return -1;
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC) != SQL_SUCCESS)
		return -1;

	_stprintf(InCon, _T("%s"), _T("Driver={SQL Server}; SERVER=localhost; DATABASE=testDB; Network=dbmssocn; UID = sa; PWD = kgca!@34;"));
	Ret = SQLDriverConnect(hDBC, NULL, (SQLTCHAR*)InCon, _countof(InCon), OutCon,
		_countof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO))
		return -1;

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hSTMT) != SQL_SUCCESS)
		return -1;

	if (hSTMT) SQLCloseCursor(hSTMT);
	if (hSTMT) SQLFreeHandle(SQL_HANDLE_STMT, hSTMT);
	if (hDBC) SQLDisconnect(hDBC);
	if (hSTMT) SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	system("pause");

	return 0;
} 