#include "iocpClient.h"

IOCP_Client::IOCP_Client()
{
	std::wcout.imbue(std::locale("korean"));	// Locale Korean
	g_hiocp = INVALID_HANDLE_VALUE;
	listenSocket = INVALID_SOCKET;
	uniqueNo = 0;
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

	// CompletionPort��ü ���� ��û�� �Ѵ�.
	g_hiocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, MAX_WORKERTHREAD);
	if (NULL == g_hiocp) {
		spdlog::error("CreateIoCompletionPort() Function failure : {}", GetLastError());
		return false;
	}

	// ���ӵ� Ŭ���̾�Ʈ �ּ� ������ ������ ����ü
	bool bRet = CreateWokerThread();
	if (false == bRet) {
		return false;
	}

	return true;
}

void IOCP_Client::destroyThread()
{
	mIsWorkerRun = false;
	mIOWorkerThreads.join();
}

bool IOCP_Client::SendPacket(char * pMsg, int nLen)
{
	int retval = send(listenSocket, pMsg, nLen, 0);
	return retval;
}

bool IOCP_Client::CreateWokerThread()
{
	mIOWorkerThreads = std::thread([this]() { WokerThread(); });
	return true;
}

void IOCP_Client::WokerThread()
{
	testcnt = 0;
	while (mIsWorkerRun)
	{
		int retval = 0;
		remainSize = 0;
		PACKET_HEADER header;

		retval = recv(listenSocket, read_buffer.getReadBuffer(), MIN_SOCKBUF, 0);
		if (retval == SOCKET_ERROR) {
			spdlog::error("recv() Function failure : {}({})", GetLastError(), retval);
			break;
		}
		else if (retval < sizeof(header)) {
			// Packet�� �� ���� ���Ͽ��� ��� �ٽ� recv �޵��� �Ѵ�.
			read_buffer.moveReadPos(retval);
			continue;
		}

		// ���� ���� ��ġ �������� �Ű��ش�.
		if (!read_buffer.moveWritePos(retval)){
			spdlog::error("ReadBuffer Over Flow || [unique_no:{}]", uniqueNo);
		}

		while (true) {
			// while�� ����� ���� :
			// �ѹ��� ��Ŷ�� �������� ��Ŷ�� ���ļ� �� �� �ִ�.
			// remainSize�� retval(���� ��Ŷ ��)�� ���� ���� �� ���� ��� �ݺ��Ͽ�, ��Ŷ ó���� �Ѵ�.
			if (remainSize >= retval) {
				break;
			}

			// Packet_Header �� �����´�.
			auto PacketSize = read_buffer.getHeaderSize((char*)&header, sizeof(header));
			if (PacketSize == -1) {
				spdlog::error("getHeaderSize || [unique_no:{}]", uniqueNo);
				break;
			}
			else if (header.packet_len == 0) {
				// Packer Header�� ������ �ִ�.
				read_buffer.moveReadPos(retval- remainSize);
				break;
			}

			if (read_buffer.getReadAbleSize() < header.packet_len || header.packet_len <= PACKET_HEADER_BYTE) {
				// ���� �� �ִ� Packet ����� ���۵� Packet�� ��ü ������� ���� ��� ó���� �Ѵ�.
				spdlog::critical("Packet Header Critical AbleSize({}) <= PacketSize({}) OR getReadAbleSize({}) < PacketSize({}) || [unique_no:{}]",
					header.packet_len, PACKET_HEADER_BYTE, read_buffer.getReadAbleSize(), header.packet_len, uniqueNo);
				// Packet ����� Header ũ�⺸�� ���� ��� Error count�� �ø���.
				if (header.packet_len <= PACKET_HEADER_BYTE) {
					errcnt++;
				}
				break;
			}
			else {
				// ������ ó�� �ϴ� ��ġ
				// API ���̺귯���� �ش� ���� ���� ���� �ش�.
				ProtocolType protocolBase = (ProtocolType)((int)header.packet_type / (int)PACKET_RANG_SIZE * (int)PACKET_RANG_SIZE);

				api.packet_Add(uniqueNo, read_buffer.getReadBuffer(), header.packet_len);
				++testcnt;
				// �б� �Ϸ� ó��
				read_buffer.moveReadPos(header.packet_len);
				remainSize += (header.packet_len);
				read_buffer.checkWrite(remainSize);
				spdlog::info("{}", testcnt);
			}
		}

	}
}
