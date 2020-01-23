#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Object.h"

#define UNIQUE_START_NO 1000	// 고유번호 시작
#define MAX_SOCKBUF	 4096		// 최대 패킷 사이즈
#define MAX_WORKERTHREAD 9		// 쓰레드 풀에 넣을 쓰레드 수
#define PACKET_HEADER_BYTE 4	// Packet Header 크기


// 프로토콜 타입
const enum ProtocolType {
	None = 0,

	PACKET_PROTOCOL_BASE = 1000,
	PACKET_RANG_SIZE = 100,

	// Client to Server
	CLIENT_BASE = PACKET_PROTOCOL_BASE,
	CLIENT_AUTH_BASE = CLIENT_BASE + PACKET_RANG_SIZE,
	CLIENT_FRONT_BASE = CLIENT_AUTH_BASE + PACKET_RANG_SIZE,
	CLIENT_GOODS_BASE = CLIENT_FRONT_BASE + PACKET_RANG_SIZE,
	CLIENT_INFO_BASE = CLIENT_GOODS_BASE + PACKET_RANG_SIZE,

	// Auth
	CLIENT_AUTH = CLIENT_AUTH_BASE,
	CLIENT_AUTH_LOGIN,
	CLIENT_AUTH_TEST,
	CLIENT_AUTH_TEST2,
	CLIENT_AUTH_TEST3,
	CLIENT_AUTH_TEST4,

	// Front
	CLIENT_FRONT = CLIENT_FRONT_BASE,
	CLIENT_FRONT_ACCOUNT,
	CLIENT_FRONT_TEST,

	// Goods
	CLIENT_GOODS = CLIENT_GOODS_BASE,
	CLIENT_GOODS_INFO,
	CLIENT_GOODS_TEST,

	// Info
	CLIENT_INFO = CLIENT_INFO_BASE,
	CLIENT_INFO_DATA,
	CLIENT_INFO_TEST,

	// Max Protocol No (Client)
	MAX_CLIENT_PROTOCOL_NO,


	// Server to Client
	SERVER_BASE = ((MAX_CLIENT_PROTOCOL_NO / PACKET_RANG_SIZE) * PACKET_RANG_SIZE) + PACKET_RANG_SIZE,	// Client 마지막 번호 에서 부터 시작한다.
	SERVER_AUTH_BASE = SERVER_BASE + PACKET_RANG_SIZE,
	SERVER_FRONT_BASE = SERVER_AUTH_BASE + PACKET_RANG_SIZE,
	SERVER_GOODS_BASE = SERVER_FRONT_BASE + PACKET_RANG_SIZE,
	SERVER_INFO_BASE = SERVER_GOODS_BASE + PACKET_RANG_SIZE,

	// Auth
	SERVER_AUTH = SERVER_AUTH_BASE,
	SERVER_CLIENT_NO,

	// Front
	SERVER_FRONT = SERVER_FRONT_BASE,
	SERVER_FRONT_ACCOUNT,

	// Goods
	SERVER_GOODS = SERVER_GOODS_BASE,
	SERVER_GOODS_INFO,

	// Info
	SERVER_INFO = SERVER_INFO_BASE,
	SERVER_INFO_TEST,

	// Max Protocol No (Server)
	MAX_SERVER_PROTOCOL_NO,
};

// 8byte | Packet 전송시 제일 앞 부분에 넣는다.
struct PACKET_HEADER {
	unsigned short packet_len;
	unsigned short packet_type;
};

struct Packet_Frame {
	unsigned short packet_type = -1; // NONE
	unsigned short size = 0;
	unsigned __int64 unique_id = 0;
	char* pData = nullptr;
};

// 타이머 타입
enum TimerType {
	T_NormalTime,
};

// ↓ 클라 -> 서버 패킷
struct cs_packet_dir : public PACKET_HEADER {
	Location dir;
};

struct cs_packet_test : public PACKET_HEADER {
	int tp;
	int cp;
	int xp;
};


// ↓ 서버 -> 클라 패킷
struct sc_packet_clientno : public PACKET_HEADER {
	unsigned __int64 no;
};

struct sc_packet_dir : public PACKET_HEADER {
	Location dir;
};

#endif