#ifndef __ICOP_H__
#define __ICOP_H__

#include "Main.h"
#define MAX_SOCKBUF	 1024	// �ִ� ��Ŷ ������

enum class IOOperation {
	RECV,
	SEND
};

// WSAOVERLAPPED����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���.
struct stOverlappedEx {
	WSAOVERLAPPED m_wsaOverlapped;						// Overlapped I/O����ü
	SOCKET					m_socketClient;							// Ŭ���̾�Ʈ ����
	WSABUF					m_wsaBuf;									// Overlapped I/O�۾� ����
	char							m_szBuf[MAX_SOCKBUF];		// ������ ����
	IOOperation			m_eOperation;							// �۾� ���� ����
};

class IOCP_Server {
private:
	SOCKET g_socket;												// Socket
	std::chrono::high_resolution_clock::time_point serverTimer;		// ���� ���� �ð�
	HANDLE g_hiocp;													// handle ����

	bool initServer();
	void err_quit(char *msg);										// Error ���� ��� Server ����
	void err_display(char *msg, int err_no);						// Error ǥ�� ���ֱ�

public:
	HANDLE getHandle() { return g_hiocp; }							// Handle Return
	IOCP_Server();													// init IOCP
	~IOCP_Server();													// remove IOCP
};

#endif