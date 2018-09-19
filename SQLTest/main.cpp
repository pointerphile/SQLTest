#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <stdio.h>
#include <locale>

#ifdef UNICODE
#define Tcout wcout
typedef std::wstring Tstring;
#else
#define Tcout cout
typedef std::string Tstring;
#endif // UNICODE


SQLHENV hEnv;
SQLHDBC hDBC;
SQLHSTMT hSTMT;

static void extract_error(const char *fn, SQLHANDLE handle, SQLSMALLINT handletype) {

	SQLWCHAR sqlstate[1024];
	SQLWCHAR message[1024];

	if (SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL)) {
		std::cout << "Message: ";
		std::wcout << message;
		std::cout << " nSQLSTATE: ";
		std::wcout << sqlstate;
		std::cout << std::endl;
	}
}

int main(int argc, char* argv[]) {
	TCHAR InCon[255];
	Tstring tInCon;
	SQLTCHAR OutCon[1024];
	SQLSMALLINT cbOutCon;
	TCHAR Dir[MAX_PATH];
	SQLRETURN Ret;

	std::locale::global(std::locale("Korean"));

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return -1;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return -1;
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC) != SQL_SUCCESS)
		return -1;

	tInCon = _T("Driver={SQL Server}; SERVER=127.0.0.1, 1433; DATABASE=testDB; Network=dbmssocn; UID=sa; PWD=kgca!@34;");
	Ret = SQLDriverConnect(hDBC, NULL, (SQLTCHAR*)tInCon.c_str(), (SQLSMALLINT)tInCon.length(), OutCon,
		_countof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO)) {
		extract_error("SQLDriverConnect", hDBC, SQL_HANDLE_DBC);
		system("pause");
		return -1;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hSTMT) != SQL_SUCCESS)
		return -1;

	SQLWCHAR username[16];
	SQLLEN lUsername;
	SQLCHAR password[50];
	SQLLEN lPassword;
	SQLINTEGER iReturn = 0;

	SQLBindCol(hSTMT, 1, SQL_C_WCHAR, username, sizeof(username), &lUsername);
	SQLBindCol(hSTMT, 2, SQL_C_BINARY, &password, sizeof(password), &lPassword);

	SQLBindParameter(hSTMT, 1, SQL_PARAM_OUTPUT, sql_c_)

	// SQL���� �����Ѵ�.
	if (SQLExecDirect(hSTMT, (SQLTCHAR *)_T("call uspSignUp('test', '1234')"), SQL_NTS) != SQL_SUCCESS) {
		return FALSE;
	}
	if (SQLExecDirect(hSTMT, (SQLTCHAR *)_T("select * from testTable"), SQL_NTS) != SQL_SUCCESS) {
		return FALSE;
	}

	// �о�� ������ ���
	while (SQLFetch(hSTMT) != SQL_NO_DATA) {
		std::Tcout << "\t" << username << "\t" << "\t" << password << std::endl;
	}

	if (hSTMT) SQLCloseCursor(hSTMT);
	if (hSTMT) SQLFreeHandle(SQL_HANDLE_STMT, hSTMT);
	if (hDBC) SQLDisconnect(hDBC);
	if (hSTMT) SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	system("pause");

	return 0;
}