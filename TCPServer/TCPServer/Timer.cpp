#include "Main.h"

void SERVER_Timer::Timer_Thread()
{
	for (;;) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		for (;;) {
			// 큐가 비어있으면 꺼내면 안되니까
			if (0 == timer_queue.size()) {
				break;
			}

			// 여러 이벤트 중에 실행시간이 제일 최근인 이벤트를 실행해야 하므로 우선순위 큐를 만듬
			std::lock_guard<std::mutex> guard(mLock);
			Timer_Event t = timer_queue.top();
			if (t.exec_time > high_resolution_clock::now()) {
				break; // 현재시간보다 크다면, 기다려줌
			}
			timer_queue.pop();

			stOverlappedEx *over = new stOverlappedEx;
			if (T_NormalTime == t.event) {
				over->m_eOperation = IOOperation::IO_EVENT;
				over->m_unique_id = t.object_id;
			}
			PostQueuedCompletionStatus(g_hiocp, 1, t.object_id, &over->m_wsaOverlapped);
		}
	}
}

void SERVER_Timer::setTimerEvent(Timer_Event t)
{
	std::lock_guard<std::mutex> guard(mLock);
	timer_queue.push(t);
}

void SERVER_Timer::startTimer(HANDLE& handle)
{
	g_hiocp = handle;
	timer_tread = std::thread(&SERVER_Timer::Timer_Thread, this);
	timer_tread.join();
}

void SERVER_Timer::destroyTimer()
{
	timer_tread.join();
}

SERVER_Timer::SERVER_Timer()
{
	serverTimer = high_resolution_clock::now();
}

SERVER_Timer::~SERVER_Timer()
{
	timer_tread.join();
}
