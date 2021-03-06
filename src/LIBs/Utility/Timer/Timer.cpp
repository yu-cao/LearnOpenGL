#include <Utility/Timer.h>
#include <ctime>

using namespace Ubpa;
using namespace std;


Timer::Timer(size_t maxLogNum)
	: maxLogNum(maxLogNum){
	Reset();
}

bool Timer::Start() {
	switch (state)
	{
	case Ubpa::Timer::ENUM_STATE_INIT:
		wholeTime = 0;
		logs.push_front(0);
		// no break
	case Ubpa::Timer::ENUM_STATE_STOP:
		lastLogTime = GetCurTime();
		state = ENUM_STATE_RUNNING;
		return true;
	default:
		return false;
	}
}

bool Timer::Stop() {
	switch (state)
	{
	case Ubpa::Timer::ENUM_STATE_RUNNING: {
		double curTime = GetCurTime();
		double deltaTime = curTime - lastLogTime;
		wholeTime += deltaTime;
		logs.front() += deltaTime;
		state = ENUM_STATE_STOP;
		return true;
	}
	default:
		return false;
	}
}

double Timer::Log() {
	switch (state)
	{
	case Ubpa::Timer::ENUM_STATE_RUNNING: {
		double curTime = GetCurTime();
		double deltaTime = curTime - lastLogTime;
		wholeTime += deltaTime;
		logs.front() += deltaTime;
		logs.push_front(0);
		if (logs.size() > maxLogNum)
			logs.pop_back();
		lastLogTime = curTime;
		// state no change
		// state = ENUM_STATE_RUNNING;
		return deltaTime;
	}
	default:
		return -1.0;
	}
}

void Timer::Reset() {
	wholeTime = 0;
	logs.clear();
	state = ENUM_STATE_INIT;
}

Timer::ENUM_STATE Timer::GetState() {
	return state;
}

double Timer::GetCurTime() {
	clock_t curTime = clock();
	return static_cast<double>(curTime) / static_cast<double>(CLOCKS_PER_SEC);
}

double Timer::GetWholeTime() {
	return wholeTime;
}

std::ostream & Timer::operator <<(std::ostream & os) {
	os << "Whole Time : " << wholeTime << endl;
	for (size_t i = 0; i < logs.size(); i++) {
		os << "Log[" << i << "] : " << logs[i] << endl;
	}
	return os;
}