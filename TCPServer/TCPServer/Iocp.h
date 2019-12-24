#ifndef __ICOP_H__
#define __ICOP_H__

#include "Object.h"

#define MAX_SOCKBUF	 1024	// 최대 패킷 사이즈

class IOCP_Server {
public:
	HANDLE getHandle() { return g_hiocp; }							// Handle Return
	IOCP_Server();													// init IOCP
	~IOCP_Server();													// remove IOCP
	bool initServer();
	bool BindandListen(const u_short port);
	bool StartServer();
	void initClient(std::list<class PLAYER *>& player);
	void destroyThread();

private:
	SOCKET listenSocket;												// Socket
	std::chrono::high_resolution_clock::time_point serverTimer;			// 서버 기준 시간
	HANDLE g_hiocp;														// handle 선언
	std::vector<std::thread> mIOWorkerThreads;							// IO Worker 스레드
	std::thread	mAccepterThread;										// Accept 스레드
	std::vector<class PLAYER_Session *> player_session;					// 플레이어 세션
	unsigned __int64 uniqueId;											// 접속 UniqueID
	bool mIsWorkerRun;													// 작업 Thread 동작 플래그

	bool CreateWokerThread();											// WorkThread init
	void WokerThread();													// WorkThread
	bool mIsAccepterRun;												// Accept 
	bool CreateAccepterThread();										// AcceptThread init
	void AccepterThread();												// AcceptThread
	PLAYER_Session* GetEmptySession();									// Empty Session Search
	bool BindIOCompletionPort(PLAYER_Session* pClientInfo);				// Bind
	bool BindRecv(PLAYER_Session* pClientInfo);							// WSARecv Overlapped I/O 작업을 시킨다.
	void CloseSocket(class PLAYER_Session* pClientInfo, bool bIsForce = false);	// Socket 연결을 끊는다.
};

#endif