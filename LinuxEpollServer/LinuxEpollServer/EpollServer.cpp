#include "EpollServer.h"

Epoll_Server::Epoll_Server()
{
	memset(&ev, 0, sizeof ev);
	mIsEventThreadRun = false;
	mIsWorkerThreadRun = false;
	disconnectUniqueNo.clear();
	// 임시 uniqueNo 추가
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
	
	mIsWorkerThreadRun = true;
	mWorkerThreads.reserve(MAX_WORKERTHREAD + 1);
	for (int i = 0; i < 1; i++) {
		mWorkerThreads.emplace_back([this]() { WorkerThread(); });
	}

	spdlog::info("Epoll Server Thread Start..!");
	ev.events = EPOLLIN;
	ev.data.fd = sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
}

PLAYER_Session * Epoll_Server::getSessionByNo(int socketNo)
{
	auto pTempPlayerSession = player_session.find(socketNo);
	if (pTempPlayerSession == player_session.end()) {
		if (disconnectUniqueNo.find(socketNo) == disconnectUniqueNo.end()) {
			spdlog::error("[getSessionByNo] No Exit Session || [socketNo:{}]", socketNo);
			disconnectUniqueNo.insert(pair<unsigned_int64, bool>(socketNo, true));
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
	ev.events = EPOLLIN | EPOLLRDHUP | EPOLLET;
	ev.data.fd = sock;
	epoll_ctl(epfd, EPOLL_CTL_ADD, sock, &ev);
}

void Epoll_Server::EventThread()
{
	int ci = 0;
	int nfds;
	while (mIsEventThreadRun)
	{
		// -1 : Event가 일어나기 전까지 계속 대기 한다.
		// 0  : Event가 일어나는지 상관 없이 조사만 하고 리턴
		// 1~ : 해당 시간동안 Event가 있는지 대기한다.
		nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);

		if (nfds < 0) {
			// critical error
			spdlog::error("epoll_wait() Function failure : {}", strerror(errno));
			exit(-1);
		}
		else if (nfds == 0) {
			// Event가 없다.
			continue;
		}

		for (int i = 0; i < nfds; i++) {
			std::lock_guard<std::mutex> guard(mLock);
			// 해당 이벤트를 event_Queue에 넣어준다.
			struct epoll_event eev;
			memcpy(&eev, &events[i], sizeof(events[i]));
			event_Queue.push(eev);
			++ci;
			spdlog::info("ci : {}, nfds : {}", ci, nfds);
		}

	}
}

void Epoll_Server::WorkerThread()
{
	int cnt = 0;
	while (mIsWorkerThreadRun)
	{
		std::lock_guard<std::mutex> guard(mLock);
		if (!event_Queue.empty()) {
			auto event = event_Queue.front();
			event_Queue.pop();
			if (event.data.fd == sock) {
				// 신규 유저 접속 처리
				PLAYER_Session* pPlayerSession = new PLAYER_Session;
				pPlayerSession->set_init_session();

				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof client_addr;

				pPlayerSession->get_sock() = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);
				if (pPlayerSession->get_sock() < 0) {
					// 정상적인 유저가 아니다.
					continue;
				}
				else if (player_session.size() >= CS.get_max_player()) {
					spdlog::critical("Client Full..! sessionSize({}) >= MAX_PLAYER({})", player_session.size(), CS.get_max_player());
					ClosePlayer(pPlayerSession->get_sock());
					continue;
				}

				// 임시 uniqueNo가 없을 경우 강제로 연결을 끊는다.
				if (tempUniqueNo.size() == 0) {
					spdlog::critical("tempUniqueNo Full..!");
					ClosePlayer(pPlayerSession->get_sock());
					continue;
				}

				// session에 set 해준다.
				pPlayerSession->set_unique_no(tempUniqueNo.front());

				// player_session에 추가 한다.
				player_session.insert(std::unordered_map<int, class PLAYER_Session *>::value_type(pPlayerSession->get_sock(), pPlayerSession));

				// 플레이어를 set 해준다.
				class PLAYER * acceptPlayer = new class PLAYER;
				acceptPlayer->set_sock(pPlayerSession->get_sock());
				acceptPlayer->set_unique_no(tempUniqueNo.front());
				player.insert(std::unordered_map<int, class PLAYER *>::value_type(pPlayerSession->get_sock(), acceptPlayer));

				//클라이언트 갯수 증가
				tempUniqueNo.pop();

				char clientIP[32] = { 0, };
				inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);
				spdlog::info("[Connect] Client IP : {} / SOCKET : {} || [unique_no:{}]", clientIP, (int)pPlayerSession->get_sock(), pPlayerSession->get_unique_no());

				// fd에 통신 준비 처리
				SetNonBlocking(pPlayerSession->get_sock());
			}
			else {
				// 기존 접속자
				++cnt;
				auto pPlayerSession = getSessionByNo(event.data.fd);
				int ioSize = read(event.data.fd, pPlayerSession->read_buffer().getReadBuffer(), sizeof(pPlayerSession->read_buffer().getReadBuffer()));

				if (ioSize == 0) {
					spdlog::info("[Disconnect] SOCKET : {} || [unique_no:{}]", (int)pPlayerSession->get_sock(), (int)pPlayerSession->get_unique_no());
					ClosePlayer(pPlayerSession->get_sock());
				}
				else if (ioSize < 0) {
					// read
					spdlog::info("ioSize : {}", ioSize);
				}
				else {
					// 계속 읽기.
					pPlayerSession->read_buffer().moveReadPos(ioSize);
				}

				spdlog::info("{}", cnt);
			}
			
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}
}

void Epoll_Server::ClosePlayer(const int sock)
{
	close(sock);
}
