#ifndef __SESSION_H__
#define __SESSION_H__

enum class IOOperation {
	RECV,
	SEND
};

// WSAOVERLAPPED����ü�� Ȯ�� ���Ѽ� �ʿ��� ������ �� �־���.
struct stOverlappedEx {
	WSAOVERLAPPED			m_wsaOverlapped;			// Overlapped I/O����ü
	SOCKET					m_socketSession;				// Ŭ���̾�Ʈ ����
	WSABUF					m_wsaBuf;					// Overlapped I/O�۾� ����
	char					m_szBuf[MAX_SOCKBUF];		// ������ ����
	IOOperation				m_eOperation;				// �۾� ���� ����
};

class PLAYER_Session {
public:
	SOCKET			m_socketSession;			// Cliet�� ����Ǵ� ����
	PLAYER_Session()
	{
		ZeroMemory(&m_stRecvOverlappedEx, sizeof(stOverlappedEx));
		ZeroMemory(&m_stSendOverlappedEx, sizeof(stOverlappedEx));
		m_socketSession = INVALID_SOCKET;
		unique_id = 0;
	}
	// get
	unsigned __int64 get_unique_id() { return unique_id; }
	stOverlappedEx& get_Recv_over() { return m_stRecvOverlappedEx; }
	stOverlappedEx& get_Send_over() { return m_stSendOverlappedEx; }

	// set
	void set_unique_id(const unsigned __int64 id);
	void set_init_session();

private:
	stOverlappedEx	m_stRecvOverlappedEx;	// RECV Overlapped I/O�۾��� ���� ����
	stOverlappedEx	m_stSendOverlappedEx;	// SEND Overlapped I/O�۾��� ���� ����
	unsigned __int64 unique_id;				// ���� ���̵�
};
#endif