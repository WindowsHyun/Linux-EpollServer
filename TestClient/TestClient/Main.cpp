#include "Main.h"



int main() {
	SOCKET socket = INVALID_SOCKET;
	init_sock(socket);
	//char *buf = new char[MAX_SOCKBUF];
	int totalPacket_cnt = 0;
	char buf[2048];

	//CLIENT_AUTH_LOGIN
	cs_packet_auth sendPacket;
	sendPacket.packet_len = sizeof(sendPacket);
	sendPacket.packet_type = CLIENT_AUTH_LOGIN;
	strcpy(sendPacket.sha256sum, "0998deee89b70c6b4e68a15a731bfc86bb1707d32d9825035819d8a338172bcf");
	//sendPacket.sha256sum = "0998deee89b70c6b4e68a15a731bfc86bb1707d32d9825035819d8a338172bca";
	int retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
	totalPacket_cnt++;
	Sleep(100);


	//retval = recvn(socket, buf, sizeof(int), 0);
	//if (retval == SOCKET_ERROR) {
	//	std::cout << "retval Error : " << retval << std::endl;
	//	//break;
	//}

	//printf("");
	for (int i = 0; i < 50; ++i) {
		cs_packet_dir sendPacket2;
		sendPacket2.packet_len = sizeof(sendPacket2);
		sendPacket2.packet_type = CLIENT_AUTH_TEST;
		sendPacket2.dir.x = 1;
		sendPacket2.dir.y = 2;
		retval = send(socket, reinterpret_cast<const char *>(&sendPacket2), sizeof(sendPacket2), 0);
		totalPacket_cnt++;
		//std::cout << "." << std::endl;
	}
	/*for (int i = 0; i < 5; ++i) {
		
	}*/
	//for (int i = 0; i < 2500; ++i) {
	//
	//	totalPacket_cnt++;
	//	Sleep(1);
	//	//std::cout << sizeof(sendPacket) << std::endl;
	//	cs_packet_dir sendPacket2;
	//	sendPacket2.packet_len = sizeof(sendPacket2);
	//	sendPacket2.packet_type = CLIENT_AUTH_TEST2;
	//	sendPacket2.dir.x = 2;
	//	sendPacket2.dir.y = totalPacket_cnt;

	//	retval = send(socket, reinterpret_cast<const char *>(&sendPacket2), sizeof(sendPacket2), 0);
	//	totalPacket_cnt++;
	//	Sleep(1);
	//	//std::cout << sizeof(sendPacket) << std::endl;

	//	sendPacket2.packet_len = sizeof(sendPacket2);
	//	sendPacket2.packet_type = CLIENT_AUTH_TEST3;
	//	sendPacket2.dir.x = 3;
	//	sendPacket2.dir.y = totalPacket_cnt;

	//	retval = send(socket, reinterpret_cast<const char *>(&sendPacket2), sizeof(sendPacket2), 0);
	//	totalPacket_cnt++;
	//	Sleep(1);
	//	//std::cout << sizeof(sendPacket) << std::endl;

	//	sendPacket2.packet_len = sizeof(sendPacket2);
	//	sendPacket2.packet_type = CLIENT_AUTH_TEST4;
	//	sendPacket2.dir.x = 4;
	//	sendPacket2.dir.y = totalPacket_cnt;

	//	retval = send(socket, reinterpret_cast<const char *>(&sendPacket2), sizeof(sendPacket2), 0);
	//	totalPacket_cnt++;
	//	Sleep(1);
	//	//std::cout << sizeof(sendPacket) << std::endl;

	//	/*sendPacket.packet_len = sizeof(sendPacket);
	//	sendPacket.packet_type = CLIENT_AUTH_TEST4 + 1;
	//	sendPacket.dir.x = 4;
	//	sendPacket.dir.y = 123456;

	//	retval = send(socket, reinterpret_cast<const char *>(&sendPacket), sizeof(sendPacket), 0);
	//	std::cout << sizeof(sendPacket) << std::endl;*/
	//	
	//	
	//}
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
	if (Result == SOCKET_ERROR) {
		std::cout << "연결 실패..!" << std::endl;
		getchar();
		exit(1);
	}
	

}

int recvn(SOCKET s, char *buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}