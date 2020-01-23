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
	// REDIS_ID
	GetPrivateProfileString("DB", "REDIS_ID", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "REDIS_ID", "WindowsHyun", "./iocpserver.ini");
		GetPrivateProfileString("DB", "REDIS_ID", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_redis_id(buf, strlen(buf));

	// REDIS_PW
	GetPrivateProfileString("DB", "REDIS_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "REDIS_PW", "WindowsHyun", "./iocpserver.ini");
		GetPrivateProfileString("DB", "REDIS_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_redis_pw(buf, strlen(buf));

	// SQL_ID
	GetPrivateProfileString("DB", "SQL_ID", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "SQL_ID", "WindowsHyun", "./iocpserver.ini");
		GetPrivateProfileString("DB", "SQL_ID", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_sql_id(buf, strlen(buf));

	// SQL_PW
	GetPrivateProfileString("DB", "SQL_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	if (strcmp(buf, "NOT_FOUND") == 0) {
		// 설정 ini 파일이 없을 경우 처리
		WritePrivateProfileString("DB", "SQL_PW", "WindowsHyun", "./iocpserver.ini");
		GetPrivateProfileString("DB", "SQL_PW", "NOT_FOUND", buf, MAX_SOCKBUF, "./iocpserver.ini");
	}
	this->set_sql_pw(buf, strlen(buf));

	delete[] buf;
	spdlog::info("Server Setting Load Complete..!");
}
