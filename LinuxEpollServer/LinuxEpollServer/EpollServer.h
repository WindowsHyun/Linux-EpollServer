#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include "Main.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <errno.h>
#define MAX_EVENTS 256			// 예상되는 최대 동시접속 수
#define BACKLOG 10				// 접속 대기 큐
#define CONNECTION_RESET 104	// 클라이언트 접속 리셋 되었다.


class Epoll_Server {
public:
	Epoll_Server();
	~Epoll_Server();
	void init_server();
	void BindandListen(int port);
	void add_tempUniqueNo(unsigned_int64 uniqueNo);								// 사용된 uniqueNo 다시 등록
	bool SendPacket(int sock, char* pMsg, int nLen);							// Packet Send 처리를 한다.
	class PLAYER_Session * getSessionByNo(int socketNo);						// PlayerSession 가져오기

private:
	int sock;
	int epfd;															// size 만큼의 커널 폴링 공간 [fd_epoll]
	struct sockaddr_in sin;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	std::mutex	mLock;
	std::queue<struct epoll_event> event_Queue;

	std::unordered_map<int, bool> disconnectUniqueNo;					// 종료된 UniqueNo
	std::queue<unsigned_int64> tempUniqueNo;							// 임시 uniqueNo
	bool mIsEventThreadRun;												// Event
	std::thread	mEventThread;											// Event Thread
	bool mIsWorkerThreadRun;											// Worker
	std::vector<std::thread> mWorkerThreads;							// Worker Thread
	void SetNonBlocking(int sock);
	void EventThread();													// EventThread Function
	void WorkerThread();												// WorkerThread Function
	void ClosePlayer(const int sock, struct epoll_event *ev);			// User Close
	bool AcceptProcessing(struct epoll_event &ev);						// Accept Processing
	void OnRecv(const int sock, const int ioSize);						// Recv 처리를 진행 한다.
};


#endif