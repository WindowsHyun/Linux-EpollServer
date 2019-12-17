#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Object.h"

#define SERVERPORT 9000

// 기본 구조체 [서버에서는 제거]

#define VK_UP_UP 100
#define VK_DOWN_UP 101
#define VK_UP_DOWN 200
#define VK_DOWN_DOWN 201
#define VK_UP_LEFT 300
#define VK_DOWN_LEFT 301
#define VK_UP_RIGHT 400
#define VK_DOWN_RIGHT 401
#define VK_UP_SPACE 500
#define VK_DOWN_SPACE 501

// 프로토콜 타입
const enum ProtocolType {
	None = 0,

	PACKET_PROTOCOL_BASE = 1000,
	PACKET_RANG_SIZE = 1000,
	
	CLIENT_BASE = PACKET_PROTOCOL_BASE,
	CLIENT_DIR,

	SERVER_BASE = CLIENT_BASE + PACKET_RANG_SIZE,
	SERVER_CLIENT_NO,

	MAX_PROTOCOL_NO,
};


// ↓ 클라 -> 서버 패킷
struct cs_packet_dir {
	int type;
	//----------------
	Location dir;
};


// ↓ 서버 -> 클라 패킷
struct sc_packet_clientno {
	int type;
	int byte;
	//----------------
	int no;
};

#endif