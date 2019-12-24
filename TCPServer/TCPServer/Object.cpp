#include "Main.h"

void PLAYER::set_sock(const SOCKET g_sock)
{
	sock = g_sock;
}

void PLAYER::set_unique_id(const unsigned __int64 id)
{
	unique_id = id;
}

void PLAYER::set_init_player()
{
	ZeroMemory(&nickName, sizeof(char));
	sock = INVALID_SOCKET;
	unique_id = 0;
	connect = false;
	live = false;
	game_play = false;
}
