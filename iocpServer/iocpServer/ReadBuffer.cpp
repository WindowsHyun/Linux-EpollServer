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

int ReadBuffer::getHeaderSize(char* pMsg, int size)
{
	std::lock_guard<std::mutex> guard(mLock);

	// 이건 잘못된 거다
	if (totalSize - readPos < size) {
		return -1;
	}
	// Packet_Header Size 만큼 pMsg(Packet_Header)에 복사한다.
	memcpy_s(pMsg, size, &buffer[readPos], size);

	return size;
}

int ReadBuffer::setWriteBuffer(char * pMsg, int size)
{
	std::lock_guard<std::mutex> guard(mLock);

	// 쓰기 가능한 공간이, size보다 크면 초기화를 해준다.
	if (size > getWriteAbleSize()) {
		memcpy_s(&buffer[0], size, pMsg, size);
		readPos = 0;
		writePos = size;
	}
	else {
		memcpy_s(&buffer[writePos], size, pMsg, size);
		writePos += size;
	}
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

int ReadBuffer::getWriteAbleSize(void)
{
	if (readPos > writePos)
	{
		return readPos - writePos;
	}
	else
	{
		return totalSize - writePos;
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