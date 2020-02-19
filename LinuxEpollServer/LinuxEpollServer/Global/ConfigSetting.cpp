#include "ConfigSetting.h"

void ConfigSetting::loadSettingData()
{

	INIReader reader("SettingConfig.ini");

	if (reader.ParseError() < 0) {
		spdlog::error("Not Found 'SettingConfig.ini' File Default Load..!");
		//exit(1);
	}

	// SERVER_PORT
	this->set_server_port(reader.GetInteger("Common","SERVER_PORT", 9001));

	// MAX_PLAYER
	this->set_max_player(reader.GetInteger("Common", "MAX_PLAYER", 10));

	// LIMIT_ERROR_CNT
	this->set_limit_err_cnt(reader.GetInteger("Common", "LIMIT_ERROR_CNT", 10));


	// DB Default Setting
	// REDIS_IP
	this->set_redis_ip(reader.Get("REDIS_DB", "REDIS_IP", "127.0.0.1").c_str(), strlen(reader.Get("REDIS_DB", "REDIS_IP", "127.0.0.1").c_str()));

	// REDIS_PW
	// echo "windowshyun" | sha256sum | 3235e85a87a00eed432ee7512950abccd085c805d5825c4c17cdc65ad3835867
	const string db_pw = "3235e85a87a00eed432ee7512950abccd085c805d5825c4c17cdc65ad3835867";
	this->set_redis_pw(reader.Get("REDIS_DB", "REDIS_PW", db_pw).c_str(), strlen(reader.Get("REDIS_DB", "REDIS_PW", db_pw).c_str()));

	// SQL_HOST
	this->set_sql_host(reader.Get("MYSQL_DB", "SQL_HOST", "127.0.0.1").c_str(), strlen(reader.Get("MYSQL_DB", "SQL_HOST", "127.0.0.1").c_str()));

	// SQL_ID
	this->set_sql_id(reader.Get("MYSQL_DB", "SQL_ID", "root").c_str(), strlen(reader.Get("MYSQL_DB", "SQL_ID", "root").c_str()));

	// SQL_PW
	this->set_sql_pw(reader.Get("MYSQL_DB", "SQL_PW", "windowshyun").c_str(), strlen(reader.Get("MYSQL_DB", "SQL_PW", "windowshyun").c_str()));

	// SQL_DB
	this->set_sql_db(reader.Get("MYSQL_DB", "SQL_DB", "GameServer").c_str(), strlen(reader.Get("MYSQL_DB", "SQL_DB", "GameServer").c_str()));

	spdlog::info("Server Setting Load Complete..!");
}
