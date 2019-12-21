#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "Iocp.h"

struct Location {
	int x;
	int y;
};

struct ITEM {
	int count;
	char name[20];
};

struct BULLET {
	Location position;	// �Ѿ��� ��ġ
	int type;			// � ��ü�� �Ѿ��ΰ�
	int dir;			// � �������� ����
	int bullet_type;	// �Ѿ� �׸��� Ÿ��
};

struct PLAYER {
	SOCKET sock;
	stOverlappedEx	m_stRecvOverlappedEx;		//RECV Overlapped I/O�۾��� ���� ����
	stOverlappedEx	m_stSendOverlappedEx;	//SEND Overlapped I/O�۾��� ���� ����
	int cliend_id = -1;			// Ŭ���̾�Ʈ ���� ��ȣ
	bool connect = false;		// Ŭ���̾�Ʈ ���� ����
	int hp;						// Ŭ���̾�Ʈ ü��
	bool live = true;			// Ŭ���̾�Ʈ ���� ����
	bool game_play = false;		// Ŭ���̾�Ʈ �÷��� ����
	char nickName[16];			// Ŭ���̾�Ʈ �̸�
	Location position;			// Ŭ���̾�Ʈ ��ġ
	std::list<ITEM> item;		// Ŭ���̾�Ʈ ������
	std::list<BULLET> bullet;	// Ŭ���̾�Ʈ �Ѿ�
	PLAYER() {
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		sock = INVALID_SOCKET;
	}
};

#endif