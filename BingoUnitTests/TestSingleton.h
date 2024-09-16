#pragma once
#include "pch.h"
#include "Singleton.h"

class _TestSingleton : public Bingo::Singleton<_TestSingleton> {
	//
};

TEST(TestSingleton, TestConstruct) {
	EXPECT_NO_THROW(_TestSingleton());
	EXPECT_ANY_THROW(_TestSingleton());
}

TEST(TestSingleton, TestGetSingleton) {
	EXPECT_NO_THROW(_TestSingleton::getSingleton());
}

TEST(TestSingleton, TestGetSingletonPtr) {
	EXPECT_NO_THROW(_TestSingleton::getSingletonPtr());
}

TEST(TestSingleton, TestGetSingletonPtr2) {
	EXPECT_EQ((_TestSingleton::getSingletonPtr()), &_TestSingleton::getSingleton());
}