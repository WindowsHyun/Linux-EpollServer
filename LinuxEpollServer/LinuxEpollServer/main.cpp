#include "Main.h"

class ConfigSetting CS;
class Logic_API api;
class Epoll_Server epoll_server;
std::vector<class RedisConnect *> RDC;
std::unordered_map<int, class PLAYER *> player;
std::unordered_map<int, class PLAYER_Session *> player_session;

int main()
{
#if DEBUG
	// daily Logger Start 
	// Debug ��忡���� �ֿܼ� ��¸�, ������ ���� �ʴ´�.
	auto daily_logger = spdlog::daily_logger_mt("daily_logger", "logs/iocpServer.txt", 0, 0);	// 00:00 new log
	daily_logger->flush_on(spdlog::level::debug);
	spdlog::set_default_logger(daily_logger);
#endif
	// Server Packet Info
	spdlog::info("Client Start Packet No : {}", CLIENT_BASE);
	spdlog::info("Max Client Packet No   : {}", MAX_CLIENT_PROTOCOL_NO);
	spdlog::info("Server Start Packet No : {}", SERVER_BASE);
	spdlog::info("Max Server Packet No   : {}\n", MAX_SERVER_PROTOCOL_NO);

	// Start Server
	CS.loadSettingData();												// Load Server Config
	initRDC();															// RedisClinet ����
	epoll_server.init_server();											// Server init
	epoll_server.BindandListen(CS.get_server_port());					// Server BindListen
	api.start();														// API Thread init

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

	// ���� ������ȣ ���� �κ� DB�� ��������
	std::string value;
	RDC[REDISDB::REDIS_USER_AUTH_DB]->get("UNIQUE_NO", value);
	if (value == "") {
		// UNIQUE_NO�� ���� ��� ���� �ʱ� ���� �� ���� ������ �Ѵ�.
		RDC[REDISDB::REDIS_USER_AUTH_DB]->set("UNIQUE_NO", to_string(UNIQUE_START_NO));
		CS.set_unique_no(atoi(to_string(UNIQUE_START_NO).c_str()));
	}
	else {
		CS.set_unique_no(atoi(value.c_str()));
	}
	spdlog::info("User Unique Start No   : {}", CS.get_unique_no());
}
