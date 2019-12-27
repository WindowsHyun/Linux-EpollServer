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

#include "Protocol.h"
#include "Object.h"

const char ipAddres[MAX_SOCKBUF]{ "127.0.0.1" };

void init_sock(SOCKET& sock);
#endif