#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Object.h"

#define SERVERPORT 9000
#define UNIQUE_START_NO 1000	// 고유번호 시작
#define MAX_SOCKBUF	 1024		// 최대 패킷 사이즈
#define MAX_PLAYER 10			// 최대 접속 인원

// 프로토콜 타입
const enum ProtocolType {
	None = 0,

	PACKET_PROTOCOL_BASE = 1000,
	PACKET_RANG_SIZE = 1000,

	CLIENT_BASE = PACKET_PROTOCOL_BASE,				// Client to Server
	CLIENT_DIR,

	MAX_CLIENT_PROTOCOL_NO,

	SERVER_BASE = CLIENT_BASE + PACKET_RANG_SIZE,	// Server to Client
	SERVER_CLIENT_NO,

	MAX_SERVER_PROTOCOL_NO,
};

// 타이머 타입
enum TimerType {
	T_NormalTime,
};

// ↓ 클라 -> 서버 패킷
struct cs_packet_dir {
	int size;
	int type;
	//----------------
	Location dir;
};


// ↓ 서버 -> 클라 패킷
struct sc_packet_clientno {
	int size;
	int type;
	//----------------
	int no;
};

#endif