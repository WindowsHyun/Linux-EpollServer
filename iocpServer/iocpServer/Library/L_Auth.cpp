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
		unsigned __int64 olduniqueID = packet.unique_id;

		// Redis 에서 유저 고유 번호를 가져온다.
		result = auth->get_uniqueID(my_packet->sha256sum, uniqueID);

		if (result != true) {
			if (auth->set_uniqueID(my_packet->sha256sum, uniqueID) == false) {
				// 신규유저 UniqueID를 생성 도중 문제가 생겼다...
				break;
			}
		}

		auto pTempPlayerSession = player_session.find(packet.unique_id); // getPlayerSession(pOver->m_unique_id);
		if (pTempPlayerSession == player_session.end()) {
			spdlog::error("[CLIENT_AUTH_LOGIN] No Exit Session || [unique_id:{}]", packet.unique_id);
			break;
		}
		auto pPlayerSession = pTempPlayerSession->second;
		pPlayerSession->set_unique_id(uniqueID);

		// 플레이어를 set 해준다.
		class PLAYER * acceptPlayer = new class PLAYER;
		acceptPlayer->set_sock(pPlayerSession->get_sock());
		acceptPlayer->set_unique_id(uniqueID);
		player.insert(std::unordered_map<unsigned __int64, class PLAYER *>::value_type(uniqueID, acceptPlayer));
		// 기존 플레이어 del 해준다.
		player.erase(olduniqueID);

		// 세션도 변경 처리 한다.
		player_session.insert(std::unordered_map<unsigned __int64, class PLAYER_Session *>::value_type(uniqueID, pPlayerSession));
		player_session.erase(olduniqueID);

		// 사용한 tempUniqueID는 다시 등록을 해준다.
		iocp_server.add_tempUniqueID(olduniqueID);
		// 다시 Recv할 수 있게 변경을 해준다.
		iocp_server.BindRecv(pPlayerSession, pPlayerSession->get_remainSize());

		spdlog::info("[CLIENT_AUTH_LOGIN] Old UniqueID : {} / Changed UniqueID : {} || [unique_id:{}]", olduniqueID, uniqueID, pPlayerSession->get_unique_id());
	}
	break;

	case CLIENT_AUTH_TEST:
	{
		//std::cout << "CLIENT_AUTH_TEST" << " | " << packet.unique_id << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST2:
	{
		//std::cout << "CLIENT_AUTH_TEST2" << " | " << packet.unique_id << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST3:
	{
		//std::cout << "CLIENT_AUTH_TEST3" << " | " << packet.unique_id << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST4:
	{
		//std::cout << "CLIENT_AUTH_TEST4" << " | " << packet.unique_id << std::endl;
	}
	break;

	default:
	{
		spdlog::error("L_Auth->ApiProcessing ProtocolType ({})is not found..! || [unique_id:{}]", packet.packet_type, packet.unique_id);
	}
	break;
	}
}
