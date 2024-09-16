#pragma once
#include "pch.h"
#include "Utils.h"
#include "VecN.h"

TEST(TestUtils, TestGetTypeName) {

	EXPECT_EQ(Bingo::Utils::GetTypeName(short), "short");
	EXPECT_EQ(Bingo::Utils::GetTypeName(int), "int");
	EXPECT_EQ(Bingo::Utils::GetTypeName(float), "float");
	EXPECT_EQ(Bingo::Utils::GetTypeName(double), "double");
	EXPECT_EQ(Bingo::Utils::GetTypeName(std::vector<int>), "std::vector");

	// If a type uses multiple template parameters, the comma will throw off the macro
	{
		//Both of these will fail

		//string VecN_name = Bingo::Utils::GetTypeName(Bingo::Math::VecN<int, 2>);
		//EXPECT_EQ(VecN_name, "VecN");
		
		//EXPECT_EQ(Bingo::Utils::GetTypeName(Bingo::Math::VecN<int, 2>), "VecN");
	}

	EXPECT_EQ(Bingo::Utils::GetTypeName(Bingo::Math::DynVecN<int>), "Bingo::Math::DynVecN");
}

TEST(TestUtils, TestGetTypeNameFromVar) {
	short a = 1;
	int b = 1;
	float c = 1.0f;
	double d = 1.0;
	std::vector<int> e = std::vector<int>();
	auto f = Bingo::Math::VecN<int, 2>({ 1, 2 });
	auto g = Bingo::Math::DynVecN<int>();

	EXPECT_EQ(Bingo::Utils::GetTypeNameFromVar(a), "short");
	EXPECT_EQ(Bingo::Utils::GetTypeNameFromVar(b), "int");
	EXPECT_EQ(Bingo::Utils::GetTypeNameFromVar(c), "float");
	EXPECT_EQ(Bingo::Utils::GetTypeNameFromVar(d), "double");
	EXPECT_EQ(Bingo::Utils::GetTypeNameFromVar(e), "std::vector");
	EXPECT_EQ(Bingo::Utils::GetTypeNameFromVar(f), "Bingo::Math::VecN");
	EXPECT_EQ(Bingo::Utils::GetTypeNameFromVar(g), "Bingo::Math::DynVecN");
}

TEST(TestUtils, TestRound) {
	EXPECT_EQ(Bingo::Utils::round(3.14159f, 2), 3.14f);
}

TEST(TestUtils, TestRound2) {
	EXPECT_EQ(Bingo::Utils::round(1.5, 0), 2);
}

TEST(TestUtils, TestDegToRad) {
	EXPECT_EQ(Bingo::Utils::degToRadians(90), PI / 2.0);
}

TEST(TestUtils, TestDegToRad2) {
	EXPECT_EQ(Bingo::Utils::degToRadians(135), (PI / 4.0) * 3.0);
}

TEST(TestUtils, TestRadToDeg) {
	EXPECT_EQ(Bingo::Utils::radToDegrees(PI / 2.0), 90);
}

TEST(TestUtils, TestRadToDeg2) {
	EXPECT_EQ(Bingo::Utils::radToDegrees((PI / 4.0) * 3.0), 135);
}

TEST(TestUtils, TestTabs) {
	EXPECT_EQ(Bingo::Utils::tabs(1), "\t");
}

TEST(TestUtils, TestTabs3) {
	EXPECT_EQ(Bingo::Utils::tabs(5), "\t\t\t\t\t");
}

TEST(TestUtils, TestTokenize) {
	string tokenStr = "123abc<>456def()";

	Bingo::Utils::tokenize(tokenStr);

	EXPECT_EQ(tokenStr, "123 abc <> 456 def ()");
}

TEST(TestUtils, TestTokenize2) {
	string tokenStr = "123abc<>456def()";
	std::vector<uint> tokenizeResults;

	tokenizeResults.push_back(3);
	tokenizeResults.push_back(6);
	tokenizeResults.push_back(8);
	tokenizeResults.push_back(11);
	tokenizeResults.push_back(14);

	EXPECT_EQ(Bingo::Utils::tokenize(tokenStr, Bingo::Utils::TokenizeType::RetVec), tokenizeResults);
}

TEST(TestUtils, TestL_Strip) {
	EXPECT_EQ(Bingo::Utils::lstrip("   testing   "), "testing   ");
}

TEST(TestUtils, TestL_Strip2) {
	EXPECT_EQ(Bingo::Utils::lstrip("123   456"), "123   456");
}

TEST(TestUtils, TestR_Strip) {
	EXPECT_EQ(Bingo::Utils::rstrip("   testing   "), "   testing");
}

TEST(TestUtils, TestR_Strip2) {
	EXPECT_EQ(Bingo::Utils::rstrip("123   456"), "123   456");
}

TEST(TestUtils, TestStrip) {
	EXPECT_EQ(Bingo::Utils::strip("   testing   "), "testing");
}

TEST(TestUtils, TestStrip2) {
	EXPECT_EQ(Bingo::Utils::strip("123   456"), "123   456");
}

TEST(TestUtils, TestSplit) {
	std::vector<string> splitResult;

	splitResult.push_back("honk");
	splitResult.push_back("honk");
	splitResult.push_back("honk");
	splitResult.push_back("honk");

	EXPECT_EQ(Bingo::Utils::split("honk honk honk honk"), splitResult);
}

TEST(TestUtils, TestSplit2) {
	std::vector<string> splitResult;

	splitResult.push_back("foo");
	splitResult.push_back("bar");
	splitResult.push_back("baz");

	EXPECT_EQ(Bingo::Utils::split("foo,bar,baz", ","), splitResult);
}

TEST(TestUtils, TestSplit3) {
	std::vector<string> splitResult;

	splitResult.push_back("thor");
	splitResult.push_back("is");
	splitResult.push_back("awesome");

	EXPECT_EQ(Bingo::Utils::split("thor777is777awesome", "777"), splitResult);
}

TEST(TestUtils, TestSplit4) {
	std::vector<string> splitResult;

	splitResult.push_back("1");
	splitResult.push_back("");
	splitResult.push_back("3");
	splitResult.push_back("4");
	splitResult.push_back("");
	splitResult.push_back("6");

	EXPECT_EQ(Bingo::Utils::split("1//3/4//6", "/"), splitResult);
}

TEST(TestUtils, Test_SDL_PointEq) {
	auto p1 = SDL_Point{ 4, -8 };
	auto p2 = SDL_Point{ 4, -8 };

	EXPECT_TRUE(Bingo::Utils::operator==(p1, p2));
}

TEST(TestUtils, TestStrAppendInt) {
	EXPECT_EQ(Bingo::Utils::operator+(string("test"), -123), "test-123");
}

TEST(TestUtils, TestStrAppendInt2) {
	EXPECT_EQ(Bingo::Utils::operator+(string("test"), 456), "test456");
}

TEST(TestUtils, TestStringIsSpace) {
	EXPECT_TRUE(Bingo::Utils::isSpace(" "));
	EXPECT_TRUE(Bingo::Utils::isSpace("\t"));
	EXPECT_TRUE(Bingo::Utils::isSpace("\n"));
	EXPECT_TRUE(Bingo::Utils::isSpace("\t \n"));

	EXPECT_FALSE(Bingo::Utils::isSpace(""));
	EXPECT_FALSE(Bingo::Utils::isSpace(" abc"));
	EXPECT_FALSE(Bingo::Utils::isSpace("abc "));
	EXPECT_FALSE(Bingo::Utils::isSpace("   123"));
	EXPECT_FALSE(Bingo::Utils::isSpace("456   "));
}

TEST(TestUtils, TestStringIsAlpha) {
	EXPECT_TRUE(Bingo::Utils::isAlpha("abc"));
	EXPECT_TRUE(Bingo::Utils::isAlpha("xyz"));

	EXPECT_FALSE(Bingo::Utils::isAlpha(""));
	EXPECT_FALSE(Bingo::Utils::isAlpha(" abc"));
	EXPECT_FALSE(Bingo::Utils::isAlpha("abc "));
	EXPECT_FALSE(Bingo::Utils::isAlpha("   123"));
	EXPECT_FALSE(Bingo::Utils::isAlpha("456   "));
}

TEST(TestUtils, TestStringIsDigit) {
	EXPECT_TRUE(Bingo::Utils::isDigit("012"));
	EXPECT_TRUE(Bingo::Utils::isDigit("789"));

	EXPECT_FALSE(Bingo::Utils::isDigit(""));
	EXPECT_FALSE(Bingo::Utils::isDigit(" abc"));
	EXPECT_FALSE(Bingo::Utils::isDigit("abc "));
	EXPECT_FALSE(Bingo::Utils::isDigit("   123"));
	EXPECT_FALSE(Bingo::Utils::isDigit("456   "));
}

TEST(TestUtils, TestStringIsAlphaNum) {
	EXPECT_TRUE(Bingo::Utils::isAlphaNum("abc123"));
	EXPECT_TRUE(Bingo::Utils::isAlphaNum("xyz789"));

	EXPECT_FALSE(Bingo::Utils::isAlphaNum(""));
	EXPECT_FALSE(Bingo::Utils::isAlphaNum(" abc"));
	EXPECT_FALSE(Bingo::Utils::isAlphaNum("abc "));
	EXPECT_FALSE(Bingo::Utils::isAlphaNum("   123"));
	EXPECT_FALSE(Bingo::Utils::isAlphaNum("456   "));
}

TEST(TestUtils, TestToLower) {
	EXPECT_EQ(Bingo::Utils::toLower("AaBbCc"), "aabbcc");
	EXPECT_EQ(Bingo::Utils::toLower("ABC123"), "abc123");
	EXPECT_EQ(Bingo::Utils::toLower("xyz "), "xyz ");
}

TEST(TestUtils, TestToUpper) {
	EXPECT_EQ(Bingo::Utils::toUpper("AaBbCc"), "AABBCC");
	EXPECT_EQ(Bingo::Utils::toUpper("ABC123"), "ABC123");
	EXPECT_EQ(Bingo::Utils::toUpper("xyz "), "XYZ ");
}

