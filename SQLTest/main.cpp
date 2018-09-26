#include "PPSQLServer.h"

int main(int argc, char* argv[]) {
	PPSQLServer SQLServer;

	std::setlocale(LC_ALL, "");

	SQLServer.Init();
	SQLServer.Run();
	SQLServer.Release();

	return 0;
}