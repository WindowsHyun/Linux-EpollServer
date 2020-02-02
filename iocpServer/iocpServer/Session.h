#ifndef __SESSION_H__
#define __SESSION_H__

#include "Main.h"
#include "ReadBuffer.h"

enum class IOOperation {
	RECV,
	SEND,
	IO_EVENT,
	DisconnectRemove
};

// WSAOVERLAPPED구조체를 확장 시켜서 필요한 정보를 더 넣었다.
struct stOverlappedEx {
	WSAOVERLAPPED			m_wsaOverlapped;			// Overlapped I/O구조체
	SOCKET					m_socketSession;			// 클라이언트 소켓
	WSABUF					m_wsaBuf;					// Overlapped I/O작업 버퍼
	IOOperation				m_eOperation;				// 작업 동작 종류
	unsigned __int64		m_unique_no;				// 유저 고유 번호

	stOverlappedEx() {
		memset(&m_wsaOverlapped, 0, sizeof(OVERLAPPED));
		memset(&m_wsaBuf, 0, sizeof(OVERLAPPED));
		m_socketSession = INVALID_SOCKET;
		m_unique_no = 0;
	}
};

class PLAYER_Session {
public:
	PLAYER_Session() {
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_readBuffer.init(MAX_SOCKBUF);
		m_sendBuffer.init(MAX_SOCKBUF);
		m_socketSession = INVALID_SOCKET;
		unique_no = 0;
		error_cnt = 0;
		remainSize = 0;
	}
	// get
	SOCKET& get_sock() { return m_socketSession; }
	ReadBuffer& read_buffer() { return m_readBuffer; }
	ReadBuffer& send_buffer() { return m_sendBuffer; }
	unsigned __int64 get_unique_no() { return unique_no; }
	stOverlappedEx& get_Recv_over() { return m_stRecvOverlappedEx; }
	stOverlappedEx& get_Send_over() { return m_stSendOverlappedEx; }
	int get_error_cnt() { return error_cnt; }
	int get_remainSize() { return remainSize; }

	// set
	void set_unique_no(const unsigned __int64 id);
	void set_init_session();
	void update_error_cnt();
	void set_remainSize(const int value){ remainSize = value;}
	void incr_remainSize(const int value) { remainSize += value; }
	bool sendReady(char* pMsg, int size);
	bool sendIo();
	bool sendFinish(int size);

private:
	SOCKET			m_socketSession;		// Cliet와 연결되는 소켓
	ReadBuffer		m_readBuffer;			// readBuffet
	ReadBuffer		m_sendBuffer;			// sendBuffet
	stOverlappedEx	m_stRecvOverlappedEx;	// RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;	// SEND Overlapped I/O작업을 위한 변수
	unsigned __int64 unique_no;				// 고유 아이디
	int error_cnt;							// 패킷 오류 Count
	int remainSize;							// Auth Login을 위하여
};
#endif