#include "Main.h"

bool IOCP_Server::initServer()
{
	WSADATA wsaData;

	int nRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != nRet){
		std::cout << "[Error] WSAStartup()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	//연결지향형 TCP , Overlapped I/O 소켓을 생성
	g_socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	if (INVALID_SOCKET == g_socket){
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
	//어떤 주소에서 들어오는 접속이라도 받아들이겠다.
	//보통 서버라면 이렇게 설정한다. 만약 한 아이피에서만 접속을 받고 싶다면
	//그 주소를 inet_addr함수를 이용해 넣으면 된다.
	stServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//위에서 지정한 서버 주소 정보와 cIOCompletionPort 소켓을 연결한다.
	int nRet = bind(g_socket, (SOCKADDR*)&stServerAddr, sizeof(SOCKADDR_IN));
	if (0 != nRet){
		std::cout << "[Error] bind()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	//접속 요청을 받아들이기 위해 cIOCompletionPort소켓을 등록하고 
	//접속대기큐를 5개로 설정 한다.
	nRet = listen(g_socket, 5);
	if (0 != nRet){
		std::cout << "[Error] listen()함수 실패 : " << WSAGetLastError() << std::endl;
		return false;
	}

	std::cout << "Server Registration Successful..!" << std::endl;
	return true;
}

bool IOCP_Server::StartServer()
{
	//CompletionPort객체 생성 요청을 한다.
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
	if (NULL == g_hiocp){
		std::cout << "[Error] CreateIoCompletionPort()함수 실패 : " << GetLastError() << std::endl;
		return false;
	}

	//접속된 클라이언트 주소 정보를 저장할 구조체
	//bool bRet = CreateWokerThread();
	//if (false == bRet) {
	//	return false;
	//}

	//bRet = CreateAccepterThread();
	//if (false == bRet) {
	//	return false;
	//}

	std::cout << "IOCP Server Start..!" << std::endl;
	return true;
}

void IOCP_Server::initClient(std::list<class PLAYER>& player)
{
	// 플레이어 데이터를 미리 추가해 놓는다.
	for (int i = 0; i < MAX_PLAYER; ++i) {
		player.emplace_back();
	}
}

IOCP_Server::IOCP_Server()
{
	std::wcout.imbue(std::locale("korean"));	// Locale Korean
	uniqueId = UNIQUE_START_NO;
}

IOCP_Server::~IOCP_Server()
{
	WSACleanup();
}
