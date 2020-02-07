#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include "Main.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#define MAX_EVENTS 256		// ����Ǵ� �ִ� �������� ��
#define BACKLOG 10			// ���� ��� ť


class Epoll_Server {
public:
	Epoll_Server();
	~Epoll_Server();
	void init_server();
	void BindandListen(int port);
	class PLAYER_Session * getSessionByNo(int socketNo);

private:
	int sock;
	int epfd;															// size ��ŭ�� Ŀ�� ���� ���� [fd_epoll]
	struct sockaddr_in sin;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	std::mutex	mLock;
	std::queue<struct epoll_event> event_Queue;

	std::unordered_map<unsigned_int64, bool> disconnectUniqueNo;		// ����� UniqueNo
	std::queue<unsigned_int64> tempUniqueNo;							// �ӽ� uniqueNo
	bool mIsEventThreadRun;												// Event
	std::thread	mEventThread;											// Event Thread
	bool mIsWorkerThreadRun;											// Worker
	std::vector<std::thread> mWorkerThreads;							// Worker Thread
	void SetNonBlocking(int sock);
	void EventThread();													// EventThread Function
	void WorkerThread();												// WorkerThread Function
	void ClosePlayer(const int sock, struct epoll_event *ev);			// User Close
	bool AcceptProcessing(struct epoll_event &ev);						// Accept Processing
	void OnRecv(const int sock, const int ioSize);						// Recv ó���� ���� �Ѵ�.
};


#endif