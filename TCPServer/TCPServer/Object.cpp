#include "Main.h"

void PLAYER::set_sock(SOCKET g_sock)
{
	this->sock = g_sock;
}

void PLAYER::set_unique_id(unsigned __int64 id)
{
	this->unique_id = id;
}

void PLAYER::set_init_player()
{
	ZeroMemory(&this->m_stRecvOverlappedEx, sizeof(stOverlappedEx));
	ZeroMemory(&this->m_stSendOverlappedEx, sizeof(stOverlappedEx));
	ZeroMemory(&this->nickName, sizeof(char));
	this->sock = INVALID_SOCKET;
	this->unique_id = -1;
	this->connect = false;
	this->live = false;
	this->game_play = false;
}
