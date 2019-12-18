#ifndef __ICOP_H__
#define __ICOP_H__

class IOCP_Server {
private:
	SOCKET g_socket;												// Socket
	std::chrono::high_resolution_clock::time_point serverTimer;		// ���� ���� �ð�
	HANDLE g_hiocp;													// handle ����

	void initServer();
	void err_quit(char *msg);										// Error ���� ��� Server ����
	void err_display(char *msg, int err_no);						// Error ǥ�� ���ֱ�

public:
	HANDLE getHandle() { return g_hiocp; }							// Handle Return
	IOCP_Server();													// init IOCP
	~IOCP_Server();													// remove IOCP
};

#endif