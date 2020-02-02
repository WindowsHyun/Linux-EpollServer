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
	void add_tempUniqueNo(unsigned __int64 uniqueNo);
	bool BindRecv(class PLAYER_Session* pPlayerSession, int remainSize);			// WSARecv Overlapped I/O 작업을 시킨다.
	bool SendPacket(unsigned __int64 uniqueNo, char* pMsg, int nLen);				// Packet Send 처리를 한다.
	class PLAYER_Session* getSessionByNo(unsigned __int64 uniqueNo);

private:
	SOCKET listenSocket;												// Socket
	std::chrono::high_resolution_clock::time_point serverTimer;			// 서버 기준 시간
	HANDLE g_hiocp;														// handle 선언
	std::vector<std::thread> mIOWorkerThreads;							// IO Worker 스레드
	std::thread	mAccepterThread;										// Accept 스레드
	unsigned __int64 uniqueNo;											// 접속 uniqueNo
	bool mIsWorkerRun;													// 작업 Thread 동작 플래그
	std::queue<unsigned __int64> tempUniqueNo;							// 임시 uniqueNo
	std::unordered_map<unsigned __int64, bool> disconnectUniqueNo;		// 종료된 UniqueNo

	bool CreateWokerThread();											// WorkThread init
	void WokerThread();													// WorkThread
	bool mIsAccepterRun;												// Accept 
	bool CreateAccepterThread();										// AcceptThread init
	void AccepterThread();												// AcceptThread
	bool BindIOCompletionPort(class PLAYER_Session* pPlayerSession);				// Bind
	void CloseSocket(class PLAYER_Session* pPlayerSession, bool bIsForce = false);	// Socket 연결을 끊는다.
	void ClosePlayer(unsigned __int64 uniqueNo);									// Socket 연결을 끊는다.
	void OnRecv(struct stOverlappedEx* pOver, int ioSize);							// Recv 처리를 진행 한다.
	void OnSend(class PLAYER_Session* pSession, int size);
};

#endif