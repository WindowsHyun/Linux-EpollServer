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
	case CLIENT_AUTH_LOGIN:
	{
		cs_packet_auth *my_packet = reinterpret_cast<cs_packet_auth *>(packet.pData);
		bool result = false;
		unsigned __int64 uniqueID;

		std::cout << my_packet->sha256sum << std::endl;
		result = auth->get_uniqueID(my_packet->sha256sum, uniqueID);

		if (result == true) {
			// 기존 유저
			PLAYER_Session* pPlayerSession = new PLAYER_Session;
			pPlayerSession->set_init_session();

			// session에 set 해준다.
			pPlayerSession->set_unique_id(uniqueID);

			// player_session에 추가 한다.
			player_session.insert(std::unordered_map<unsigned __int64, class PLAYER_Session *>::value_type(uniqueID, pPlayerSession));

			// 플레이어를 set 해준다.
			class PLAYER * acceptPlayer = new class PLAYER;
			acceptPlayer->set_sock(pPlayerSession->get_sock());
			acceptPlayer->set_unique_id(uniqueID);
			player.insert(std::unordered_map<unsigned __int64, class PLAYER *>::value_type(uniqueID, acceptPlayer));

		}
		else {
			// 신규 유저
		}
	}
	break;

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
