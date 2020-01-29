#include "Main.h"

class IOCP_Server iocp_server;
class SERVER_Timer timer;
class Logic_API api;
class ConfigSetting CS;
std::queue<Packet_Frame> recvPacketQueue;
std::unordered_map<unsigned __int64, class PLAYER *> player;
std::unordered_map<unsigned __int64, class PLAYER_Session *> player_session;

int main() {
	// daily Logger Start
#if DEBUG
	// Debug 모드에서는 콘솔에 출력만, 저장은 하지 않는다.
	auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/iocpServer.txt", 0, 0);	// 00:00 new log
	daily_logger->flush_on(spdlog::level::debug);
	spdlog::set_default_logger(daily_logger);
#endif

	RedisConnect *redis = new RedisConnect;
	redis->Setup("192.168.56.43", 6380, "3235e85a87a00eed432ee7512950abccd085c805d5825c4c17cdc65ad3835867");
	redis->connect("192.168.56.43", 6380, 3);
	redis->auth("3235e85a87a00eed432ee7512950abccd085c805d5825c4c17cdc65ad3835867");
	//shared_ptr<RedisConnect> redis = RedisConnect::Instance();
	redis->ping();
	redis->set("1993", "06");
	//redis->set("key", "val");


	// Server Packet Info
	spdlog::info("Client Start Packet No : {}", CLIENT_BASE);
	spdlog::info("Max Client Packet No   : {}", MAX_CLIENT_PROTOCOL_NO);
	spdlog::info("Server Start Packet No : {}", SERVER_BASE);
	spdlog::info("Max Server Packet No   : {}\n", MAX_SERVER_PROTOCOL_NO);
	
	// Server Start
	CS.loadSettingData();									// 서버설정 불러오기
	iocp_server.initServer();								// Socket 생성
	iocp_server.BindandListen(CS.get_server_port());		// Listen 처리
	iocp_server.initClient();								// Player Init
	iocp_server.StartServer();								// Server 시작
	api.start();											// API처리 시작
	timer.start(iocp_server.getHandle());					// Timer 시작

	

	getchar();
	return 0;
}