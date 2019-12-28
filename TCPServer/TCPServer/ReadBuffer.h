#ifndef __READBUFFER_H__
#define __READBUFFER_H__

#include <mutex>
#define MAX_SOCKBUF	 1024		// 최대 패킷 사이즈

class ReadBuffer {
public:
	ReadBuffer();
	~ReadBuffer();
	void init(int size);
	char * getHeaderPacket();
	char * getReadBuffer(void) { return &buffer[readPos]; }

private:
	char* buffer;
	int totalSize = 0;
	int readPos = 0;
	std::mutex	mLock;
};

#endif