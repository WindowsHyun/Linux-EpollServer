#include "Main.h"

void IOCP_Server::initServer()
{
}

void IOCP_Server::err_quit(char * msg)
{
}

void IOCP_Server::err_display(char * msg, int err_no)
{
}

IOCP_Server::IOCP_Server()
{
	std::wcout.imbue(std::locale("korean"));	// Locale Korean
	initServer();								// initServer
}

IOCP_Server::~IOCP_Server()
{
}
