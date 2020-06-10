
#ifndef _TIMER_H_
#define _TIMER_H_

#include <chrono>
using namespace std::chrono;

class Timer {
	steady_clock::time_point begin;
	bool is_running;
public:
	Timer() :is_running(false) {
	}
	inline void start() {
		begin = steady_clock::now();
		is_running = true;
	}
	inline double duration()const {
		return is_running ? std::chrono::duration<double>(steady_clock::now() - begin).count() : 0.0;
	}
};
#endif
