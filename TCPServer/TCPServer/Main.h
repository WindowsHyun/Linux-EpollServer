#ifndef __MAIN_H__
#define __MAIN_H__

#pragma comment(lib, "ws2_32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS	// ���� ���� ����
#define MAX_CONNECT 40											// �ִ� ���� �ο�
#define MAX_WORKERTHREAD 4								// ������ Ǯ�� ���� ������ ��
#define DEBUG_MODE	TRUE										// printf ��� ����

// Header ����
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


// ��� ��������
#include "Protocol.h"
#include "Iocp.h"
#include "Object.h"
#endif