#ifndef __MAIN_H__
#define __MAIN_H__

#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS					// 윈속 오류 방지
#define MAX_CONNECT 40									// 최대 접속 인원
#define MAX_WORKERTHREAD 9								// 쓰레드 풀에 넣을 쓰레드 수
#define DEBUG_MODE	TRUE								// printf 출력 여부

// Header 선언
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <Windows.h>
#include <time.h>
#include <ctime>
#include <list>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <queue>
#include <mutex>


// 헤더 가져오기
#include "Protocol.h"
#include "Iocp.h"
#include "Object.h"
#include "Session.h"
#include "Timer.h"

extern IOCP_Server iocp_server;
extern std::unordered_map<unsigned __int64, PLAYER *> player;
extern SERVER_Timer timer;
#endif