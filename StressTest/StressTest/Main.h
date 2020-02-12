#ifndef __Main_H__
#define __Main_H__

#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

// SPDLog 1.5.0 <2020.01.21> github Include Add
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"

// header 불러오기
#include "ReadBuffer.h"
#include "iocpClient.h"
#include "Protocol.h"
#include "Object.h"
#include "Library/Api.h"

#define SERVERPORT 9001

const char ipAddres[MAX_SOCKBUF]{ "192.168.56.43" };
extern std::vector<class IOCP_Client *> vIocpClient;
extern class Logic_API api;
extern class PLAYER *Player;
#endif