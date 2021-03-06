﻿#ifndef __API_H__
#define __API_H__

#include "../Main.h"
// Route Header
#include "L_Auth.h"
#include "../Module/M_Auth.h"

class Logic_API {
public:
	bool start();
	bool stop();
	void packet_Add(unsigned_int64 unique_no, char* pMsg, unsigned short packetLen);
	std::mutex& get_mutex() { return mLock; }
	std::queue<Packet_Frame>& get_PacketFrame() { return recvPacketQueue; }
	Logic_API();
	~Logic_API();

private:
	bool threadRun;
	std::thread api_thread;
	std::queue<Packet_Frame> recvPacketQueue;
	std::mutex	mLock;
	void API_Thread();
};



#endif