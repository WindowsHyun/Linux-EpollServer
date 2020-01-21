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

void Logic_API::packet_Add(unsigned __int64 unique_id, char * pData, unsigned short packetLen)
{
	// Packet Header Read
	char *packetHeader = new char[packetLen];
	memcpy(packetHeader, pData, packetLen);
	auto pHeader = (PACKET_HEADER*)packetHeader;
	PACKET_HEADER packet_header;
	packet_header.packet_type = pHeader->packet_type;
	packet_header.packet_len = pHeader->packet_len;

	// Packet Frame Add
	Packet_Frame packet_frame;
	packet_frame.packet_type = packet_header.packet_type;
	packet_frame.size = packet_header.packet_len;
	packet_frame.pData = packetHeader;
	packet_frame.unique_id = unique_id;
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
			//std::cout << "[INFO] API Queue : " << recvPacketQueue.size() << std::endl;
			std::lock_guard<std::mutex> guard(mLock);
			auto packet = recvPacketQueue.front();

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
				auth->ApiProcessing(packet);
				//delete[] auth;
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
				spdlog::error("ProcessPacket ProtocolType ({} / {})이 없습니다 / [unique_id:{}]", packet.packet_type, protocolBase, packet.unique_id);
				break;

			}

			recvPacketQueue.pop();
			delete[] packet.pData;


		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}
}
