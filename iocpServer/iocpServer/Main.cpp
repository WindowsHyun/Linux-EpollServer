#include "Main.h"

class IOCP_Server iocp_server;
class SERVER_Timer timer;
class Logic_API api;
class ConfigSetting CS;
std::vector<class RedisConnect *> RDC;
std::queue<Packet_Frame> recvPacketQueue;
std::unordered_map<unsigned __int64, class PLAYER *> player;
std::unordered_map<unsigned __int64, class PLAYER_Session *> player_session;

int main() {
#if DEBUG
	// daily Logger Start
	// Debug 모드에서는 콘솔에 출력만, 저장은 하지 않는다.
	auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/iocpServer.txt", 0, 0);	// 00:00 new log
	daily_logger->flush_on(spdlog::level::debug);
	spdlog::set_default_logger(daily_logger);
#endif

	// Server Packet Info
	spdlog::info("Client Start Packet No : {}", CLIENT_BASE);
	spdlog::info("Max Client Packet No   : {}", MAX_CLIENT_PROTOCOL_NO);
	spdlog::info("Server Start Packet No : {}", SERVER_BASE);
	spdlog::info("Max Server Packet No   : {}\n", MAX_SERVER_PROTOCOL_NO);
	
	// Server Start
	CS.loadSettingData();												// 서버설정 불러오기
	initRDC();															// RedisClinet 생성
	iocp_server.initServer();											// Socket 생성
	iocp_server.BindandListen(CS.get_server_port());					// Listen 처리
	iocp_server.initClient();											// Player Init
	iocp_server.StartServer();											// Server 시작
	api.start();														// API처리 시작
	timer.start(iocp_server.getHandle());								// Timer 시작

	getchar();
	return 0;
}



void initRDC()
{
	for (int i = 0; i < REDISDB::MAX_REDIS_DB_NUM; ++i) {
		RedisConnect * rdc = new RedisConnect;
		rdc->init(CS.get_redis_ip(), REDIS_MAIN_PORT, CS.get_redis_pw(), i);
		RDC.emplace_back(rdc);
	}

	// 유저 고유번호 시작 부분 DB로 가져오기
	std::string value;
	RDC[REDISDB::REDIS_USER_AUTH_DB]->get("UNIQUE_NO", value);
	if (value == "") {
		// UNIQUE_NO가 없을 경우 서버 초기 시작 값 부터 진행을 한다.
		RDC[REDISDB::REDIS_USER_AUTH_DB]->set("UNIQUE_NO", to_string(UNIQUE_START_NO));
		CS.set_unique_id_no(atoi(to_string(UNIQUE_START_NO).c_str()));
	}
	else {
		CS.set_unique_id_no(atoi(value.c_str()));
	}
	spdlog::info("User Unique Start No   : {}", CS.get_unique_id_no());
}
