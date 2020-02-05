#include "EpollServer.h"

Epoll_Server::Epoll_Server()
{
	memset(&ev, 0, sizeof ev);
	mIsEventThreadRun = false;
	disconnectUniqueNo.clear();
	// �ӽ� uniqueNo �߰�
	for (int i = 0; i < UNIQUE_START_NO; ++i) {
		tempUniqueNo.push(i);
	}
}

Epoll_Server::~Epoll_Server()
{
}

void Epoll_Server::init_server()
{
	if ((epfd = epoll_create(MAX_EVENTS)) < 0) {
		spdlog::error("epoll_create() Function failure");
	}

	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		spdlog::error("socket() Function failure");
		exit(EXIT_FAILURE);
	}

}

void Epoll_Server::BindandListen(int port)
{
	memset(&sin, 0, sizeof sin);

	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);

	if (bind(sock, (struct sockaddr *) &sin, sizeof sin) < 0) {
		close(sock);
		spdlog::error("bind() Function failure");
		exit(EXIT_FAILURE);
	}

	if (listen(sock, BACKLOG) < 0) {
		close(sock);
		spdlog::error("listen() Function failure");
		exit(EXIT_FAILURE);
	}

	mIsEventThreadRun = true;
	mEventThread = std::thread([this]() { EventThread(); });
	spdlog::info("EventThread Start..!");

	ev.events = EPOLLIN;
	ev.data.fd = sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
}

PLAYER_Session * Epoll_Server::getSessionByNo(unsigned_int64 uniqueNo)
{
	auto pTempPlayerSession = player_session.find(uniqueNo);
	if (pTempPlayerSession == player_session.end()) {
		if (disconnectUniqueNo.find(uniqueNo) == disconnectUniqueNo.end()) {
			spdlog::error("[getSessionByNo] No Exit Session || [unique_no:{}]", uniqueNo);
			disconnectUniqueNo.insert(pair<unsigned_int64, bool>(uniqueNo, true));
		}
		return nullptr;
	}
	auto pPlayerSession = pTempPlayerSession->second;

	return pPlayerSession;
}

void Epoll_Server::SetNonBlocking(int sock)
{
	int flag = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, flag | O_NONBLOCK);

	memset(&ev, 0, sizeof ev);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
}

void Epoll_Server::EventThread()
{
	int nfds;
	while (mIsEventThreadRun)
	{
		// -1 : Event�� �Ͼ�� ������ ��� ��� �Ѵ�.
		// 0  : Event�� �Ͼ���� ��� ���� ���縸 �ϰ� ����
		// 1~ : �ش� �ð����� Event�� �ִ��� ����Ѵ�.
		nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

		if (nfds < 0) {
			// critical error
			spdlog::error("epoll_wait() Function failure : {}", strerror(errno));
			exit(-1);
		}
		else if (nfds == 0) {
			// Event�� ����.
			continue;
		}

		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == sock) {
				// �ű� ���� ���� ó��
				PLAYER_Session* pPlayerSession = new PLAYER_Session;
				pPlayerSession->set_init_session();

				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof client_addr;

				pPlayerSession->get_sock() = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);
				if (pPlayerSession->get_sock() < 0) {
					// �������� ������ �ƴϴ�.
					continue;
				}
				else if (player_session.size() >= CS.get_max_player()) {
					spdlog::critical("Client Full..! sessionSize({}) >= MAX_PLAYER({})", player_session.size(), CS.get_max_player());
					ClosePlayer(pPlayerSession->get_sock());
					continue;
				}

				// �ӽ� uniqueNo�� ���� ��� ������ ������ ���´�.
				if (tempUniqueNo.size() == 0) {
					spdlog::critical("tempUniqueNo Full..!");
					ClosePlayer(pPlayerSession->get_sock());
					continue;
				}

				// session�� set ���ش�.
				pPlayerSession->set_unique_no(tempUniqueNo.front());

				// player_session�� �߰� �Ѵ�.
				player_session.insert(std::unordered_map<unsigned_int64, class PLAYER_Session *>::value_type(tempUniqueNo.front(), pPlayerSession));

				// �÷��̾ set ���ش�.
				class PLAYER * acceptPlayer = new class PLAYER;
				acceptPlayer->set_sock(pPlayerSession->get_sock());
				acceptPlayer->set_unique_no(tempUniqueNo.front());
				player.insert(std::unordered_map<unsigned_int64, class PLAYER *>::value_type(tempUniqueNo.front(), acceptPlayer));

				//Ŭ���̾�Ʈ ���� ����
				tempUniqueNo.pop();

				char clientIP[32] = { 0, };
				inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);
				spdlog::info("[Connect] Client IP : {} / SOCKET : {} || [unique_no:{}]", clientIP, (int)pPlayerSession->get_sock(), pPlayerSession->get_unique_no());
				
				// fd�� ��� �غ� ó��
				SetNonBlocking(pPlayerSession->get_sock());
			}
			else {
				// ���� ���� ���� ó��
			}

		}

	}
}

void Epoll_Server::ClosePlayer(const int sock)
{
	close(sock);
}
