#include "L_Auth.h"

AuthRoute::AuthRoute(Packet_Frame packet)
{
	pk = packet;
}

void AuthRoute::ApiProcessing()
{
	switch (pk.packet_type) {
	case CLIENT_AUTH_TEST:
		std::cout << "CLIENT_AUTH_TEST" << std::endl;
		break;

	case CLIENT_AUTH_TEST2:
		std::cout << "CLIENT_AUTH_TEST2" << std::endl;
		break;

	case CLIENT_AUTH_TEST3:
		std::cout << "CLIENT_AUTH_TEST3" << std::endl;
		break;

	case CLIENT_AUTH_TEST4:
		std::cout << "CLIENT_AUTH_TEST4" << std::endl;
		break;

	default:
		std::cout << "[Error] ApiProcessing ProtocolType( AuthRoute / " << pk.packet_type << " )�� �����ϴ�..!" << std::endl;
		break;
	}
}
