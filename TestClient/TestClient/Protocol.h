#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Object.h"

#define SERVERPORT 9001
#define UNIQUE_START_NO 1000	// 고유번호 시작
#define MAX_SOCKBUF	 1024		// 최대 패킷 사이즈
#define MAX_PLAYER 10			// 최대 접속 인원
#define PACKET_HEADER_BYTE 4	// Packet Header 크기

// 프로토콜 타입
const enum ProtocolType {
	None = 0,

	PACKET_PROTOCOL_BASE = 1000,
	PACKET_RANG_SIZE = 1000,

	// Client to Server
	CLIENT_BASE = PACKET_PROTOCOL_BASE,
	CLIENT_DIR,
	CLIENT_TEST,

	MAX_CLIENT_PROTOCOL_NO,

	// Server to Client
	SERVER_BASE = CLIENT_BASE + PACKET_RANG_SIZE,
	SERVER_CLIENT_NO,
	SERVER_CLIENT_DIR,

	MAX_SERVER_PROTOCOL_NO,
};

// 8byte | Packet 전송시 제일 앞 부분에 넣는다.
struct PACKET_HEADER {
	unsigned short packet_len;
	unsigned short packet_type;
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
	int no;
};

struct sc_packet_dir : public PACKET_HEADER {
	Location dir;
};

#endif