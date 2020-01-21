#include "Main.h"



int main() {
	SOCKET socket = INVALID_SOCKET;
	init_sock(socket);
	int totalPacket_cnt = 0;

	/*for (int i = 0; i < 5; ++i) {
		
	}*/
	for (int i = 0; i < 2500; ++i) {
		cs_packet_dir sendPacket;
		sendPacket.packet_len = sizeof(sendPacket);
		sendPacket.packet_type = CLIENT_AUTH_TEST;
		sendPacket.dir.x = 1;
		sendPacket.dir.y = totalPacket_cnt;

		int retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
		totalPacket_cnt++;
		Sleep(1);
		//std::cout << sizeof(sendPacket) << std::endl;

		sendPacket.packet_len = sizeof(sendPacket);
		sendPacket.packet_type = CLIENT_AUTH_TEST2;
		sendPacket.dir.x = 2;
		sendPacket.dir.y = totalPacket_cnt;

		retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
		totalPacket_cnt++;
		Sleep(1);
		//std::cout << sizeof(sendPacket) << std::endl;

		sendPacket.packet_len = sizeof(sendPacket);
		sendPacket.packet_type = CLIENT_AUTH_TEST3;
		sendPacket.dir.x = 3;
		sendPacket.dir.y = totalPacket_cnt;

		retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
		totalPacket_cnt++;
		Sleep(1);
		//std::cout << sizeof(sendPacket) << std::endl;

		sendPacket.packet_len = sizeof(sendPacket);
		sendPacket.packet_type = CLIENT_AUTH_TEST4;
		sendPacket.dir.x = 4;
		sendPacket.dir.y = totalPacket_cnt;

		retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
		totalPacket_cnt++;
		Sleep(1);
		//std::cout << sizeof(sendPacket) << std::endl;

		/*sendPacket.packet_len = sizeof(sendPacket);
		sendPacket.packet_type = CLIENT_AUTH_TEST4 + 1;
		sendPacket.dir.x = 4;
		sendPacket.dir.y = 123456;

		retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
		std::cout << sizeof(sendPacket) << std::endl;*/
		
		
	}
	std::cout << "[INFO] Total Packet Count : " << totalPacket_cnt << std::endl;
	exit(1);
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
