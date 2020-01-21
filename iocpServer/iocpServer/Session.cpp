#include "Session.h"

void PLAYER_Session::set_unique_id(const unsigned __int64 id)
{
	unique_id = id;
}

void PLAYER_Session::set_init_session()
{
	ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
	ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
	m_socketSession = INVALID_SOCKET;
	unique_id = 0;
}

void PLAYER_Session::set_error_cnt()
{
	// Call Error += 1
	error_cnt += 1;
}
