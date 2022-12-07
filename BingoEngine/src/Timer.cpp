//Dustin Gehm

#include <ctime>
#include <exception>
#include <sstream>

#include "Exception.h"
#include "Timer.h"

using std::istringstream;

using Bingo::Time::CountDownTimer;
using Bingo::Time::Timer;

void Timer::start() {
	state = RUN;
	startTicks = SDL_GetTicks();
}

uint Timer::end() {
	if (state == RUN) {
		state = IDLE;
		endTicks = SDL_GetTicks();
		lastDiff = endTicks - startTicks;
	}

	return lastDiff;
}

uint Timer::diff() const {
	return lastDiff;
}

uint Timer::getTicks() {
	return SDL_GetTicks();
}

bool Timer::everyXSeconds(uint sec) {
	return everyXMillis(sec * 1000);
}

bool Timer::everyXMillis(uint ms) {
	timeAccumulator += end();
	start();

	if (timeAccumulator > ms) {
		timeAccumulator -= ms;
		return true;
	}

	return false;
}

CountDownTimer::CountDownTimer() {
	memset(&targetTime, 0, sizeof(targetTime));
}

CountDownTimer::CountDownTimer(const tm& timeStruct) {
	targetTime = timeStruct;
	set = true;
}

bool CountDownTimer::isTimeUp() {
	if (!timeUp && set) {
		time_t targetTimeStamp = TimeConvert::struct2Stamp(targetTime);
		time_t curTimeStamp = time(NULL);

		double diff = difftime(targetTimeStamp, curTimeStamp);

		if (-1 < diff && diff < 1) {
			timeUp = true;
			return true;
		}
	}

	return false;
}

void CountDownTimer::setTime(const struct tm& timeStruct) {
	targetTime = timeStruct;
	set = true;
	timeUp = false;
}

bool CountDownTimer::isTimeStr(const string& str, const string& format) {
	try {
		makeTime(str, format);
	}
	catch (Exception&) {
		return false;
	}

	return true;
}

tm CountDownTimer::makeTime(const string& str, const string& format) {
	if (str.empty() || format.empty()) {
		throw Exception("Time string or format is blank");
	}

	const string separators = " :.,/\\";

	istringstream tss(str);
	istringstream fss(format);
	tm readTime = TimeConvert::stamp2Struct(time(NULL));
	char period = 'a';

	//assume seconds is 00
	readTime.tm_sec = 0;

	while (!fss.eof()) {
		char nextCh = fss.get();

		if (nextCh == '%') {
			nextCh = fss.get();

			while (separators.find(tss.peek()) != string::npos) {
				tss.ignore(1);
			}

			switch (nextCh) {
			case 'p':
				tss >> period;

				//if failed to read period, just assume am
				if (tss.fail()) {
					tss.clear();
				}

				break;
			case 's':
				tss >> readTime.tm_sec;
				break;
			case 'm':
				tss >> readTime.tm_min;
				break;
			case 'h':
				tss >> readTime.tm_hour;
				break;
			case 'd':
				tss >> readTime.tm_mday;
				break;
			case 'M':
				tss >> readTime.tm_mon;
				break;
			case 'y':
				tss >> readTime.tm_year;
				break;
			case 'w':
				tss >> readTime.tm_wday;
				break;
			case 'D':
				tss >> readTime.tm_yday;
				break;
			case 'S':
				tss >> readTime.tm_isdst;
				break;
			default:
				throw Exception("Invalid specifier in format string");
				break;
			}

			if (tss.fail()) {
				throw Exception("Invalid value in time string");
			}
		}
	}

	if (tolower(period) == 'p') {
		readTime.tm_hour += 12;
	}

	return readTime;
}