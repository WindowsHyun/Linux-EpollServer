#ifndef __RESULTCODE_H__
#define __RESULTCODE_H__

const enum class ResultCode {
	NONE = 1,								// 정상적인 처리 완료
	PASS,									// 결과를 전송할 필요 없다. [ 이미 Library에서 처리를 함 ]

	ERROR_RESULT_CODE_START = 10000,
	NO_EXIT_SESSION,						// 해당 유저의 세션이 존재하지 않는다.
	REDIS_CREATE_USER_ID_FAIL,				// Redis에 해당 유저를 Set하지 못하였다.
};

#endif