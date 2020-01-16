#include "ReadBuffer.h"

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

	// �̰� �߸��� �Ŵ�
	if (totalSize - readPos < size) {
		return -1;
	}
	// Packet_Header Size ��ŭ pData(Packet_Header)�� �����Ѵ�.
	memcpy_s(pData, size, &buffer[readPos], size);
	return size;
}

// 
bool ReadBuffer::moveWritePos(int size)
{
	std::lock_guard<std::mutex> guard(mLock);

	// ������ �߸� �Ȱ� 
	if (size > totalSize) {
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
