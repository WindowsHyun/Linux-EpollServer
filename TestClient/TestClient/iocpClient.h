#ifndef __ICOP_H__
#define __ICOP_H__

#include "Main.h"

class IOCP_Client {
public:
	HANDLE& getHandle() { return g_hiocp; }							// Handle Return
	IOCP_Client();													// init IOCP
	~IOCP_Client();													// remove IOCP
	bool SendPacket(char* pMsg, int nLen);							// Packet Send ó���� �Ѵ�.
	bool initClient();
	bool connectServer(const char * ipAddres);
	void destroyThread();
	int& get_remainSize() { return remainSize; }
	ReadBuffer read_buffer;
	ReadBuffer send_buffer;
	

private:
	SOCKET listenSocket;												// Socket
	HANDLE g_hiocp;														// handle ����
	std::thread mIOWorkerThreads;										// IO Worker ������
	unsigned __int64 uniqueNo;											// ���� uniqueNo
	bool mIsWorkerRun;													// �۾� Thread ���� �÷���
	int remainSize;
	int errcnt;
	int testcnt;

	bool CreateWokerThread();											// WorkThread init
	void WokerThread();													// WorkThread
	void OnRecv(struct stOverlappedEx* pOver, int ioSize);							// Recv ó���� ���� �Ѵ�.
	void OnSend(class PLAYER_Session* pSession, int size);
};

#endif