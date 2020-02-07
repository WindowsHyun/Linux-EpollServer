#include "EpollServer.h"

Epoll_Server::Epoll_Server()
{
	memset(&ev, 0, sizeof ev);
	mIsEventThreadRun = false;
	mIsWorkerThreadRun = false;
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
			continue;
			//exit(-1);
		}
		else if (nfds == 0) {
			// Event�� ����.
			continue;
		}

		for (int i = 0; i < nfds; i++) {
			if (events[i].data.fd == sock) {
				// �ű� ���� ���� ó��
				AcceptProcessing(events[i]);
			}
			else {
				std::lock_guard<std::mutex> guard(mLock);
				// �ش� �̺�Ʈ�� event_Queue�� �־��ش�.
				struct epoll_event eev;
				memcpy(&eev, &events[i], sizeof(events[i]));
				event_Queue.push(eev);
			}
			
		}

	}
}

void Epoll_Server::WorkerThread()
{
	while (mIsWorkerThreadRun)
	{
		std::lock_guard<std::mutex> guard(mLock);
		if (!event_Queue.empty()) {
			auto event = event_Queue.front();
			event_Queue.pop();
			spdlog::info("eventQueue Size : {}", event_Queue.size());
			mLock.unlock();
			// ���� ������
			auto pPlayerSession = getSessionByNo(event.data.fd);
			if (pPlayerSession == nullptr) continue;

			if (event.events & EPOLLIN) {
				int ioSize = 0;

				if (pPlayerSession->get_remainSize() > 0) {
					pPlayerSession->read_buffer().checkWrite(pPlayerSession->get_remainSize());
				}
				ioSize = read(pPlayerSession->get_sock(), pPlayerSession->read_buffer().getWriteBuffer(), pPlayerSession->read_buffer().getWriteAbleSize());

				if (ioSize == 0) {
					spdlog::info("[Disconnect] EPOLLIN SOCKET : {} || [unique_no:{}]", (int)pPlayerSession->get_sock(), (int)pPlayerSession->get_unique_no());
					ClosePlayer(pPlayerSession->get_sock(), &event);
				}
				else if (ioSize < 0) {
					// Read Error
					spdlog::error("[Exception WorkerThread()] Read Error ioSize : {} || [unique_no:{}]",
						ioSize, pPlayerSession->get_unique_no());
				}
				else {
					// ��� �б�.
					spdlog::info("ioSize : {}", ioSize);
					OnRecv(event.data.fd, ioSize);
					//pPlayerSession->read_buffer().moveReadPos(ioSize);
					
				}

			}
			else if (event.events & EPOLLERR) {
				spdlog::info("[Disconnect] EPOLLERR SOCKET : {} || [unique_no:{}]", (int)pPlayerSession->get_sock(), (int)pPlayerSession->get_unique_no());
				ClosePlayer(pPlayerSession->get_sock(), &event);
			}
			else  if (event.events & EPOLLOUT) {
				// sendIO
			}
			else if (event.events & EPOLLRDHUP) {
				spdlog::info("[Disconnect] EPOLLRDHUP SOCKET : {} || [unique_no:{}]", (int)pPlayerSession->get_sock(), (int)pPlayerSession->get_unique_no());
				ClosePlayer(pPlayerSession->get_sock(), &event);
			}else {
				spdlog::error("[Exception WorkerThread()] No Event ({}) || [unique_no:{}]",
					ev.events, pPlayerSession->get_unique_no());
			}
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}
}

void Epoll_Server::ClosePlayer(const int sock, struct epoll_event *ev)
{
	epoll_ctl(epfd, EPOLL_CTL_DEL, sock, ev);
	close(sock);
}

bool Epoll_Server::AcceptProcessing(struct epoll_event &ev)
{
	// �ű� ���� ���� ó��
	PLAYER_Session* pPlayerSession = new PLAYER_Session;
	pPlayerSession->set_init_session();

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof client_addr;

	pPlayerSession->get_sock() = accept(sock, (struct sockaddr *) &client_addr, &client_addr_len);
	if (pPlayerSession->get_sock() < 0) {
		// �������� ������ �ƴϴ�.
		return false;
	}
	else if (player_session.size() >= CS.get_max_player()) {
		spdlog::critical("Client Full..! sessionSize({}) >= MAX_PLAYER({})", player_session.size(), CS.get_max_player());
		ClosePlayer(pPlayerSession->get_sock(), &ev);
		return false;
	}

	// �ӽ� uniqueNo�� ���� ��� ������ ������ ���´�.
	if (tempUniqueNo.size() == 0) {
		spdlog::critical("tempUniqueNo Full..!");
		ClosePlayer(pPlayerSession->get_sock(), &ev);
		return false;
	}

	// session�� set ���ش�.
	pPlayerSession->set_unique_no(tempUniqueNo.front());

	// player_session�� �߰� �Ѵ�.
	player_session.insert(std::unordered_map<int, class PLAYER_Session *>::value_type(pPlayerSession->get_sock(), pPlayerSession));

	// �÷��̾ set ���ش�.
	class PLAYER * acceptPlayer = new class PLAYER;
	acceptPlayer->set_sock(pPlayerSession->get_sock());
	acceptPlayer->set_unique_no(tempUniqueNo.front());
	player.insert(std::unordered_map<int, class PLAYER *>::value_type(pPlayerSession->get_sock(), acceptPlayer));

	//Ŭ���̾�Ʈ ���� ����
	tempUniqueNo.pop();

	// fd�� ��� �غ� ó��
	SetNonBlocking(pPlayerSession->get_sock());

	char clientIP[32] = { 0, };
	inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);
	spdlog::info("[Connect] Client IP : {} / SOCKET : {} || [unique_no:{}]", clientIP, (int)pPlayerSession->get_sock(), pPlayerSession->get_unique_no());
}

void Epoll_Server::OnRecv(const int sock, const int ioSize)
{
	auto pPlayerSession = getSessionByNo(sock);

	// ���⸦ ���� ��ġ�� �Ű��ش�.
	if (!pPlayerSession->read_buffer().moveWritePos(ioSize))
	{
		spdlog::error("ReadBuffer Over Flow || [unique_no:{}]", pPlayerSession->get_unique_no());
	}

	PACKET_HEADER header;
	pPlayerSession->set_remainSize(0);
	while (pPlayerSession->read_buffer().getReadAbleSize() > 0) {
		spdlog::info("readPos : {}, writePos : {}, ReadAbleSize : {}", 
			pPlayerSession->read_buffer().getReadPos(), pPlayerSession->read_buffer().getWritePos(), 
			pPlayerSession->read_buffer().getReadAbleSize());
		// ���� �� �ִ� Packet ũ�Ⱑ Header Packet ���� ���� ��� ó�� �Ѵ�.
		if (pPlayerSession->read_buffer().getReadAbleSize() <= sizeof(header)) {
			break;
		}

		// Packet_Header �� �����´�.
		auto PacketSize = pPlayerSession->read_buffer().getHeaderSize((char*)&header, sizeof(header));
		if (PacketSize == -1) {
			spdlog::error("getHeaderSize || [unique_no:{}]", pPlayerSession->get_unique_no());
		}

		if (pPlayerSession->read_buffer().getReadAbleSize() < header.packet_len || header.packet_len <= PACKET_HEADER_BYTE) {
			// ���� �� �ִ� Packet ����� ���۵� Packet�� ��ü ������� ���� ��� ó���� �Ѵ�.
			spdlog::critical("Packet Header Critical AbleSize({}) <= PacketSize({}) OR getReadAbleSize({}) < PacketSize({}) || [unique_no:{}]",
				header.packet_len, PACKET_HEADER_BYTE, pPlayerSession->read_buffer().getReadAbleSize(), header.packet_len, pPlayerSession->get_unique_no());
			// Packet ����� Header ũ�⺸�� ���� ��� Error count�� �ø���.
			if (header.packet_len <= PACKET_HEADER_BYTE) {
				pPlayerSession->update_error_cnt();
			}
			break;
		}
		else {
			// ������ ó���� �ϴ� ��ġ

			// API ���̺귯���� �ش� ���� ���� ���� �ش�.
			ProtocolType protocolBase = (ProtocolType)((int)header.packet_type / (int)PACKET_RANG_SIZE * (int)PACKET_RANG_SIZE);

			spdlog::info("uniqueNo : {}, packet Size : {}, protocolBase : {}, protocolType : {}", 
				pPlayerSession->get_unique_no(), header.packet_len, protocolBase, header.packet_type);

			//api.packet_Add(pPlayerSession->get_unique_no(), pPlayerSession->read_buffer().getReadBuffer(), header.packet_len);

			// �б� �Ϸ� ó��
			pPlayerSession->read_buffer().moveReadPos(header.packet_len);
			pPlayerSession->incr_remainSize((ioSize - header.packet_len));
		}


	}

}
