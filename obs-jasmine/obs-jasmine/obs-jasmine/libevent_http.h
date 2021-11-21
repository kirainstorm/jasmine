#pragma once
//----------------------------------------
// kirainstorm
// https://github.com/kirainstorm
//----------------------------------------
#include <mutex>
#include <thread>
#include <stdint.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <deque>
#include <time.h>
#include <cassert>
#include <atomic>
#include <cctype>
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <math.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/http.h>
#include <event2/event.h>
#include <event2/thread.h>
#include <event2/bufferevent_compat.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
namespace libevent
{
	using namespace std;
	class http
	{
	public:
		http();
		~http();
	public:
		static http& instance() {
			static http instance;
			return instance;
		}
		void start();
		void stop();
	private:

		struct event_base* base;
		volatile bool thread_exit_;
		std::vector<std::thread> threads_;
		void worker();
	};
}