#ifndef __MAIN_H__
#define __MAIN_H__

#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <queue>
#include <pthread.h>

// SPDLog 1.5.0 <2020.01.21> github Include Add
#include "includes/spdlog/spdlog.h"
#include "includes/spdlog/sinks/daily_file_sink.h"

// header
#include "Protocol.h"
#include "Global/ConfigSetting.h"
#include "Global/ResultCode.h"
#include "Global/RedisConnect.h"
#include "Library/Api.h"
#include "ReadBuffer.h"
#include "Object.h"
#include "Session.h"
#include "EpollServer.h"

// Setting Value
extern class ConfigSetting CS;
extern std::vector<class RedisConnect *> RDC;
extern class Epoll_Server epoll_server;
extern class Logic_API api;
//extern class SERVER_Timer timer;
extern std::unordered_map<int, class PLAYER *> player;					// 플레이어 데이터
extern std::unordered_map<int, class PLAYER_Session *> player_session;	// 플레이어 세션

void initRDC();
#endif