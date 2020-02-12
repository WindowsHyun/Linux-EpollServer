#include "Main.h"

#define MAX_CONNECT 1000

std::vector<class IOCP_Client *> vIocpClient;
class Logic_API api;
class PLAYER *Player;

int main() {

	// Create
	vIocpClient.clear();
	for (int i = 0; i < MAX_CONNECT; ++i) {
		class IOCP_Client* temp_client = new class IOCP_Client;
		temp_client->initClient();
		temp_client->connectServer(ipAddres);

		vIocpClient.push_back(temp_client);
	}


	// Login
	int k = 0;
	for (auto iocp : vIocpClient) {
		cs_packet_auth sendPacket;
		sendPacket.packet_len = sizeof(sendPacket);
		sendPacket.packet_type = CLIENT_AUTH_LOGIN;
		char c_int[10];
		_itoa(k, c_int, 10);
		strcpy(sendPacket.sha256sum, c_int);
		vIocpClient[k]->SendPacket(reinterpret_cast<char *>(&sendPacket), sizeof(sendPacket));
		++k;
	}

	Sleep(2048);

	for (int i = 0; i < 100; ++i) {
		// Packet Send
		k = 0;
		for (auto iocp : vIocpClient) {
			cs_packet_dir sendPacket2;
			sendPacket2.packet_len = sizeof(sendPacket2);
			sendPacket2.packet_type = CLIENT_AUTH_TEST;
			sendPacket2.dir.x = 06;
			sendPacket2.dir.y = 16;
			vIocpClient[k]->SendPacket(reinterpret_cast<char *>(&sendPacket2), sizeof(sendPacket2));
			++k;
		}
		Sleep(10);
	}


	getchar();
}
