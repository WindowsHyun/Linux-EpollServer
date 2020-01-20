#include "L_Auth.h"

AuthRoute::AuthRoute()
{
}

AuthRoute::~AuthRoute()
{
	std::cout << "~AuthRoute()" << std::endl;
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
		std::cout << "[Error] ApiProcessing ProtocolType( AuthRoute / " << packet.packet_type << " )이 없습니다..!" << std::endl;
	}
	break;
	}
}
