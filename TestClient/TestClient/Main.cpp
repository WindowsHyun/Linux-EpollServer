#include "Main.h"



int main() {
	SOCKET socket = INVALID_SOCKET;
	init_sock(socket);

	for (int i = 0; i < 5; ++i) {
		cs_packet_dir sendPacket;
		sendPacket.packet_len = sizeof(sendPacket);
		sendPacket.packet_type = CLIENT_DIR - i;
		sendPacket.dir.x = 930616 + i;
		sendPacket.dir.y = 123456 - i;

		int retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
		std::cout << sizeof(sendPacket) << std::endl;
	}
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
