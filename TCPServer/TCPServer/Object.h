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
	Location position;	// 총알의 위치
	int type;			// 어떤 객체의 총알인가
	int dir;			// 어떤 방향으로 갈지
	int bullet_type;	// 총알 그리기 타입
};

class PLAYER {
public:
	PLAYER() {
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&nickName, sizeof(char));
		sock = INVALID_SOCKET;
		unique_id = 1;
		connect = false;
		live = false;
		game_play = false;
	}
	// get
	SOCKET get_sock() { return sock; }
	unsigned __int64 get_unique_id() { return unique_id; }

	// set
	void set_sock(const SOCKET g_sock);
	void set_unique_id(const unsigned __int64 id);
	void set_init_player();

private:
	SOCKET sock;
	stOverlappedEx	m_stRecvOverlappedEx;		//RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;		//SEND Overlapped I/O작업을 위한 변수
	unsigned __int64 unique_id;				// 클라이언트 고유 번호
	bool connect;				// 클라이언트 연결 여부
	int hp;						// 클라이언트 체력
	bool live;					// 클라이언트 생존 여부
	bool game_play;				// 클라이언트 플레이 여부
	char nickName[16];			// 클라이언트 이름
	Location position;			// 클라이언트 위치
	std::list<ITEM> item;		// 클라이언트 아이템
	std::list<BULLET> bullet;	// 클라이언트 총알
};
#endif