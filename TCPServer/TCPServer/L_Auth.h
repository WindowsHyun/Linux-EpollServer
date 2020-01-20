#ifndef __L_AUTH_H__
#define __L_AUTH_H__

#include "Api.h"

class AuthRoute {
public:
	AuthRoute(Packet_Frame packet);
	void ApiProcessing();	// API Ã³¸®

private:
	Packet_Frame pk;

};


#endif