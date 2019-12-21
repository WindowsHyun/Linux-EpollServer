#include "Main.h"

bool IOCP_Server::initServer()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet){
		printf("[Error] WSAStartup()함수 실패 : %d\n", WSAGetLastError());
		return false;
	}

	//연결지향형 TCP , Overlapped I/O 소켓을 생성
	g_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == g_socket){
		printf("[Error] socket()함수 실패 : %d\n", WSAGetLastError());
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
