#ifndef __EPOLL_SERVER_H__
#define __EPOLL_SERVER_H__

#include "Main.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#define MAX_EVENTS 256		// ����Ǵ� �ִ� �������� ��
#define BACKLOG 10


class Epoll_Server {
public:
	Epoll_Server();
	~Epoll_Server();
	void init_server();
	void BindandListen(int port);
	class PLAYER_Session * getSessionByNo(unsigned_int64 uniqueNo);

private:
	int sock;
	int epfd;															// size ��ŭ�� Ŀ�� ���� ���� [fd_epoll]
	struct sockaddr_in sin;
	struct epoll_event ev;
	struct epoll_event events[MAX_EVENTS];
	std::unordered_map<unsigned_int64, bool> disconnectUniqueNo;		// ����� UniqueNo
	std::queue<unsigned_int64> tempUniqueNo;							// �ӽ� uniqueNo
	bool mIsEventThreadRun;												// Event 
	std::thread	mEventThread;											// Event Thread
	void SetNonBlocking(int sock);
	void EventThread();													// EventThread Function
	void ClosePlayer(const int sock);									// User Close
};


#endif