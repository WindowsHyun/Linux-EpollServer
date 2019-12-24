#include "Main.h"

IOCP_Server iocp_server;
std::list<PLAYER *> player;

int main() {
	std::cout << "Client Start Packet No : " << CLIENT_BASE << std::endl;
	std::cout << "Max Client Packet No : " << MAX_CLIENT_PROTOCOL_NO << std::endl;
	std::cout << "Server Start Packet No : " << SERVER_BASE << std::endl;
	std::cout << "Max Server Packet No : " << MAX_SERVER_PROTOCOL_NO << std::endl;

	iocp_server.initServer();				// Socket 생성
	iocp_server.BindandListen(SERVERPORT);	// Listen 처리
	iocp_server.initClient(player);			// Player Init
	iocp_server.StartServer();				// Server 시작

	//iocp_server.destroyThread();
	for (auto& client : player) {
		//std::cout << client->get_unique_id() << std::endl;
	}
}