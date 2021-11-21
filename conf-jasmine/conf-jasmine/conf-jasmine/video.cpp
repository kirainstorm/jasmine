#include "video.hpp"





QMutex Mutex_;
void lockVideoRender()
{
	Mutex_.lock();
}
void unlockVideoRender()
{
	Mutex_.unlock();
}