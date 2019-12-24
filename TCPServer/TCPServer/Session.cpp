#include "Main.h"

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