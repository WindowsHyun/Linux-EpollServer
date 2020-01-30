#include "M_Auth.h"

bool AuthModule::get_uniqueID(const char * value, unsigned __int64& uniqueID)
{
	std::string result;
	std::string RDCKey = "Data:";
	RDCKey += value;

	RDC[REDISDB::REDIS_USER_AUTH_DB]->get(RDCKey, result);
	if (result != "") {
		// 존재하는 유저
		uniqueID = atoi(result.c_str());
		return true;
	}
	else {
		return false;
	}

}
