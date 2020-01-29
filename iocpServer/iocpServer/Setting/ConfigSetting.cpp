#include "ConfigSetting.h"

void ConfigSetting::loadSettingData()
{
	char *buf = NULL;
	buf = (char *)malloc(sizeof(char) * MAX_SOCKBUF);
	memset(buf, 0x00, sizeof(buf));

	// SERVER_PORT
	GetPrivateProfileString("Common", "SERVER_PORT", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("Common", "SERVER_PORT", "9001", "./iocpserver.ini");
		GetPrivateProfileString("Common", "SERVER_PORT", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_server_port(atoi(buf));

	// MAX_PLAYER
	GetPrivateProfileString("Common", "MAX_PLAYER", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("Common", "MAX_PLAYER", "10", "./iocpserver.ini");
		GetPrivateProfileString("Common", "MAX_PLAYER", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_max_player(atoi(buf));

	// LIMIT_ERROR_CNT
	GetPrivateProfileString("Common", "LIMIT_ERROR_CNT", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("Common", "LIMIT_ERROR_CNT", "5", "./iocpserver.ini");
		GetPrivateProfileString("Common", "LIMIT_ERROR_CNT", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_limit_err_cnt(atoi(buf));


	// DB Default Setting
	// REDIS_IP
	GetPrivateProfileString("DB", "REDIS_IP", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "REDIS_IP", "192.168.56.43", "./iocpserver.ini");
		GetPrivateProfileString("DB", "REDIS_IP", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_redis_id(buf, strlen(buf));

	// REDIS_PORT
	GetPrivateProfileString("DB", "REDIS_IP", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "REDIS_IP", "192.168.56.43", "./iocpserver.ini");
		GetPrivateProfileString("DB", "REDIS_IP", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_redis_id(buf, strlen(buf));

	// REDIS_PW
	GetPrivateProfileString("DB", "REDIS_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		// echo "windowshyun" | sha256sum
		WritePrivateProfileString("DB", "REDIS_PW", "3235e85a87a00eed432ee7512950abccd085c805d5825c4c17cdc65ad3835867", "./iocpserver.ini");
		GetPrivateProfileString("DB", "REDIS_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_redis_pw(buf, strlen(buf));

	// SQL_ID
	GetPrivateProfileString("DB", "SQL_ID", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "SQL_ID", "root", "./iocpserver.ini");
		GetPrivateProfileString("DB", "SQL_ID", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_sql_id(buf, strlen(buf));

	// SQL_PW
	GetPrivateProfileString("DB", "SQL_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "SQL_PW", "windowshyun", "./iocpserver.ini");
		GetPrivateProfileString("DB", "SQL_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_sql_pw(buf, strlen(buf));

	delete[] buf;
	spdlog::info("Server Setting Load Complete..!");
}
