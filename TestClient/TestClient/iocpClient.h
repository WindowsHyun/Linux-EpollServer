#ifndef __ICOP_H__
#define __ICOP_H__

#include "Main.h"

class IOCP_Client {
public:
	HANDLE& getHandle() { return g_hiocp; }							// Handle Return
	IOCP_Client();													// init IOCP
	~IOCP_Client();													// remove IOCP
	bool SendPacket(char* pMsg, int nLen);							// Packet Send 처리를 한다.
	bool initClient();
	bool connectServer(const char * ipAddres);
	void destroyThread();
	int& get_remainSize() { return remainSize; }
	ReadBuffer read_buffer;
	ReadBuffer send_buffer;
	

private:
	SOCKET listenSocket;												// Socket
	HANDLE g_hiocp;														// handle 선언
	std::thread mIOWorkerThreads;										// IO Worker 스레드
	unsigned __int64 uniqueNo;											// 접속 uniqueNo
	bool mIsWorkerRun;													// 작업 Thread 동작 플래그
	int remainSize;
	int errcnt;
	int testcnt;

	bool CreateWokerThread();											// WorkThread init
	void WokerThread();													// WorkThread
	void OnRecv(struct stOverlappedEx* pOver, int ioSize);							// Recv 처리를 진행 한다.
	void OnSend(class PLAYER_Session* pSession, int size);
};

#endif