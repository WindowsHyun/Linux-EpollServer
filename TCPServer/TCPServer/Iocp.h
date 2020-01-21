#ifndef __ICOP_H__
#define __ICOP_H__

#include "Main.h"

class IOCP_Server {
public:
	HANDLE& getHandle() { return g_hiocp; }							// Handle Return
	IOCP_Server();													// init IOCP
	~IOCP_Server();													// remove IOCP
	bool initServer();
	bool BindandListen(const u_short port);
	bool StartServer();
	void initClient();
	void destroyThread();

private:
	SOCKET listenSocket;												// Socket
	std::chrono::high_resolution_clock::time_point serverTimer;			// 서버 기준 시간
	HANDLE g_hiocp;														// handle 선언
	std::vector<std::thread> mIOWorkerThreads;							// IO Worker 스레드
	std::thread	mAccepterThread;										// Accept 스레드
	unsigned __int64 uniqueId;											// 접속 UniqueID
	bool mIsWorkerRun;													// 작업 Thread 동작 플래그

	bool CreateWokerThread();											// WorkThread init
	void WokerThread();													// WorkThread
	bool mIsAccepterRun;												// Accept 
	bool CreateAccepterThread();										// AcceptThread init
	void AccepterThread();												// AcceptThread
	bool BindIOCompletionPort(class PLAYER_Session* pPlayerSession);				// Bind
	bool BindRecv(class PLAYER_Session* pPlayerSession, int remainSize);			// WSARecv Overlapped I/O 작업을 시킨다.
	void CloseSocket(class PLAYER_Session* pPlayerSession, bool bIsForce = false);	// Socket 연결을 끊는다.
	void ClosePlayer(unsigned __int64 uniqueId);									// Socket 연결을 끊는다.
	bool SendPacket(class PLAYER_Session* pPlayerSession, char* pMsg, int nLen);	// Packet Send 처리를 한다.
	void OnRecv(struct stOverlappedEx* pOver, int ioSize);							// Recv 처리를 진행 한다.
	//auto getPlayerSession(unsigned __int64 index) { return  player_session.find(index); }			// 플레이어 세션은 전달해 준다.

};

#endif