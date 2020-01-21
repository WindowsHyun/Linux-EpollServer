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

	// totalSize���� ū ��Ŷ�� �ø��� ����.
	if (size > totalSize) {
		return false;
	}
	//printf("[INFO] writePos : %d, ioSize : %d\n", writePos, size);
	writePos = (writePos + size) % totalSize;
	//printf("[INFO] writePos + size : %d, ��totalSize : %d\n", (writePos + size), (writePos + size) % totalSize);
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

	// ��ȯ
	if (writePos + size >= totalSize)
	{
		printf("[INFO] writePos : %d, readPos : %d, size : %d\n", writePos, readPos , size);
		memcpy_s(buffer, writePos - readPos,
			&buffer[readPos], writePos - readPos);

		readPos = 0;
		writePos = writePos - readPos;
		//printf("Because Packet split and buffer over ... (%d)\n", writePos);
	}
}

void ReadBuffer::show_readWrite()
{
	printf("[INFO] read : %d, write : %d\n", readPos, writePos);
}
