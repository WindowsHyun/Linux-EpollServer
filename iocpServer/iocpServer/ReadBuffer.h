#ifndef __READBUFFER_H__
#define __READBUFFER_H__

#include <mutex>
#include "spdlog/spdlog.h"

class ReadBuffer {
public:
	ReadBuffer();
	~ReadBuffer();
	void init(int size);
	int getHeaderSize(char* pData, int size);
	char * getReadBuffer(void) { return &buffer[readPos]; }
	char * getWriteBuffer(void) { return &buffer[writePos]; }
	bool moveWritePos(int size);
	void moveReadPos(int size);
	int	getReadAbleSize(void);
	void checkWrite(int size);
	int getReadPos() { return readPos; }
	int getWritePos() { return writePos; }

private:
	char* buffer;
	int totalSize = 0;
	int readPos = 0;
	int writePos = 0;
	std::mutex	mLock;
};

#endif