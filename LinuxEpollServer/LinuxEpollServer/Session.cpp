#include "Session.h"

void PLAYER_Session::set_unique_no(const unsigned_int64 id)
{
	unique_no = id;
}

void PLAYER_Session::set_init_session()
{
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

}

bool PLAYER_Session::sendFinish(int size)
{
	m_sendBuffer.moveReadPos(size);
	return true;
}
