#ifndef __SESSION_H__
#define __SESSION_H__

#include "Main.h"
#include "ReadBuffer.h"

class PLAYER_Session {
public:
	PLAYER_Session() {
		m_readBuffer.init(MAX_SOCKBUF);
		m_sendBuffer.init(MAX_SOCKBUF);
		m_socketSession = INVALID_SOCKET;
		unique_no = 0;
		error_cnt = 0;
		remainSize = 0;
	}
	// get
	int& get_sock() { return m_socketSession; }
	ReadBuffer& read_buffer() { return m_readBuffer; }
	ReadBuffer& send_buffer() { return m_sendBuffer; }
	unsigned_int64 get_unique_no() { return unique_no; }
	int get_error_cnt() { return error_cnt; }
	int get_remainSize() { return remainSize; }

	// set
	void set_unique_no(const unsigned_int64 id);
	void set_init_session();
	void update_error_cnt();
	void set_remainSize(const int value){ remainSize = value;}
	void incr_remainSize(const int value) { remainSize += value; }
	bool sendReady(char* pMsg, int size);
	bool sendIo();
	bool sendFinish(int size);

private:
	int			m_socketSession;			// Cliet와 연결되는 소켓
	ReadBuffer		m_readBuffer;			// readBuffet
	ReadBuffer		m_sendBuffer;			// sendBuffet
	unsigned_int64 unique_no;				// 고유 아이디
	int error_cnt;							// 패킷 오류 Count
	int remainSize;							// Auth Login을 위하여
};
#endif