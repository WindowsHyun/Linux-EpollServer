#include "L_Auth.h"

AuthRoute::AuthRoute()
{
}

AuthRoute::~AuthRoute()
{
}

void AuthRoute::ApiProcessing(Packet_Frame packet)
{
	switch (packet.packet_type) {
	case CLIENT_AUTH_TEST:
	{
		//std::cout << "CLIENT_AUTH_TEST" << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST2:
	{
		//std::cout << "CLIENT_AUTH_TEST2" << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST3:
	{
		//std::cout << "CLIENT_AUTH_TEST3" << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST4:
	{
		//std::cout << "CLIENT_AUTH_TEST4" << std::endl;
	}
	break;

	default:
	{
		spdlog::error("L_Auth->ApiProcessing ProtocolType ({})is not found..! / [unique_id:{}]", packet.packet_type, packet.unique_id);
	}
	break;
	}
}
