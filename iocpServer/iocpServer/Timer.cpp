#include "Timer.h"

void SERVER_Timer::Timer_Thread()
{
	while (threadRun) {
		if (!timer_queue.empty()) {
			// 여러 이벤트 중에 실행시간이 제일 최근인 이벤트를 실행해야 하므로 우선순위 큐를 만듬
			std::lock_guard<std::mutex> guard(mLock);
			Timer_Event t = timer_queue.top();
			if (t.exec_time > high_resolution_clock::now()) {
				continue; // 현재시간보다 크다면, 기다려줌
			}
			timer_queue.pop();

			stOverlappedEx *over = new stOverlappedEx;
			switch (t.event) {
			case T_NormalTime:
			{
				over->m_eOperation = IOOperation::IO_EVENT;
				over->m_unique_id = t.object_id;
			}
			break;

			default:
			{
				// 예외 상황
				spdlog::critical("[Exception TimerThread({})] No value defined..! || [unique_id:{}]",
					(int)t.event, t.object_id);
				continue;
			}
			break;
			}
			PostQueuedCompletionStatus(g_hiocp, 1, t.object_id, &over->m_wsaOverlapped);
		}
		else {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
}

void SERVER_Timer::setTimerEvent(Timer_Event t)
{
	std::lock_guard<std::mutex> guard(mLock);
	timer_queue.push(t);
}

bool SERVER_Timer::start(HANDLE& handle)
{
	g_hiocp = handle;
	threadRun = true;
	timer_thread = std::thread([this]() { Timer_Thread(); });
	return true;
}

bool SERVER_Timer::stop()
{
	threadRun = false;
	timer_thread.join();

	return true;
}

SERVER_Timer::SERVER_Timer()
{
	threadRun = false;
	serverTimer = high_resolution_clock::now();
}

SERVER_Timer::~SERVER_Timer()
{
	stop();
}
