#include "Main.h"

int main() {
	printf("Client Start Packet No : %d\n", CLIENT_BASE);
	printf("Max Client Packet No : %d\n", MAX_CLIENT_PROTOCOL_NO);
	printf("Server Start Packet No : %d\n", SERVER_BASE);
	printf("Max Server Packet No : %d\n\n", MAX_SERVER_PROTOCOL_NO);

	IOCP_Server iocp_server;
}