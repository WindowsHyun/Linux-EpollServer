#include "Main.h"



int main() {
	SOCKET socket = INVALID_SOCKET;
	init_sock(socket);


	cs_packet_dir sendPacket;
	sendPacket.type = CLIENT_DIR;
	sendPacket.size = sizeof(sendPacket);
	sendPacket.dir.x = 10;
	sendPacket.dir.y = 10;

	int retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
	getchar();
}

void init_sock(SOCKET& sock)
{
	WSADATA	wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVERPORT);
	ServerAddr.sin_addr.s_addr = inet_addr(ipAddres);

	int Result = WSAConnect(sock, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);

}
