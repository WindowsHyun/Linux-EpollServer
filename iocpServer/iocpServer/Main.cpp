#include "Main.h"

class IOCP_Server iocp_server;
class SERVER_Timer timer;
class Logic_API api;
int packet_cnt;
std::queue<Packet_Frame> recvPacketQueue;
std::unordered_map<unsigned __int64, class PLAYER *> player;
std::unordered_map<unsigned __int64, class PLAYER_Session *> player_session;

int main() {
	// daily Logger Start
	//auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/daily.txt", 0, 0);	// 00:00 new log
	//spdlog::set_default_logger(daily_logger);

	packet_cnt = 0;
	spdlog::info("Client Start Packet No : {}", CLIENT_BASE);
	spdlog::info("Max Client Packet No   : {}", MAX_CLIENT_PROTOCOL_NO);
	spdlog::info("Server Start Packet No : {}", SERVER_BASE);
	spdlog::info("Max Server Packet No   : {}\n", MAX_SERVER_PROTOCOL_NO);

	iocp_server.initServer();					// Socket 생성
	iocp_server.BindandListen(SERVERPORT);		// Listen 처리
	iocp_server.initClient();					// Player Init
	iocp_server.StartServer();					// Server 시작
	api.start();								// API처리 시작
	timer.start(iocp_server.getHandle());		// Timer 시작

	

	getchar();
	return 0;
}