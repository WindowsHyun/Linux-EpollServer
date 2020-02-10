#ifndef __M_AUTH_H__
#define __M_AUTH_H__

#include "../Library/L_Auth.h"

class AuthModule {
public:
	bool get_uniqueNo(const char * value, unsigned_int64& uniqueNo);
	bool set_uniqueNo(const char * value, unsigned_int64& uniqueNo);


private:

};


#endif