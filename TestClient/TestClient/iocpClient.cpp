#include "iocpClient.h"

IOCP_Client::IOCP_Client()
{
	std::wcout.imbue(std::locale("korean"));	// Locale Korean
	g_hiocp = INVALID_HANDLE_VALUE;
	listenSocket = INVALID_SOCKET;
	unique_no = 0;
	remainSize = 0;
	errcnt = 0;
	read_buffer.init(MAX_SOCKBUF);
	send_buffer.init(MAX_SOCKBUF);
	mIsWorkerRun = true;
}

IOCP_Client::~IOCP_Client()
{
	WSACleanup();
	destroyThread();
}

bool IOCP_Client::initClient()
{
	WSADATA	wsadata;
	int nRet = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (0 != nRet) {
		spdlog::error("WSAStartup() Function failure : {}", WSAGetLastError());
		return false;
	}
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	if (INVALID_SOCKET == listenSocket) {
		spdlog::error("socket() Function failure : {}", WSAGetLastError());
		return false;
	}

	// 네이글 알고리즘 OFF
	int option = TRUE;
	setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

	Player = new PLAYER;
	Player->set_sock(listenSocket);

	return true;
}

bool IOCP_Client::connectServer(const char * ipAddres)
{
	SOCKADDR_IN ServerAddr;
	ZeroMemory(&ServerAddr, sizeof(SOCKADDR_IN));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(SERVERPORT);
	ServerAddr.sin_addr.s_addr = inet_addr(ipAddres);

	int Result = WSAConnect(listenSocket, (sockaddr *)&ServerAddr, sizeof(ServerAddr), NULL, NULL, NULL, NULL);
	if (Result == SOCKET_ERROR) {
		spdlog::error("WSAConnect({}) Function failure : {}", ipAddres, Result);
		return false;
	}

	// CompletionPort객체 생성 요청을 한다.
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
	if (NULL == g_hiocp) {
		spdlog::error("CreateIoCompletionPort() Function failure : {}", GetLastError());
		return false;
	}

	// 접속된 클라이언트 주소 정보를 저장할 구조체
	bool bRet = CreateWokerThread();
	if (false == bRet) {
		return false;
	}

	// socket과 pPlayerSession를 CompletionPort객체와 연결시킨다.
	auto hIOCP = CreateIoCompletionPort((HANDLE)Player->get_sock(), g_hiocp, (ULONG_PTR)(Player), 0);

	if (NULL == hIOCP || g_hiocp != hIOCP) {
		spdlog::error("CreateIoCompletionPort() Function failure : {} || [unique_no:{}]", GetLastError(), Player->get_unique_no());
		return false;
	}

	BindRecv(Player);

	return true;
}

void IOCP_Client::destroyThread()
{
	mIsWorkerRun = false;
	mIOWorkerThreads.join();
}

bool IOCP_Client::BindRecv(PLAYER * pPlayer)
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;
	WSABUF wBuf;

	if (remainSize > 0) {
		read_buffer.checkWrite(remainSize);
	}

	// Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	wBuf.len = MAX_SOCKBUF;
	wBuf.buf = read_buffer.getWriteBuffer();
	m_stRecvOverlappedEx.m_eOperation = IOOperation::RECV;
	m_stRecvOverlappedEx.m_unique_no = Player->get_unique_no();
	m_stRecvOverlappedEx.m_socketSession = Player->get_sock();

	int nRet = WSARecv(Player->get_sock(),
		&wBuf,
		1,
		&dwRecvNumBytes,
		&dwFlag,
		(LPWSAOVERLAPPED) & (m_stRecvOverlappedEx),
		NULL);

	// socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
		spdlog::error("WSARecv() Function failure : {} || [unique_no:{}]", WSAGetLastError(), Player->get_unique_no());
		/*ClosePlayer(pPlayerSession->get_unique_no());
		CloseSocket(pPlayerSession);*/
		return false;
	}

	return true;
}

bool IOCP_Client::SendPacket(char * pMsg, int nLen)
{
	send_buffer.setWriteBuffer(pMsg, nLen);

	m_stSendOverlappedEx.m_wsaBuf.len = send_buffer.getReadAbleSize();
	m_stSendOverlappedEx.m_wsaBuf.buf = send_buffer.getReadBuffer();
	m_stSendOverlappedEx.m_eOperation = IOOperation::SEND;


	DWORD dwRecvNumBytes = 0;
	int nRet = WSASend(listenSocket,
		&(m_stSendOverlappedEx.m_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED) & (m_stSendOverlappedEx),
		NULL);

	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
		spdlog::error("WSASend() Function failure : {} || [unique_no:{}]", WSAGetLastError(), unique_no);
		return false;
	}
	send_buffer.moveReadPos(nLen);
	
	return true;
}

bool IOCP_Client::CreateWokerThread()
{
	mIOWorkerThreads = std::thread([this]() { WokerThread(); });
	return true;
}

void IOCP_Client::WokerThread()
{
	PLAYER* pPlayer = NULL;
	//함수 호출 성공 여부
	BOOL bSuccess = TRUE;
	//Overlapped I/O작업에서 전송된 데이터 크기
	DWORD dwIoSize = 0;
	//I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
	LPOVERLAPPED lpOverlapped = NULL;
	while (mIsWorkerRun)
	{
		bSuccess = GetQueuedCompletionStatus(g_hiocp,
			&dwIoSize,					// 실제로 전송된 바이트
			(PULONG_PTR)&pPlayer,	// CompletionKey
			&lpOverlapped,				// Overlapped IO 객체
			INFINITE);					// 대기할 시간

		// 사용자 쓰레드 종료 메세지 처리..
		if (TRUE == bSuccess && 0 == dwIoSize && NULL == lpOverlapped) {
			mIsWorkerRun = false;
			continue;
		}

		if (NULL == lpOverlapped) {
			continue;
		}

		// Server가 접속을 끊었을때..			
		if (FALSE == bSuccess )
		{
			spdlog::info("[Disconnect] SOCKET : {} || [unique_no:{}]", (int)pPlayer->get_sock(), (int)pPlayer->get_unique_no());
			//ClosePlayer(pPlayerSession->get_unique_no());
			//CloseSocket(pPlayerSession);
			continue;
		}
		else if (0 == dwIoSize && TRUE == bSuccess && !(read_buffer.getReadPos() <= read_buffer.getWritePos())) {
			continue;
		}

		stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;

		switch (pOverlappedEx->m_eOperation) {

		case IOOperation::RECV:
		{
			//spdlog::info("[RECV] bytes : {} , msg : {}", dwIoSize, read_buffer.getReadBuffer());
			OnRecv(pOverlappedEx, dwIoSize);
		}
		break;

		case IOOperation::SEND:
		{
			// Overlapped I/O Send작업 결과 뒤 처리
			//OnSend(pPlayerSession, dwIoSize);
			spdlog::info("[SEND] bytes : {} , msg : {}", dwIoSize, pOverlappedEx->m_wsaBuf.buf);
		}
		break;

		default:
		{
			// 예외 상황
			spdlog::critical("[Exception WokerThread({})] No value defined..! || [unique_no:{}]",
				(int)pOverlappedEx->m_eOperation, pPlayer->get_unique_no());
		}
		break;

		}


	}
}

void IOCP_Client::OnRecv(stOverlappedEx * pOver, int ioSize)
{
	// 쓰기를 위한 위치를 옮겨준다.
	if (!read_buffer.moveWritePos(ioSize)) {
		spdlog::error("ReadBuffer Over Flow || [unique_no:{}]", Player->get_unique_no());
	}

	PACKET_HEADER header;
	remainSize = 0;

	while (read_buffer.getReadAbleSize() > 0) {
		// 일정 개수 이상 Packet 오류가 나면 강제 종료 시킨다.
		if (errcnt >= LIMIT_ERROR_COUNT) {
			spdlog::error("Limit Error Count Maximum Exceeded / ErrorCnt({}) >= LimitCnt({}) || [unique_no:{}]",
				errcnt, LIMIT_ERROR_COUNT, Player->get_unique_no());
			/*ClosePlayer(pPlayerSession->get_unique_no());
			CloseSocket(pPlayerSession);*/
			break;
		}

		// 읽을 수 있는 Packet 크기가 Header Packet 보다 작을 경우 처리 한다.
		if (read_buffer.getReadAbleSize() <= sizeof(header)) {
			break;
		}

		// Packet_Header 를 가져온다.
		auto PacketSize = read_buffer.getHeaderSize((char*)&header, sizeof(header));
		if (PacketSize == -1) {
			spdlog::error("getHeaderSize || [unique_no:{}]", Player->get_unique_no());
		}

		if (read_buffer.getReadAbleSize() < header.packet_len || header.packet_len <= PACKET_HEADER_BYTE) {
			// 읽을 수 있는 Packet 사이즈가 전송된 Packet의 전체 사이즈보다 작을 경우 처리를 한다.
			spdlog::critical("Packet Header Critical AbleSize({}) <= PacketSize({}) OR getReadAbleSize({}) < PacketSize({}) || [unique_no:{}]",
				header.packet_len, PACKET_HEADER_BYTE, read_buffer.getReadAbleSize(), header.packet_len, Player->get_unique_no());
			// Packet 사이즈가 Header 크기보다 작을 경우 Error count를 올린다.
			if (header.packet_len <= PACKET_HEADER_BYTE) {
				errcnt += 1;
			}
			break;
		}
		else {
			// API 라이브러리로 해당 값을 전달 시켜 준다.
			ProtocolType protocolBase = (ProtocolType)((int)header.packet_type / (int)PACKET_RANG_SIZE * (int)PACKET_RANG_SIZE);

			api.packet_Add(Player->get_unique_no(), read_buffer.getReadBuffer(), header.packet_len);

			// 읽기 완료 처리
			read_buffer.moveReadPos(header.packet_len);
			remainSize += (ioSize - header.packet_len);
		}


	}



}
