#include "iocp.h"

bool IOCP_Server::initServer()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet) {
		spdlog::error("WSAStartup() Function failure : {}", WSAGetLastError());
		return false;
	}

	//연결지향형 TCP , Overlapped I/O 소켓을 생성
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == listenSocket) {
		spdlog::error("socket() Function failure : {}", WSAGetLastError());
		return false;
	}

	spdlog::info("Socket Init Success..!");
	return true;
}

bool IOCP_Server::BindandListen(const u_short port)
{
	SOCKADDR_IN		stServerAddr;
	stServerAddr.sin_family = AF_INET;
	stServerAddr.sin_port = htons(port); //서버 포트를 설정한다.		
	// 어떤 주소에서 들어오는 접속이라도 받아들이겠다.
	// 보통 서버라면 이렇게 설정한다. 만약 한 아이피에서만 접속을 받고 싶다면
	// 그 주소를 inet_addr함수를 이용해 넣으면 된다.
	stServerAddr.sin_addr.s_addr = INADDR_ANY;

	// 위에서 지정한 서버 주소 정보와 cIOCompletionPort 소켓을 연결한다.
	//int nRet = ::bind(listenSocket, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
	int nRet = ::bind(listenSocket, reinterpret_cast<sockaddr *>(&stServerAddr), sizeof(SOCKADDR_IN));
	if (0 != nRet) {
		spdlog::error("bind() Function failure : {}", WSAGetLastError());
		return false;
	}

	// 접속 요청을 받아들이기 위해 cIOCompletionPort소켓을 등록하고 
	// 접속 대기큐를 5개로 설정 한다.
	nRet = ::listen(listenSocket, 5);
	if (0 != nRet) {
		spdlog::error("listen() Function failure : {}", WSAGetLastError());
		return false;
	}

	// 네이글 알고리즘 OFF
	int option = TRUE;
	setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));


	spdlog::info("Server Registration Successful..!");
	return true;
}

bool IOCP_Server::StartServer()
{
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

	bRet = CreateAccepterThread();
	if (false == bRet) {
		return false;
	}

	spdlog::info("IOCP Server Start..!");
	return true;
}

void IOCP_Server::initClient()
{
	// 미리 player, player_session 공간을 할당 한다.
	player_session.reserve(MAX_PLAYER);
	player.reserve(MAX_PLAYER);
}

void IOCP_Server::destroyThread()
{
	// Worker Thread 종료한다.
	mIsWorkerRun = false;
	CloseHandle(g_hiocp);
	for (auto& th : mIOWorkerThreads) {
		if (th.joinable()) {
			th.join();
		}
	}

	// Accepter Thread 종료한다.
	mIsAccepterRun = false;
	closesocket(listenSocket);

	if (mAccepterThread.joinable()) {
		mAccepterThread.join();
	}

	// Timer, API Thread를 종료 한다.
	api.stop();
	timer.stop();
}

bool IOCP_Server::CreateWokerThread()
{
	// Vector 공간 미리 할당
	mIOWorkerThreads.reserve(MAX_WORKERTHREAD + 1);

	// WaingThread Queue에 대기 상태로 넣을 쓰레드들 생성 권장되는 개수 : (cpu개수 * 2) + 1 
	for (int i = 0; i < MAX_WORKERTHREAD; i++) {
		mIOWorkerThreads.emplace_back([this]() { WokerThread(); });
	}

	spdlog::info("WokerThread Start..!");
	return true;
}

void IOCP_Server::WokerThread()
{
	//CompletionKey를 받을 포인터 변수
	PLAYER_Session* pPlayerSession = NULL;
	//함수 호출 성공 여부
	BOOL bSuccess = TRUE;
	//Overlapped I/O작업에서 전송된 데이터 크기
	DWORD dwIoSize = 0;
	//I/O 작업을 위해 요청한 Overlapped 구조체를 받을 포인터
	LPOVERLAPPED lpOverlapped = NULL;

	while (mIsWorkerRun)
	{
		//////////////////////////////////////////////////////
		// 이 함수로 인해 쓰레드들은 WaitingThread Queue에
		// 대기 상태로 들어가게 된다.
		// 완료된 Overlapped I/O작업이 발생하면 IOCP Queue에서
		// 완료된 작업을 가져와 뒤 처리를 한다.
		// 그리고 PostQueuedCompletionStatus()함수에의해 사용자
		// 메세지가 도착되면 쓰레드를 종료한다.
		//////////////////////////////////////////////////////
		bSuccess = GetQueuedCompletionStatus(g_hiocp,
			&dwIoSize,					// 실제로 전송된 바이트
			(PULONG_PTR)&pPlayerSession,	// CompletionKey
			&lpOverlapped,				// Overlapped IO 객체
			INFINITE);					// 대기할 시간

		// 사용자 쓰레드 종료 메세지 처리..
		if (TRUE == bSuccess && 0 == dwIoSize && NULL == lpOverlapped)
		{
			mIsWorkerRun = false;
			continue;
		}

		if (NULL == lpOverlapped)
		{
			continue;
		}

		// client가 접속을 끊었을때..			
		if (FALSE == bSuccess || (0 == dwIoSize && TRUE == bSuccess))
		{
			spdlog::info("Socket Disconnected / [unique_id:{}]", (int)pPlayerSession->get_unique_id());
			ClosePlayer(pPlayerSession->get_unique_id());
			CloseSocket(pPlayerSession);
			continue;
		}

		stOverlappedEx* pOverlappedEx = (stOverlappedEx*)lpOverlapped;
		// Overlapped I/O Recv작업 결과 뒤 처리
		switch (pOverlappedEx->m_eOperation) {
		case IOOperation::RECV:
		{
			OnRecv(pOverlappedEx, dwIoSize);
		}
		break;
		case IOOperation::SEND:
		{
			// Overlapped I/O Send작업 결과 뒤 처리
			spdlog::info("[SEND] bytes : {} , msg : {}", dwIoSize, pOverlappedEx->m_wsaBuf.buf);
		}
		break;
		default:
		{
			// 예외 상황
			spdlog::critical("[Exception WokerThread({})] No value defined..! / [unique_id:{}]",
				(int)pOverlappedEx->m_eOperation, pPlayerSession->get_unique_id());
		}
		break;

		}
	}
}

void IOCP_Server::CloseSocket(class PLAYER_Session * pPlayerSession, bool bIsForce)
{
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER로 설정

	// bIsForce가 true이면 SO_LINGER, timeout = 0으로 설정하여 강제 종료 시킨다. 주의 : 데이터 손실이 있을수 있음 
	if (true == bIsForce)
	{
		stLinger.l_onoff = 1;
	}

	//socketClose소켓의 데이터 송수신을 모두 중단 시킨다.
	shutdown(pPlayerSession->get_sock(), SD_BOTH);

	//소켓 옵션을 설정한다.
	setsockopt(pPlayerSession->get_sock(), SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

	//소켓 연결을 종료 시킨다. 
	closesocket(pPlayerSession->get_sock());
	pPlayerSession->get_sock() = INVALID_SOCKET;
}

void IOCP_Server::ClosePlayer(unsigned __int64 uniqueId)
{
	player.erase(uniqueId);
	player_session.erase(uniqueId);
}

bool IOCP_Server::SendPacket(class PLAYER_Session * pPlayerSession, char * pMsg, int nLen)
{
	DWORD dwRecvNumBytes = 0;

	//PACKET_HEADER packetHader;
	//packetHader.packet_len = sizeof(pMsg);
	//packetHader.packet_type = CLIENT_BASE;
	//std::cout << packetHader.packet_len << std::endl;

	//reinterpret_cast<const char *>(&packetHader)

	//char sendMSG[MAX_SOCKBUF]{ 0 };

	// 전송될 메세지를 복사
	CopyMemory(pPlayerSession->get_Send_over().m_wsaBuf.buf, pMsg, nLen);


	// Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	pPlayerSession->get_Send_over().m_wsaBuf.len = nLen;
	pPlayerSession->get_Send_over().m_wsaBuf.buf = pPlayerSession->get_Send_over().m_wsaBuf.buf;
	pPlayerSession->get_Send_over().m_eOperation = IOOperation::SEND;

	int nRet = WSASend(pPlayerSession->get_sock(),
		&(pPlayerSession->get_Send_over().m_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED) & (pPlayerSession->get_Send_over()),
		NULL);

	// socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		spdlog::error("WSASend() Function failure : {} / [unique_id:{}]", WSAGetLastError(), pPlayerSession->get_unique_id());
		ClosePlayer(pPlayerSession->get_unique_id());
		CloseSocket(pPlayerSession);
		return false;
	}
	return true;
}

void IOCP_Server::OnRecv(struct stOverlappedEx* pOver, int ioSize)
{
	// 플레이어 세션 에서 플레이어 데이터 가져오기
	auto pTempPlayerSession = player_session.find(pOver->m_unique_id); // getPlayerSession(pOver->m_unique_id);
	if (pTempPlayerSession == player_session.end()) {
		spdlog::error("No Exit Session / [unique_id:{}]", pOver->m_unique_id);
		return;
	}
	auto pPlayerSession = pTempPlayerSession->second;

	//std::cout << "User No : " << pPlayerSession->get_unique_id() << " | OnRecv..!" << std::endl;
	//std::cout << "[INFO] ioSize : " << ioSize << std::endl;

	// 쓰기를 위한 위치를 옮겨준다.
	if (!pPlayerSession->get_buffer().moveWritePos(ioSize))
	{
		spdlog::error("ReadBuffer Over Flow / [unique_id:{}]", pPlayerSession->get_unique_id());
	}

	PACKET_HEADER header;
	int remainSize = 0;
	while (pPlayerSession->get_buffer().getReadAbleSize() > 0) {

		// 일정 개수 이상 Packet 오류가 나면 강제 종료 시킨다.
		if (pPlayerSession->get_error_cnt() >= LIMIT_ERROR_CNT) {
			spdlog::error("Limit Error Count Maximum Exceeded / ErrorCnt({}) >= LimitCnt({}) /  [unique_id:{}]", 
				pPlayerSession->get_error_cnt(), LIMIT_ERROR_CNT, pPlayerSession->get_unique_id());
			ClosePlayer(pPlayerSession->get_unique_id());
			CloseSocket(pPlayerSession);
			break;
		}
		
		// 읽을 수 있는 Packet 크기가 Header Packet 보다 작을 경우 처리 한다.
		if (pPlayerSession->get_buffer().getReadAbleSize() <= sizeof(header)) {
			break;
		}

		// Packet_Header 를 가져온다.
		auto PacketSize = pPlayerSession->get_buffer().getHeaderSize((char*)&header, sizeof(header));
		if (PacketSize == -1) {
			spdlog::error("getHeaderSize / [unique_id:{}]", pPlayerSession->get_unique_id());
		}

		if (pPlayerSession->get_buffer().getReadAbleSize() < header.packet_len || header.packet_len <= PACKET_HEADER_BYTE) {
			// 읽을 수 있는 Packet 사이즈가 전송된 Packet의 전체 사이즈보다 작을 경우 처리를 한다.
			spdlog::critical("Packet Header Critical AbleSize({}) <= PacketSize({}) || getReadAbleSize({}) < PacketSize({}) / [unique_id:{}]", 
				header.packet_len, PACKET_HEADER_BYTE, pPlayerSession->get_buffer().getReadAbleSize(), header.packet_len, pPlayerSession->get_unique_id());
			// Packet 사이즈가 Header 크기보다 작을 경우 Error count를 올린다.
			if (header.packet_len <= PACKET_HEADER_BYTE) {
				pPlayerSession->update_error_cnt();
			}
			break;
		}
		else {
			// 실제로 처리를 하는 위치

			// API 라이브러리로 해당 값을 전달 시켜 준다.
			ProtocolType protocolBase = (ProtocolType)((int)header.packet_type / (int)PACKET_RANG_SIZE * (int)PACKET_RANG_SIZE);

			api.packet_Add(pPlayerSession->get_unique_id(), pPlayerSession->get_buffer().getReadBuffer(), header.packet_len);

			// 읽기 완료 처리
			pPlayerSession->get_buffer().moveReadPos(header.packet_len);
			remainSize += (ioSize - header.packet_len);
		}

	}


	//pOverlappedEx->m_szBuf[dwIoSize] = NULL;

	//PACKET_HEADER *my_packet = reinterpret_cast<PACKET_HEADER *>(pOverlappedEx->m_wsaBuf.buf);
	//my_packet->packet_type;
	//ProcessPacket(pPlayerSession, pOverlappedEx->m_wsaBuf.buf[0], pOverlappedEx->m_wsaBuf.buf);
	//printf("[수신] bytes : %d , msg : %s\n", dwIoSize, pOverlappedEx->m_wsaBuf.buf);
	//////----------------------------------------------------
	////// 타이머 테스트
	////Timer_Event* t = new Timer_Event;
	////t->object_id = 0;
	////t->exec_time = high_resolution_clock::now() + 2048ms;
	////t->event = T_NormalTime;
	////timer.setTimerEvent(*t);
	//////----------------------------------------------------
	////클라이언트에 메세지를 에코한다. 'kch'일 경우에만 리턴을 하도록 하였다.
	//if (!strcmp(pOverlappedEx->m_wsaBuf.buf, "kch")) {
	//	//sc_packet_clientno packet;
	//	//packet.packet_len = sizeof(packet);
	//	//packet.packet_type = SERVER_CLIENT_NO;
	//	//packet.no = 0;


	//	SendPacket(pPlayerSession, pOverlappedEx->m_wsaBuf.buf, dwIoSize);

	//}
	BindRecv(pPlayerSession, remainSize);
}

bool IOCP_Server::CreateAccepterThread()
{
	mAccepterThread = std::thread([this]() { AccepterThread(); });

	spdlog::info("AccepterThread Start..!");
	return true;
}

void IOCP_Server::AccepterThread()
{
	SOCKADDR_IN client_addr;
	auto client_len = static_cast<int>(sizeof(client_addr));


	while (mIsAccepterRun)
	{
		// 접속 받을 유저 소켓을 생성 한다.
		PLAYER_Session* pPlayerSession = new PLAYER_Session;
		pPlayerSession->set_init_session();


		// 클라이언트 접속 요청이 들어올 때까지 기다린다.
		pPlayerSession->get_sock() = WSAAccept(listenSocket, reinterpret_cast<sockaddr *>(&client_addr), &client_len, NULL, NULL);
		if (INVALID_SOCKET == pPlayerSession->get_sock()) {
			continue;
		}
		else if (player_session.size() >= MAX_PLAYER) {
			spdlog::critical("Client Full..! sessionSize({}) >= MAX_PLAYER({})", player_session.size(), MAX_PLAYER);
			CloseSocket(pPlayerSession);
			continue;
		}

		// I/O Completion Port객체와 소켓을 연결시킨다.
		bool bRet = BindIOCompletionPort(pPlayerSession);
		if (false == bRet) {
			spdlog::error("BindIOCompletionPort() Function failure : {}", GetLastError());
			CloseSocket(pPlayerSession);
			continue;
		}

		// session에 set 해준다.
		pPlayerSession->set_unique_id(uniqueId);

		// player_session에 추가 한다.
		player_session.insert(std::unordered_map<unsigned __int64, class PLAYER_Session *>::value_type(uniqueId, pPlayerSession));

		// 플레이어를 set 해준다.
		class PLAYER * acceptPlayer = new class PLAYER;
		acceptPlayer->set_sock(pPlayerSession->get_sock());
		acceptPlayer->set_unique_id(uniqueId);
		player.insert(std::unordered_map<unsigned __int64, class PLAYER *>::value_type(uniqueId, acceptPlayer));

		//클라이언트 갯수 증가
		++uniqueId;

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);
		spdlog::info("[Connect] Client IP : {} / SOCKET : {} / [unique_id:{}]", clientIP, (int)pPlayerSession->get_sock(), pPlayerSession->get_unique_id());

		// Recv Overlapped I/O작업을 요청해 놓는다.
		bRet = BindRecv(pPlayerSession, 0);
		if (false == bRet) {
			spdlog::error("BindRecv() Function failure / [unique_id:{}]", pPlayerSession->get_unique_id());
			ClosePlayer(pPlayerSession->get_unique_id());
			CloseSocket(pPlayerSession);
			continue;
		}

	}
}

bool IOCP_Server::BindIOCompletionPort(class PLAYER_Session * pPlayerSession)
{
	// socket과 pPlayerSession를 CompletionPort객체와 연결시킨다.
	auto hIOCP = CreateIoCompletionPort((HANDLE)pPlayerSession->get_sock(), g_hiocp, (ULONG_PTR)(pPlayerSession), 0);

	if (NULL == hIOCP || g_hiocp != hIOCP) {
		spdlog::error("CreateIoCompletionPort() Function failure : {} / [unique_id:{}]", GetLastError(), pPlayerSession->get_unique_id());
		return false;
	}

	return true;
}

bool IOCP_Server::BindRecv(class PLAYER_Session * pPlayerSession, int remainSize)
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;
	WSABUF wBuf;

	if (remainSize > 0) {
		pPlayerSession->get_buffer().checkWrite(remainSize);
	}

	// Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	wBuf.len = MAX_SOCKBUF;
	wBuf.buf = pPlayerSession->get_buffer().getWriteBuffer();
	pPlayerSession->get_Recv_over().m_eOperation = IOOperation::RECV;
	pPlayerSession->get_Recv_over().m_unique_id = pPlayerSession->get_unique_id();
	pPlayerSession->get_Recv_over().m_socketSession = pPlayerSession->get_sock();

	int nRet = WSARecv(pPlayerSession->get_sock(),
		&wBuf,
		1,
		&dwRecvNumBytes,
		&dwFlag,
		(LPWSAOVERLAPPED) & (pPlayerSession->get_Recv_over()),
		NULL);

	// socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
		spdlog::error("WSARecv() Function failure : {} / [unique_id:{}]", WSAGetLastError(), pPlayerSession->get_unique_id());
		ClosePlayer(pPlayerSession->get_unique_id());
		CloseSocket(pPlayerSession);
		return false;
	}

	return true;
}

IOCP_Server::IOCP_Server()
{
	std::wcout.imbue(std::locale("korean"));	// Locale Korean
	g_hiocp = INVALID_HANDLE_VALUE;
	listenSocket = INVALID_SOCKET;
	uniqueId = UNIQUE_START_NO;
	mIsWorkerRun = true;
	mIsAccepterRun = true;
}

IOCP_Server::~IOCP_Server()
{
	WSACleanup();
	destroyThread();
}
