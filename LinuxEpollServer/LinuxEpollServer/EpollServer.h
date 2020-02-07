#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include "Main.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#define MAX_EVENTS 256		// 예상되는 최대 동시접속 수
#define BACKLOG 10			// 접속 대기 큐


class Epoll_Server {
public:
	Epoll_Server();
	~Epoll_Server();
	void init_server();
	void BindandListen(int port);
	class PLAYER_Session * getSessionByNo(int socketNo);

private:
	int sock;
	int epfd;															// size 만큼의 커널 폴링 공간 [fd_epoll]
	struct sockaddr_in sin;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	std::mutex	mLock;
	std::queue<struct epoll_event> event_Queue;

	std::unordered_map<unsigned_int64, bool> disconnectUniqueNo;		// 종료된 UniqueNo
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