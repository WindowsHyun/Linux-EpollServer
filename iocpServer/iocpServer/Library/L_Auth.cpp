#include "L_Auth.h"

AuthRoute::AuthRoute()
{
	auth = new AuthModule;
}

AuthRoute::~AuthRoute()
{
}

void AuthRoute::ApiProcessing(Packet_Frame packet, sc_packet_result& resultCode)
{
	switch (packet.packet_type) {
	case CLIENT_AUTH_LOGIN:
	{
		cs_packet_auth *my_packet = reinterpret_cast<cs_packet_auth *>(packet.pMsg);
		bool result = false;
		unsigned __int64 uniqueNo;
		unsigned __int64 olduniqueNo = packet.unique_no;

		// Redis 에서 유저 고유 번호를 가져온다.
		result = auth->get_uniqueNo(my_packet->sha256sum, uniqueNo);

		if (result != true) {
			if (auth->set_uniqueNo(my_packet->sha256sum, uniqueNo) == false) {
				// 신규유저 uniqueNo를 생성 도중 문제가 생겼다...
				resultCode.result = (int)ResultCode::REDIS_CREATE_USER_ID_FAIL;
				break;
			}
		}

		// 유저 세션을 찾아온다.
		auto pPlayerSession = iocp_server.getSessionByNo(packet.unique_no); 
		// 세션이 없을 경우 break 처리
		if (pPlayerSession == nullptr) break;
		pPlayerSession->set_unique_no(uniqueNo);

		// 플레이어를 set 해준다.
		class PLAYER * acceptPlayer = new class PLAYER;
		acceptPlayer->set_sock(pPlayerSession->get_sock());
		acceptPlayer->set_unique_no(uniqueNo);
		player.insert(std::unordered_map<unsigned __int64, class PLAYER *>::value_type(uniqueNo, acceptPlayer));
		// 기존 플레이어 del 해준다.
		player.erase(olduniqueNo);

		// 세션도 변경 처리 한다.
		player_session.insert(std::unordered_map<unsigned __int64, class PLAYER_Session *>::value_type(uniqueNo, pPlayerSession));
		player_session.erase(olduniqueNo);

		// 사용한 tempUniqueNo는 다시 등록을 해준다.
		iocp_server.add_tempUniqueNo(olduniqueNo);
		// 다시 Recv할 수 있게 변경을 해준다.
		iocp_server.BindRecv(pPlayerSession, pPlayerSession->get_remainSize());

		resultCode.result = (int)ResultCode::NONE;
		resultCode.unique_no = uniqueNo;
		spdlog::info("[CLIENT_AUTH_LOGIN] Old uniqueNo : {} / Changed uniqueNo : {} || [unique_no:{}]", olduniqueNo, uniqueNo, pPlayerSession->get_unique_no());
	}
	break;

	case CLIENT_AUTH_TEST:
	{
		//std::cout << "CLIENT_AUTH_TEST" << " | " << packet.unique_no << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST2:
	{
		//std::cout << "CLIENT_AUTH_TEST2" << " | " << packet.unique_no << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST3:
	{
		//std::cout << "CLIENT_AUTH_TEST3" << " | " << packet.unique_no << std::endl;
	}
	break;

	case CLIENT_AUTH_TEST4:
	{
		//std::cout << "CLIENT_AUTH_TEST4" << " | " << packet.unique_no << std::endl;
	}
	break;

	default:
	{
		spdlog::error("L_Auth->ApiProcessing ProtocolType ({})is not found..! || [unique_no:{}]", packet.packet_type, packet.unique_no);
	}
	break;
	}
}
