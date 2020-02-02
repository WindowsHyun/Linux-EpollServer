#include "M_Auth.h"

bool AuthModule::get_uniqueNo(const char * value, unsigned __int64& uniqueNo)
{
	std::string result;
	std::string RDCKey = "Data:";
	RDCKey += value;

	RDC[REDISDB::REDIS_USER_AUTH_DB]->get(RDCKey, result);
	if (result != "") {
		// 존재하는 유저
		uniqueNo = atoi(result.c_str());
		return true;
	}
	else {
		return false;
	}

}

bool AuthModule::set_uniqueNo(const char * value, unsigned __int64& uniqueNo)
{
	bool result;
	std::string stValue;
	// Unique No 가져오기
	result = RDC[REDISDB::REDIS_USER_AUTH_DB]->incr("UNIQUE_NO", stValue);
	if (result != true) {
		spdlog::error("[M_Auth] set_uniqueNo incr key : {}", "UNIQUE_NO");
		return false;
	}
	uniqueNo = atoi(stValue.c_str());

	// set으로 해당 데이터 입력
	std::string Key = "Data:";
	Key += value;
	result = RDC[REDISDB::REDIS_USER_AUTH_DB]->set(Key, stValue);
	if (result != true) {
		spdlog::error("[M_Auth] set_uniqueNo set key : {}, {}", Key, stValue);
		return false;
	}

	return true;
}
