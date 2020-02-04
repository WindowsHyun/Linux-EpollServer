#include "L_Auth.h"

AuthRoute::AuthRoute()
{
	auth = new AuthModule;
}

AuthRoute::~AuthRoute()
{
}

void AuthRoute::ApiProcessing(Packet_Frame packet)
{
	switch (packet.packet_type) {
	case SERVER_AUTH_UNIQUENO:
	{
		sc_packet_unique_no *my_packet = reinterpret_cast<sc_packet_unique_no *>(packet.pMsg);

		Player->set_unique_no(my_packet->unique_no);
		iocp_client.set_unique_no(my_packet->unique_no);
		spdlog::info("[Login Success] user unique no : {}", my_packet->unique_no);
	}
	break;

	default:
	{
		spdlog::error("L_Auth->ApiProcessing ProtocolType ({})is not found..! || [unique_no:{}]", packet.packet_type, packet.unique_no);
	}
	break;
	}
}
