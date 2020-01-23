#ifndef __API_H__
#define __API_H__

#include "../Main.h"
// Route Header
#include "L_Auth.h"

class Logic_API {
public:
	bool start();
	bool stop();
	void packet_Add(unsigned __int64 unique_id, char* pData, unsigned short packetLen);
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