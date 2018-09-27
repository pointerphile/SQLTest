#define _CRT_SECURE_NO_WARNINGS
#include "PPSQLServer.h"

PPSQLServer::PPSQLServer() {}
PPSQLServer::~PPSQLServer() {}

int PPSQLServer::Init()
{
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
		return -1;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return -1;
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDBC) != SQL_SUCCESS)
		return -1;

	wInCon = _T("Driver={SQL Server}; SERVER=192.168.0.110, 1433; DATABASE=testDB; Network=dbmssocn; UID=sa; PWD=kgca!@34;");
	Ret = SQLDriverConnect(hDBC, NULL, (SQLTCHAR*)wInCon.c_str(), (SQLSMALLINT)wInCon.length(), OutCon,
		_countof(OutCon), &cbOutCon, SQL_DRIVER_NOPROMPT);
	if ((Ret != SQL_SUCCESS) && (Ret != SQL_SUCCESS_WITH_INFO)) {
		DisplayError("SQLDriverConnect", hDBC, SQL_HANDLE_DBC);
		system("pause");
		return -1;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hSTMT) != SQL_SUCCESS)
		return -1;

	return 0;
}

int PPSQLServer::Run()
{
	int Cursor = m_enumCursor::None;
	while (Cursor != m_enumCursor::Exit) {
		std::cout << "1 : Select All" << std::endl;
		std::cout << "2 : Insert User" << std::endl;
		std::cout << "3 : Update User" << std::endl;
		std::cout << "4 : Delete User" << std::endl;
		std::cout << "5 : Exit" << std::endl;
		std::cin >> Cursor;
		switch (Cursor) {
			case Select: {
				SQLWCHAR username[17];
				SQLLEN lUsername;
				SQLCHAR password[51];
				SQLLEN lPassword;
				SQLINTEGER iReturn = 0;

				SQLBindCol(hSTMT, 1, SQL_C_WCHAR, username, sizeof(username), &lUsername);
				SQLBindCol(hSTMT, 2, SQL_C_BINARY, password, sizeof(password), &lPassword);

				if (SQLExecDirect(hSTMT, (SQLTCHAR *)_T("select * from testTable"), SQL_NTS) != SQL_SUCCESS) {
					return FALSE;
				}

				// 읽어온 데이터 출력
				while (SQLFetch(hSTMT) != SQL_NO_DATA) {
					std::wcout << "\t" << username << "\t" << "\t" << *password << "(encrypted by SHA-1)" << std::endl;
				}

				if (hSTMT) SQLCloseCursor(hSTMT);

				break;
			}
			case Insert: {
				std::wstring wstrUsername;
				std::wstring wstrPassword;
				std::wstring wstrRetypePassword;
				SQLWCHAR username[17];
				SQLLEN lUsername = SQL_NTS;
				SQLWCHAR password[17];
				SQLLEN lPassword = SQL_NTS;
				SWORD sParm1 = 0;
				SQLLEN cbParm1 = SQL_NTS;
				SQLINTEGER iReturn = 0;
				RETCODE retcode;

				std::cout << "Username: ";
				std::wcin >> wstrUsername;
				std::cout << "Password: ";
				std::wcin >> wstrPassword;
				std::cout << "Retype Password: ";
				std::wcin >> wstrRetypePassword;
				if (wstrPassword != wstrRetypePassword) {
					std::cout << "Retyped password doesn't match with the first one." << std::endl;
					break;
				}
				else {
					wcscpy(username, wstrUsername.c_str());
					wcscpy(password, wstrPassword.c_str());
					std::cout << "Inserting... " << std::endl;
					SQLBindParameter(hSTMT, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &username, wstrUsername.length(), &lUsername);
					SQLBindParameter(hSTMT, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &password, wstrPassword.length(), &lPassword);

					retcode = SQLExecDirect(hSTMT, (SQLTCHAR *)_T("{CALL usp_SignUp (?, ?)}"), SQL_NTS);
					if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
						std::cout << "Failed... " << std::endl;
					}
					else { DisplayError("SQLDriverConnect", hDBC, SQL_HANDLE_DBC); }
				}

				break;
			}
			case Update: {
				std::wstring wstrUsername;
				std::wstring wstrNewUsername;
				std::wstring wstrPassword;
				std::wstring wstrRetypePassword;
				SQLWCHAR username[17];
				SQLLEN lUsername = SQL_NTS;
				SQLWCHAR newusername[17];
				SQLLEN lnewUsername = SQL_NTS;
				SQLWCHAR password[17];
				SQLLEN lPassword = SQL_NTS;
				SWORD sParm1 = 0;
				SQLLEN cbParm1 = SQL_NTS;
				SQLINTEGER iReturn = 0;
				RETCODE retcode;

				std::cout << "Username: ";
				std::wcin >> wstrUsername;
				std::cout << "Password: ";
				std::wcin >> wstrPassword;
				std::cout << "Retype Password: ";
				std::wcin >> wstrRetypePassword;
				if (wstrPassword != wstrRetypePassword) {
					std::cout << "Retyped password doesn't match with the first one." << std::endl;
					break;
				}
				else {
					wcscpy(username, wstrUsername.c_str());
					wcscpy(password, wstrPassword.c_str());
					std::cout << "Checking... " << std::endl;
					
					SQLBindParameter(hSTMT, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &username, wstrUsername.length(), &lUsername);
					SQLBindParameter(hSTMT, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &password, wstrPassword.length(), &lPassword);
					SQLBindParameter(hSTMT, 3, SQL_PARAM_OUTPUT, SQL_C_SSHORT, SQL_INTEGER, 0, 0, &sParm1, 0, nullptr);
					retcode = SQLExecDirect(hSTMT, (SQLTCHAR *)_T("{CALL usp_SignIn (?, ?, ?)}"), SQL_NTS);
					if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
						std::cout << "Failed... " << std::endl;
						break;
					}
					if (sParm1 != 0) {
						std::cout << "Check your Username and Password again. " << std::endl;
						break;
					}
					else {
						std::cout << "Enter your new Username: ";
						std::wcin >> wstrNewUsername;
						wcscpy(newusername, wstrNewUsername.c_str());

						SQLBindParameter(hSTMT, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &username, wstrUsername.length(), &lUsername);
						SQLBindParameter(hSTMT, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &newusername, wstrNewUsername.length(), &lnewUsername);
						retcode = SQLExecDirect(hSTMT, (SQLTCHAR *)_T("{CALL usp_UpdateUsername (?, ?)}"), SQL_NTS);
						if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
							std::cout << "Failed... " << std::endl;
							break;
						}
						else {
							std::cout << "Username Updated... " << std::endl;
							break;
						}
					}
				}

				break;
			}
			case Delete: {
				std::wstring wstrUsername;
				std::wstring wstrPassword;
				std::wstring wstrRetypePassword;
				SQLWCHAR username[17];
				SQLLEN lUsername = SQL_NTS;
				SQLWCHAR password[17];
				SQLLEN lPassword = SQL_NTS;
				SWORD sParm1 = 0;
				SQLLEN cbParm1 = SQL_NTS;
				SQLINTEGER iReturn = 0;
				RETCODE retcode = 0;

				std::cout << "Username: ";
				std::wcin >> wstrUsername;
				std::cout << "Password: ";
				std::wcin >> wstrPassword;
				std::cout << "Retype Password: ";
				std::wcin >> wstrRetypePassword;
				if (wstrPassword != wstrRetypePassword) {
					std::cout << "Retyped password doesn't match with the first one." << std::endl;
					break;
				}
				else {
					wcscpy(username, wstrUsername.c_str());
					wcscpy(password, wstrPassword.c_str());
					std::cout << "Tried to Deleting... " << std::endl;
					SQLBindParameter(hSTMT, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &username, wstrUsername.length(), &lUsername);
					SQLBindParameter(hSTMT, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 16, 0, &password, wstrPassword.length(), &lPassword);

					retcode = SQLExecDirect(hSTMT, (SQLTCHAR *)_T("{CALL usp_DeleteAccount (?, ?)}"), SQL_NTS);
					if ((retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO)) {
						std::cout << "Failed... " << std::endl;
						break;
					}
					//if (sParm1 != 0) {
					//	break;
					//}
					//else {
					//	std::cout << "Deleted." << std::endl;
					//}
				}

				break;
			}
			default: {
				break;
			}
		}
		if (hSTMT) SQLCloseCursor(hSTMT);
	}

	return 0;
}

int PPSQLServer::Release()
{
	if (hSTMT) SQLCloseCursor(hSTMT);
	if (hSTMT) SQLFreeHandle(SQL_HANDLE_STMT, hSTMT);
	if (hDBC) SQLDisconnect(hDBC);
	if (hSTMT) SQLFreeHandle(SQL_HANDLE_DBC, hDBC);
	if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	return 0;
}

int PPSQLServer::SelectAll()
{
	return 0;
}

int PPSQLServer::DisplayError(const char * fn, SQLHANDLE handle, SQLSMALLINT handletype)
{
	SQLWCHAR sqlstate[1024];
	SQLWCHAR message[1024];

	if (SQL_SUCCESS == SQLGetDiagRec(handletype, handle, 1, sqlstate, NULL, message, 1024, NULL)) {
		std::cout << "Message: ";
		std::wcout << message;
		std::cout << " nSQLSTATE: ";
		std::wcout << sqlstate;
		std::cout << std::endl;
	}
	return 0;
}
