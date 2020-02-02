#include "Session.h"

void PLAYER_Session::set_unique_no(const unsigned __int64 id)
{
	unique_no = id;
}

void PLAYER_Session::set_init_session()
{
	ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
	ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
	m_socketSession = INVALID_SOCKET;
	unique_no = 0;
}

void PLAYER_Session::update_error_cnt()
{
	// Call Error += 1
	error_cnt += 1;
}

bool PLAYER_Session::sendReady(char * pMsg, int size)
{
	m_sendBuffer.setWriteBuffer(pMsg, size);
	return true;
}

bool PLAYER_Session::sendIo()
{
	m_stSendOverlappedEx.m_wsaBuf.len = m_sendBuffer.getReadAbleSize();
	m_stSendOverlappedEx.m_wsaBuf.buf = m_sendBuffer.getReadBuffer();
	m_stSendOverlappedEx.m_eOperation = IOOperation::SEND;


	DWORD dwRecvNumBytes = 0;
	int nRet = WSASend(m_socketSession,
		&(m_stSendOverlappedEx.m_wsaBuf),
		1,
		&dwRecvNumBytes,
		0,
		(LPWSAOVERLAPPED) & (m_stSendOverlappedEx),
		NULL);

	if (nRet == SOCKET_ERROR && (WSAGetLastError() != ERROR_IO_PENDING)){
		spdlog::error("WSASend() Function failure : {} || [unique_no:{}]", WSAGetLastError(), unique_no);
		return false;
	}
	return true;

}

bool PLAYER_Session::sendFinish(int size)
{
	m_sendBuffer.moveReadPos(size);
	return true;
}
