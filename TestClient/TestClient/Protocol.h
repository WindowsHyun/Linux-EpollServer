#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "Object.h"

#define SERVERPORT 9001
#define UNIQUE_START_NO 1000	// ������ȣ ����
#define MAX_SOCKBUF	 1024		// �ִ� ��Ŷ ������
#define MAX_PLAYER 10			// �ִ� ���� �ο�
#define PACKET_HEADER_BYTE 4	// Packet Header ũ��

// �������� Ÿ��
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

// 8byte | Packet ���۽� ���� �� �κп� �ִ´�.
struct PACKET_HEADER {
	unsigned short packet_len;
	unsigned short packet_type;
};

// Ÿ�̸� Ÿ��
enum TimerType {
	T_NormalTime,
};

// �� Ŭ�� -> ���� ��Ŷ
struct cs_packet_dir : public PACKET_HEADER {
	Location dir;
};

struct cs_packet_test : public PACKET_HEADER {
	int tp;
	int cp;
	int xp;
};


// �� ���� -> Ŭ�� ��Ŷ
struct sc_packet_clientno : public PACKET_HEADER {
	int no;
};

struct sc_packet_dir : public PACKET_HEADER {
	Location dir;
};

#endif