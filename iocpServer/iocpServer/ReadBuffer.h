#ifndef __READBUFFER_H__
#define __READBUFFER_H__

#include <mutex>
#include <string.h>
#include <spdlog/spdlog.h>

class ReadBuffer {
public:
	ReadBuffer();
	~ReadBuffer();
	void init(int size);
	int getHeaderSize(char* pMsg, int size);
	int setWriteBuffer(char* pMsg, int size);
	char * getReadBuffer(void) { return &buffer[readPos]; }
	char * getWriteBuffer(void) { return &buffer[writePos]; }
	bool moveWritePos(int size);
	void moveReadPos(int size);
	int	getReadAbleSize(void);
	int	getWriteAbleSize(void);
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