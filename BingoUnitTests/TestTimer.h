#pragma once
#include "pch.h"
#include "Timer.h"

TEST(TestTimer, TestConstruct) {
	auto t = Bingo::Time::Timer();
	auto t2 = new Bingo::Time::Timer();

	EXPECT_EQ(t.end(), 0);
	EXPECT_EQ(t2->end(), 0);

	EXPECT_EQ(t.diff(), 0);
	EXPECT_EQ(t2->diff(), 0);

	delete t2;
}

TEST(TestTimer, TestStartEnd) {
	auto t = Bingo::Time::Timer();

	EXPECT_NO_THROW(t.start());

	Bingo::Time::Timer::DelayMS(1000);

	EXPECT_GE(t.end(), 1000u);
	EXPECT_LT(t.end(), 2000u);

	EXPECT_EQ(t.end(), t.diff());
}

TEST(TestTimer, TestGetTicks) {
	auto t = Bingo::Time::Timer();

	EXPECT_GE(t.getTicks(), 0u);
}

TEST(TestTimer, TestDelay) {
	uint start = Bingo::Time::Timer::getTicks();

	Bingo::Time::Timer::DelayMS(1000);

	uint end = Bingo::Time::Timer::getTicks();

	EXPECT_NEAR(end - start, 1000u, 50u);
}

TEST(TestCounter, TestConstruct) {
	auto t = Bingo::Time::CountDownTimer();
	auto t2 = new Bingo::Time::Counter();

	EXPECT_FALSE(t.isTimeUp());
	EXPECT_FALSE(t2->isTimeUp());
}

TEST(TestCounter, TestConstruct2) {
	auto ts = Bingo::TimeConvert::stamp2Struct(time(NULL));
	auto t = Bingo::Time::Counter(ts);

	EXPECT_TRUE(t.isTimeUp());
}

TEST(TestCounter, TestSetTime) {
	auto t = Bingo::Time::Counter();

	EXPECT_NO_THROW(t.setTime(Bingo::TimeConvert::stamp2Struct(time(NULL))));
}

TEST(TestCounter, TestIsTimeUp) {
	auto t = Bingo::Time::Counter();

	t.setTime(Bingo::TimeConvert::stamp2Struct(time(NULL) + 3));

	EXPECT_FALSE(t.isTimeUp());

	Bingo::Time::Timer::DelayMS(3000);

	EXPECT_TRUE(t.isTimeUp());
}

TEST(TestCounter, TestIsTimeStr) {
	EXPECT_TRUE(Bingo::Time::Counter::isTimeStr("12:01a", "%h:%m%p"));
	EXPECT_TRUE(Bingo::Time::Counter::isTimeStr("1:23p", "%h:%m%p"));
	EXPECT_TRUE(Bingo::Time::Counter::isTimeStr("15:49", "%h:%m%p"));
	EXPECT_TRUE(Bingo::Time::Counter::isTimeStr("1991 01 16", "%y %M %d"));
	EXPECT_TRUE(Bingo::Time::Counter::isTimeStr("2000 06 06", "%y%M%d"));
	EXPECT_TRUE(Bingo::Time::Counter::isTimeStr("2009 12:00a", "%y %h%m%p"));
	EXPECT_TRUE(Bingo::Time::Counter::isTimeStr(Bingo::TimeConvert::stamp2String(time(NULL)), "%w %M %d %h:%m:%s %y"));
}

TEST(TestTimeConvert, TestStructToStamp) {
	auto ts = Bingo::Time::Counter::makeTime("1:23a", "%h:%m%p");

	EXPECT_GE(Bingo::TimeConvert::struct2Stamp(ts), 1673504580);
}

TEST(TestTimeConvert, TestStructToString) {
	auto ts = Bingo::Time::Counter::makeTime("11:52p", "%h:%m%p");

	EXPECT_NO_THROW(std::cout << Bingo::TimeConvert::struct2String(ts) << std::endl);
}

TEST(TestTimeConvert, TestStampToStruct) {
	auto ts = Bingo::TimeConvert::stamp2Struct(time(NULL));

	EXPECT_NO_THROW(std::cout << Bingo::TimeConvert::struct2String(ts) << std::endl);
}

TEST(TestTimeConvert, TestStampToString) {
	EXPECT_NO_THROW(std::cout << Bingo::TimeConvert::stamp2String(time(NULL)) << std::endl);
}

TEST(TestTimeConvert, Test_ms_to_sec) {
	EXPECT_EQ(Bingo::TimeConvert::ms_to_sec(1500), 1.5f);
	EXPECT_EQ(Bingo::TimeConvert::ms_to_sec(333), .333f);
}

TEST(TestTimeConvert, Test_sec_to_ms) {
	EXPECT_EQ(Bingo::TimeConvert::sec_to_ms(4.1f), 4100);
	EXPECT_EQ(Bingo::TimeConvert::sec_to_ms(.25f), 250);
}

TEST(TestTimeConvert, Test_sec_to_min) {
	EXPECT_EQ(Bingo::TimeConvert::sec_to_min(60), 1.0f);
	EXPECT_EQ(Bingo::TimeConvert::sec_to_min(150), 2.5f);
}

TEST(TestTimeConvert, Test_min_to_sec) {
	EXPECT_EQ(Bingo::TimeConvert::min_to_sec(2.2f), 132);
	EXPECT_EQ(Bingo::TimeConvert::min_to_sec(.75f), 45);
}

TEST(TestTimeConvert, Test_min_to_hour) {
	EXPECT_EQ(Bingo::TimeConvert::min_to_hour(90), 1.5f);
	EXPECT_EQ(Bingo::TimeConvert::min_to_hour(12), 0.2f);
}

TEST(TestTimeConvert, Test_hour_to_min) {
	EXPECT_EQ(Bingo::TimeConvert::min_to_sec(1.25f), 75);
	EXPECT_EQ(Bingo::TimeConvert::min_to_sec(.5f), 30);
}