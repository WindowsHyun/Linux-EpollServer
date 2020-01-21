#ifndef __SESSION_H__
#define __SESSION_H__

#include "Main.h"
#include "ReadBuffer.h"

enum class IOOperation {
	RECV,
	SEND,
	IO_EVENT,
};

// WSAOVERLAPPED구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct stOverlappedEx {
	WSAOVERLAPPED			m_wsaOverlapped;			// Overlapped I/O구조체
	SOCKET					m_socketSession;			// 클라이언트 소켓
	WSABUF					m_wsaBuf;					// Overlapped I/O작업 버퍼
	IOOperation				m_eOperation;				// 작업 동작 종류
	unsigned __int64		m_unique_id;				// 유저 고유 번호

	stOverlappedEx() {
		memset(&m_wsaOverlapped, 0, sizeof(OVERLAPPED));
		memset(&m_wsaBuf, 0, sizeof(OVERLAPPED));
		m_socketSession = INVALID_SOCKET;
		m_unique_id = 0;
	}
};

class PLAYER_Session {
public:
	PLAYER_Session() {
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_readBuffer.init(MAX_SOCKBUF);
		m_socketSession = INVALID_SOCKET;
		unique_id = 0;
		error_cnt = 0;
	}
	// get
	SOCKET& get_sock() { return m_socketSession; }
	ReadBuffer& get_buffer() { return m_readBuffer; }
	unsigned __int64 get_unique_id() { return unique_id; }
	stOverlappedEx& get_Recv_over() { return m_stRecvOverlappedEx; }
	stOverlappedEx& get_Send_over() { return m_stSendOverlappedEx; }
	int get_error_cnt() { return error_cnt; }

	// set
	void set_unique_id(const unsigned __int64 id);
	void set_init_session();
	void set_error_cnt();

private:
	SOCKET			m_socketSession;		// Cliet와 연결되는 소켓
	ReadBuffer		m_readBuffer;			// readBuffet
	stOverlappedEx	m_stRecvOverlappedEx;	// RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;	// SEND Overlapped I/O작업을 위한 변수
	unsigned __int64 unique_id;				// 고유 아이디
	int error_cnt;							// 패킷 오류 Count
};
#endif