//Dustin Gehm
/*a class used to measure the amount of time that has passed*/

#pragma once
#ifndef _TIMER_H
#define _TIMER_H

#include <ctime>

#include <SDL_timer.h>

#include "Utils.h"

#define TIME_CONVERT_MAX_STR 80

namespace Bingo {

	namespace Time {

		class Timer {
		public:
			Timer() = default;
			~Timer() = default;

			//start the timer
			void start();
			//end the timer and return the number of milliseconds since start was called
			uint end();
			//return the difference between earlier calls to start and end
			uint diff() const;
			//get the current number of milliseconds since program start
			static uint getTicks();

			static inline void DelayMS(uint ms) {
				SDL_Delay(ms);
			}

		private:
			enum TimerState { INIT, IDLE, RUN };

			uint startTicks = 0;
			uint endTicks = 0;
			uint lastDiff = 0;
			TimerState state = INIT;
			};

		class CountDownTimer {
		public:
			CountDownTimer();
			CountDownTimer(const tm& target);
			~CountDownTimer() = default;

			bool isTimeUp();
			void setTime(const struct tm& timeStruct);

			static bool isTimeStr(const string& str, const string& format);
			static tm makeTime(const string& str, const string& format);
		private:
			tm targetTime;
			bool set = false;
			bool timeUp = false;
		};

		typedef CountDownTimer Counter;

		}

	namespace TimeConvert {
		inline time_t struct2Stamp(const tm& timeStruct) {
			return mktime(const_cast<tm*>(&timeStruct));
		}

		inline string struct2String(const tm& timeStruct) {
			char buffer[TIME_CONVERT_MAX_STR] = {};

			asctime_s<TIME_CONVERT_MAX_STR>(buffer, const_cast<tm*>(&timeStruct));

			return string(buffer);
		}

		inline tm stamp2Struct(const time_t& timeStamp) {
#if _MSC_VER
			tm tempTimeStruct;

			localtime_s(&tempTimeStruct, &timeStamp);

			return tempTimeStruct;
#else
			tm* tempTimeStruct;

			tempTimeStruct = localTime(&timeStamp);

			return *tempTimeStruct;
#endif
		}

		inline string stamp2String(const time_t& timeStamp) {
			char buffer[TIME_CONVERT_MAX_STR];
			memset(buffer, 0, sizeof(buffer));

			ctime_s(buffer, TIME_CONVERT_MAX_STR, &timeStamp);

			return string(buffer);
		}

		inline float ms_to_sec(uint milliseconds) {
			return milliseconds / 1000.0f;
		}

		inline uint sec_to_ms(float seconds) {
			return static_cast<uint>(seconds * 1000.0f);
		}

		inline float sec_to_min(uint sec) {
			return sec / 60.0f;
		}

		inline uint min_to_sec(float min) {
			return static_cast<uint>(min * 60.0f);
		}

		inline float min_to_hour(uint min) {
			return min / 60.0f;
		}

		inline uint hour_to_min(float min) {
			return static_cast<uint>(min * 60.0f);
		}
	}

	}

#endif