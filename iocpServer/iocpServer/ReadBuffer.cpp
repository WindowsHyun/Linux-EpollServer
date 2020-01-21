#include "ReadBuffer.h"

char first_packet{ 0 };

ReadBuffer::ReadBuffer()
{
	totalSize = 0;
	readPos = 0;
}

ReadBuffer::~ReadBuffer()
{
	delete[] buffer;
	buffer = nullptr;
}

void ReadBuffer::init(int size)
{
	std::lock_guard<std::mutex> guard(mLock);
	totalSize = size;
	buffer = new char[size];
	memset(buffer, 0, totalSize);
	readPos = 0;
	writePos = 0;
}

int ReadBuffer::getHeaderSize(char* pData, int size)
{
	std::lock_guard<std::mutex> guard(mLock);

	// 이건 잘못된 거다
	if (totalSize - readPos < size) {
		return -1;
	}
	// Packet_Header Size 만큼 pData(Packet_Header)에 복사한다.
	memcpy_s(pData, size, &buffer[readPos], size);

	return size;
}

// 
bool ReadBuffer::moveWritePos(int size)
{
	std::lock_guard<std::mutex> guard(mLock);

	// totalSize보다 큰 패킷이 올리가 없다.
	if (size > totalSize) {
		spdlog::critical("moveWritePos size({}) > totalSize({})", size, totalSize);
		return false;
	}
	writePos = (writePos + size) % totalSize;
	return true;
}

void ReadBuffer::moveReadPos(int size)
{
	std::lock_guard<std::mutex> guard(mLock);
	readPos = (readPos + size) % totalSize;
}

int ReadBuffer::getReadAbleSize(void)
{
	if (readPos <= writePos) {
		return writePos - readPos;
	}
	else {
		return (totalSize - readPos) + writePos;
	}
}

void ReadBuffer::checkWrite(int size)
{
	std::lock_guard<std::mutex> guard(mLock);

	// 순환
	if (writePos + size >= totalSize)
	{
		memcpy_s(buffer, writePos - readPos,
			&buffer[readPos], writePos - readPos);

		readPos = 0;
		writePos = writePos - readPos;
	}
}