#ifndef __READBUFFER_H__
#define __READBUFFER_H__

#include <mutex>
#define MAX_SOCKBUF	 1024		// 최대 패킷 사이즈

class ReadBuffer {
public:
	ReadBuffer();
	~ReadBuffer();
	void init(int size);
	int getHeaderSize(char* pData, int size);
	char * getReadBuffer(void) { return &buffer[readPos]; }
	bool moveWritePos(int size);
	void moveReadPos(int size);
	int	getReadAbleSize(void);

private:
	char* buffer;
	int totalSize = 0;
	int readPos = 0;
	int writePos = 0;
	std::mutex	mLock;
};

#endif