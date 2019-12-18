#ifndef __ICOP_H__
#define __ICOP_H__

class IOCP_Server {
private:
	SOCKET g_socket;												// Socket
	std::chrono::high_resolution_clock::time_point serverTimer;		// 서버 기준 시간
	HANDLE g_hiocp;													// handle 선언

	void initServer();
	void err_quit(char *msg);										// Error 나올 경우 Server 종료
	void err_display(char *msg, int err_no);						// Error 표시 해주기

public:
	HANDLE getHandle() { return g_hiocp; }							// Handle Return
	IOCP_Server();													// init IOCP
	~IOCP_Server();													// remove IOCP
};

#endif