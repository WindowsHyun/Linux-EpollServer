#include "Main.h"

bool IOCP_Server::initServer()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet){
		printf("[Error] WSAStartup()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	//���������� TCP , Overlapped I/O ������ ����
	g_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == g_socket){
		printf("[Error] socket()�Լ� ���� : %d\n", WSAGetLastError());
		return false;
	}

	printf("Socket Init Success..!\n");
	return true;

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
	initServer();													// initServer
}

IOCP_Server::~IOCP_Server()
{
}
