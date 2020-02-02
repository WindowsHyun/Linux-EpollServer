#include "Api.h"

bool Logic_API::start()
{
	threadRun = true;
	api_thread = std::thread([this]() { API_Thread(); });
	return true;
}

bool Logic_API::stop()
{
	threadRun = false;
	if (api_thread.joinable())
	{
		api_thread.join();
	}
	return true;
}

void Logic_API::packet_Add(unsigned __int64 unique_no, char * pMsg, unsigned short packetLen)
{
	// Packet Header Read
	char *packetHeader = new char[packetLen];
	memcpy(packetHeader, pMsg, packetLen);
	auto pHeader = (PACKET_HEADER*)packetHeader;
	PACKET_HEADER packet_header;
	packet_header.packet_type = pHeader->packet_type;
	packet_header.packet_len = pHeader->packet_len;

	// Packet Frame Add
	Packet_Frame packet_frame;
	packet_frame.packet_type = packet_header.packet_type;
	packet_frame.size = packet_header.packet_len;
	packet_frame.pMsg = packetHeader;
	packet_frame.unique_no = unique_no;
	std::lock_guard<std::mutex> guard(mLock);
	api.get_PacketFrame().push(packet_frame);
}

Logic_API::Logic_API()
{
	threadRun = false;
}

Logic_API::~Logic_API()
{
	stop();
}

void Logic_API::API_Thread()
{
	while (threadRun) {
		if (!recvPacketQueue.empty()) {
			// 로직 처리를 진행
			std::lock_guard<std::mutex> guard(mLock);
			sc_packet_result result;
			auto packet = recvPacketQueue.front();
			result.packet_no = packet.packet_type;

			// Protocol Base값 을 가져온다.
			ProtocolType protocolBase = (ProtocolType)((int)packet.packet_type / (int)PACKET_RANG_SIZE * (int)PACKET_RANG_SIZE);

			// 각각의 Library로 처리를 보낸다.
			switch (protocolBase) {

			case CLIENT_BASE:
			{

			}
			break;

			case CLIENT_AUTH_BASE:
			{
				AuthRoute* auth = new AuthRoute();
				auth->ApiProcessing(packet, result);
				delete auth;
			}
			break;

			case CLIENT_FRONT_BASE:
			{

			}
			break;

			case CLIENT_GOODS_BASE:
			{

			}
			break;

			case CLIENT_INFO_BASE:
			{

			}
			break;

			default:
				spdlog::error("ProcessPacket ProtocolType ({} / {})is not found..! || [unique_no:{}]", packet.packet_type, protocolBase, packet.unique_no);
				break;

			}

			// Result Packet 보내기.
			if (result.result != (int)ResultCode::PASS) {
				// PASS처리가 아닌 경우에는 무조건 Client에게 결과를 전송한다.
				result.packet_len = sizeof(result);
				result.packet_type = SERVER_RESULT_PACKET;

				iocp_server.SendPacket(result.unique_no,
					reinterpret_cast<char *>(&result), sizeof(result));
			}
			recvPacketQueue.pop();
			delete[] packet.pMsg;
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}
}
