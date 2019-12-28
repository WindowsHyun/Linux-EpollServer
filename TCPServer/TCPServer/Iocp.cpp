#include "Main.h"

bool IOCP_Server::initServer()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet) {
		std::cout << "[Error] WSAStartup()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	//연결지향형 TCP , Overlapped I/O 소켓을 생성
	listenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == listenSocket) {
		std::cout << "[Error] socket()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Socket Init Success..!" << std::endl;
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
		std::cout << "[Error] bind()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	// 접속 요청을 받아들이기 위해 cIOCompletionPort소켓을 등록하고 
	// 접속 대기큐를 5개로 설정 한다.
	nRet = ::listen(listenSocket, 5);
	if (0 != nRet) {
		std::cout << "[Error] listen()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	// 네이글 알고리즘 OFF
	int option = TRUE;
	setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&option, sizeof(option));

	std::cout << "Server Registration Successful..!" << std::endl;
	return true;
}

bool IOCP_Server::StartServer()
{
	// CompletionPort객체 생성 요청을 한다.
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
	if (NULL == g_hiocp) {
		std::cout << "[Error] CreateIoCompletionPort()함수 실패 : " << GetLastError() << std::endl;
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

	std::cout << "IOCP Server Start..!" << std::endl;
	return true;
}

void IOCP_Server::initClient()
{
	// 미리 player, player_session 공간을 할당 한다.
	player_session.reserve(MAX_PLAYER);
	player.reserve(MAX_PLAYER);

	// 세션 데이터도 미리 추가해 놓는다.
	for (int i = 0; i < MAX_PLAYER; ++i) {
		PLAYER_Session * pPlayerSession = new PLAYER_Session;
		pPlayerSession->set_init_session();
		player_session.push_back(pPlayerSession);
	}
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

	// 타이머 Thread를 종료 한다.
	timer.destroyTimer();
}

bool IOCP_Server::CreateWokerThread()
{
	// Vector 공간 미리 할당
	mIOWorkerThreads.reserve(MAX_WORKERTHREAD + 1);

	// WaingThread Queue에 대기 상태로 넣을 쓰레드들 생성 권장되는 개수 : (cpu개수 * 2) + 1 
	for (int i = 0; i < MAX_WORKERTHREAD; i++) {
		mIOWorkerThreads.emplace_back([this]() { WokerThread(); });
	}

	std::cout << "WokerThread Start..!" << std::endl;
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
			std::cout << "[" << (int)pPlayerSession->get_unique_id() << "] Socket 접속 끊김..." << std::endl;
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
			printf("[송신] bytes : %d , msg : %s\n", dwIoSize, pOverlappedEx->m_wsaBuf.buf);
		}
		break;
		default:
		{
			// 예외 상황
			std::cout << "[Exception WokerThread(" << (int)pOverlappedEx->m_eOperation << ")] No value defined..!" << std::endl;
		}
		break;

		}
	}
}

void IOCP_Server::CloseSocket(PLAYER_Session * pPlayerSession, bool bIsForce)
{
	struct linger stLinger = { 0, 0 };	// SO_DONTLINGER로 설정

	// bIsForce가 true이면 SO_LINGER, timeout = 0으로 설정하여 강제 종료 시킨다. 주의 : 데이터 손실이 있을수 있음 
	if (true == bIsForce)
	{
		stLinger.l_onoff = 1;
	}

	//socketClose소켓의 데이터 송수신을 모두 중단 시킨다.
	shutdown(pPlayerSession->m_socketSession, SD_BOTH);

	//소켓 옵션을 설정한다.
	setsockopt(pPlayerSession->m_socketSession, SOL_SOCKET, SO_LINGER, (char*)&stLinger, sizeof(stLinger));

	//소켓 연결을 종료 시킨다. 
	closesocket(pPlayerSession->m_socketSession);
	pPlayerSession->m_socketSession = INVALID_SOCKET;
}

void IOCP_Server::ClosePlayer(unsigned __int64 uniqueId)
{
	player.erase(uniqueId);
}

bool IOCP_Server::SendPacket(PLAYER_Session * pPlayerSession, char * pMsg, int nLen)
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

	int nRet = WSASend(pPlayerSession->m_socketSession,
		&(pPlayerSession->get_Send_over().m_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED) & (pPlayerSession->get_Send_over()),
		NULL);

	// socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING))
	{
		std::cout << "[Error] WSASend()함수 실패..! " << WSAGetLastError() << std::endl;
		return false;
	}
	return true;
}

void IOCP_Server::ProcessPacket(PLAYER_Session * pPlayerSession, const int protocolType,char * packet)
{

	cs_packet_dir *my_packet = reinterpret_cast<cs_packet_dir *>(packet);

	my_packet->packet_type;
	switch (protocolType) {
	case CLIENT_DIR:
		break;
	default:
		std::cout << "[Error] ProcessPacket ProtocolType(" << protocolType << ")이 없습니다..!" << std::endl;
		break;
	}
}

void IOCP_Server::OnRecv(struct stOverlappedEx* pOver, int ioSize)
{
	// 해결해야 한다 : playerSession은 vector로 되어 있어서 unique_id로 진행을 하게 될 경우
	// vector 범위가 벗어 난다. 등록 자체를 std::unordered_map<unsigned __int64, PLAYER *> 로 만들어
	// find로 찾는 방법을 진행 하는 것이 더 좋을 듯 하다.
	auto pPlayerSession = getPlayerSession(pOver->m_unique_id);
	std::cout << "(" << pPlayerSession->get_unique_id() << ") OnRecv..!" << std::endl;
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
	//	BindRecv(pPlayerSession);
	//}

}

bool IOCP_Server::CreateAccepterThread()
{
	mAccepterThread = std::thread([this]() { AccepterThread(); });

	std::cout << "AccepterThread Start..!" << std::endl;
	return true;
}

void IOCP_Server::AccepterThread()
{
	SOCKADDR_IN client_addr;
	auto client_len = static_cast<int>(sizeof(client_addr));


	while (mIsAccepterRun)
	{
		// 접속을 받을 구조체의 인덱스를 얻어온다.
		PLAYER_Session* pPlayerSession = GetEmptySession();
		if (NULL == pPlayerSession) {
			std::cout << "[Error] Client Full..!" << std::endl;
			return;
		}

		// 클라이언트 접속 요청이 들어올 때까지 기다린다.
		pPlayerSession->m_socketSession = WSAAccept(listenSocket, reinterpret_cast<sockaddr *>(&client_addr), &client_len, NULL, NULL);
		if (INVALID_SOCKET == pPlayerSession->m_socketSession) {
			continue;
		}

		// I/O Completion Port객체와 소켓을 연결시킨다.
		bool bRet = BindIOCompletionPort(pPlayerSession);
		if (false == bRet) {
			return;
		}

		// session에 set 해준다.
		pPlayerSession->set_unique_id(uniqueId);

		// 플레이어를 set 해준다.
		class PLAYER * acceptPlayer = new class PLAYER;
		acceptPlayer->set_sock(pPlayerSession->m_socketSession);
		acceptPlayer->set_unique_id(uniqueId);
		player.insert(std::unordered_map<unsigned __int64, class PLAYER *>::value_type(uniqueId, acceptPlayer));

		//클라이언트 갯수 증가
		++uniqueId;

		char clientIP[32] = { 0, };
		inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, 32 - 1);
		std::cout << "[접속(" << acceptPlayer->get_unique_id() << ")] Client IP : " << clientIP << " / SOCKET : " << (int)pPlayerSession->m_socketSession << std::endl;

		// Recv Overlapped I/O작업을 요청해 놓는다.
		bRet = BindRecv(pPlayerSession);
		if (false == bRet) {
			return;
		}

	}
}

PLAYER_Session* IOCP_Server::GetEmptySession()
{
	for (auto& client : player_session) {
		if (INVALID_SOCKET == client->m_socketSession) {
			return client;
		}
	}

	return nullptr;
}

bool IOCP_Server::BindIOCompletionPort(PLAYER_Session * pPlayerSession)
{
	// socket과 pPlayerSession를 CompletionPort객체와 연결시킨다.
	auto hIOCP = CreateIoCompletionPort((HANDLE)pPlayerSession->m_socketSession, g_hiocp, (ULONG_PTR)(pPlayerSession), 0);

	if (NULL == hIOCP || g_hiocp != hIOCP) {
		std::cout << "[Error] CreateIoCompletionPort()함수 실패 : " << GetLastError() << std::endl;
		return false;
	}

	return true;
}

bool IOCP_Server::BindRecv(PLAYER_Session * pPlayerSession)
{
	DWORD dwFlag = 0;
	DWORD dwRecvNumBytes = 0;
	WSABUF wBuf;

	//Overlapped I/O을 위해 각 정보를 셋팅해 준다.
	wBuf.len = MAX_SOCKBUF;
	wBuf.buf = pPlayerSession->m_readBuffer.getReadBuffer();
	pPlayerSession->get_Recv_over().m_eOperation = IOOperation::RECV;
	pPlayerSession->get_Recv_over().m_unique_id = pPlayerSession->get_unique_id();
	pPlayerSession->get_Recv_over().m_socketSession = pPlayerSession->m_socketSession;

	int nRet = WSARecv(pPlayerSession->m_socketSession,
		&wBuf,
		1,
		&dwRecvNumBytes,
		&dwFlag,
		(LPWSAOVERLAPPED) & (pPlayerSession->get_Recv_over()),
		NULL);

	//socket_error이면 client socket이 끊어진걸로 처리한다.
	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)) {
		std::cout << "[Error] WSARecv()함수 실패 : " << WSAGetLastError() << std::endl;
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
