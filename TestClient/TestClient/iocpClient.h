#ifndef __ICOP_H__
#define __ICOP_H__

#include "Main.h"

enum class IOOperation {
	RECV,
	SEND,
	IO_EVENT,
	DisconnectRemove
};

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

class IOCP_Client {
public:
	HANDLE& getHandle() { return g_hiocp; }							// Handle Return
	ReadBuffer read_buffer;
	ReadBuffer send_buffer;

	IOCP_Client();													// init IOCP
	~IOCP_Client();													// remove IOCP
	bool BindRecv(class PLAYER * pPlayer);
	bool SendPacket(char* pMsg, int nLen);							// Packet Send 처리를 한다.
	bool initClient();
	bool connectServer(const char * ipAddres);
	void destroyThread();
	
	// get
	unsigned __int64 get_unique_no() { return unique_no; }
	int& get_remainSize() { return remainSize; }

	// set
	void set_unique_no(const unsigned __int64 value) { unique_no = value; }

private:
	SOCKET listenSocket;												// Socket
	HANDLE g_hiocp;														// handle 선언
	std::thread mIOWorkerThreads;										// IO Worker 스레드
	unsigned __int64 unique_no;											// 접속 unique_no
	bool mIsWorkerRun;													// 작업 Thread 동작 플래그
	int remainSize;														// Packet Read Size
	int errcnt;															// Packet Error Count
	stOverlappedEx	m_stRecvOverlappedEx;								// RECV Overlapped I/O작업을 위한 변수
	stOverlappedEx	m_stSendOverlappedEx;								// SEND Overlapped I/O작업을 위한 변수

	bool CreateWokerThread();											// WorkThread init
	void WokerThread();													// WorkThread
	void OnRecv(struct stOverlappedEx* pOver, int ioSize);				// Recv 처리를 진행 한다.

};

#endif