#ifndef __RESULTCODE_H__
#define __RESULTCODE_H__

enum class ResultCode {
	NONE = 1,								// �������� ó�� �Ϸ�

	ERROR_RESULT_CODE_START = 10000,
	NO_EXIT_SESSION,						// �ش� ������ ������ �������� �ʴ´�.
	REDIS_CREATE_USER_ID_FAIL,				// Redis�� �ش� ������ Set���� ���Ͽ���.
};

#endif