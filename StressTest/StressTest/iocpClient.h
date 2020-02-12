#ifndef __ICOP_H__
#define __ICOP_H__

#include "Main.h"

enum class IOOperation {
	RECV,
	SEND,
	IO_EVENT,
	DisconnectRemove
};

struct stOverlappedEx {
	WSAOVERLAPPED			m_wsaOverlapped;			// Overlapped I/O����ü
	SOCKET					m_socketSession;			// Ŭ���̾�Ʈ ����
	WSABUF					m_wsaBuf;					// Overlapped I/O�۾� ����
	IOOperation				m_eOperation;				// �۾� ���� ����
	unsigned __int64		m_unique_no;				// ���� ���� ��ȣ

	stOverlappedEx() {
		memset(&m_wsaOverlapped, 0, sizeof(OVERLAPPED));
		memset(&m_wsaBuf, 0, sizeof(OVERLAPPED));
		m_socketSession = INVALID_SOCKET;
		m_unique_no = 0;
	}
};

class IOCP_Client {
public:
	HANDLE& getHandle() { return g_hiocp; }							// Handle Return
	ReadBuffer read_buffer;
	ReadBuffer send_buffer;

	IOCP_Client();													// init IOCP
	~IOCP_Client();													// remove IOCP
	bool BindRecv(class PLAYER * pPlayer);
	bool SendPacket(char* pMsg, int nLen);							// Packet Send ó���� �Ѵ�.
	bool initClient();
	bool connectServer(const char * ipAddres);
	void destroyThread();
	
	// get
	unsigned __int64 get_unique_no() { return unique_no; }
	int& get_remainSize() { return remainSize; }

	// set
	void set_unique_no(const unsigned __int64 value) { unique_no = value; }

private:
	SOCKET listenSocket;												// Socket
	HANDLE g_hiocp;														// handle ����
	std::thread mIOWorkerThreads;										// IO Worker ������
	unsigned __int64 unique_no;											// ���� unique_no
	bool mIsWorkerRun;													// �۾� Thread ���� �÷���
	int remainSize;														// Packet Read Size
	int errcnt;															// Packet Error Count
	stOverlappedEx	m_stRecvOverlappedEx;								// RECV Overlapped I/O�۾��� ���� ����
	stOverlappedEx	m_stSendOverlappedEx;								// SEND Overlapped I/O�۾��� ���� ����

	bool CreateWokerThread();											// WorkThread init
	void WokerThread();													// WorkThread
	void OnRecv(struct stOverlappedEx* pOver, int ioSize);				// Recv ó���� ���� �Ѵ�.

};

#endif