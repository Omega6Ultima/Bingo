#pragma once
#include "pch.h"
#include "TextSurface.h"

TEST(TestTextSurface, TestConstruct) {
	auto t = Bingo::Surfaces::TextSurface("main", 12, "test", Bingo::Colors::BLACK);
	auto t2 = new Bingo::Surfaces::TextSurface("main", 12, "test", Bingo::Colors::BLACK);

	EXPECT_EQ(t.getText(), "test");
	EXPECT_EQ(t2->getText(), "test");

	EXPECT_EQ(t.getTextPadding(), 0);
	EXPECT_EQ(t2->getTextPadding(), 0);

	EXPECT_EQ(t.getColor(), Bingo::Colors::BLACK);
	EXPECT_EQ(t2->getColor(), Bingo::Colors::BLACK);

	EXPECT_EQ(t.getBackgroundColor(), Bingo::Colors::TRANSPARENT);
	EXPECT_EQ(t2->getBackgroundColor(), Bingo::Colors::TRANSPARENT);

	delete t2;
}

TEST(TestTextSurface, TestSetGetFont) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::RED);

	EXPECT_NO_THROW(t.setFont("other"));

	EXPECT_EQ(t.getFont(), "other");
}

TEST(TestTextSurface, TestSetGetFontSize) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::RED);

	EXPECT_NO_THROW(t.setFontSize(20));

	EXPECT_EQ(t.getFontSize(), 20);
}

TEST(TestTextSurface, TestGetLetterHeightWidth) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::RED);

	EXPECT_EQ(t.getLetterHeight(), 20);

	EXPECT_EQ(t.getLetterWidth('M'), 11);
	EXPECT_EQ(t.getLetterWidth('I'), 2);
	EXPECT_EQ(t.getLetterWidth('L'), 8);
	EXPECT_EQ(t.getLetterWidth('O'), 10);

	EXPECT_EQ(t.getLetterWidth('m'), 13);
	EXPECT_EQ(t.getLetterWidth('i'), 2);
	EXPECT_EQ(t.getLetterWidth('l'), 2);
	EXPECT_EQ(t.getLetterWidth('o'), 9);
}

TEST(TestTextSurface, TestGetTextHeightWidth) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::RED);

	EXPECT_EQ(t.getTextHeight(), 20);

	EXPECT_EQ(t.getTextWidth(), 84);
}

TEST(TestTextSurface, TestSetGetText) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::RED);

	EXPECT_EQ(t.getText(), "Hello World");

	EXPECT_NO_THROW(t.setText("Goodbye World"));

	EXPECT_EQ(t.getText(), "Goodbye World");
}

TEST(TestTextSurface, TestSetGetTextPadding) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::RED);

	EXPECT_EQ(t.getTextPadding(), 0);

	EXPECT_NO_THROW(t.setTextPadding(5));

	EXPECT_EQ(t.getTextPadding(), 5);
}

TEST(TestTextSurface, TestSetGetColor) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::BLUE);

	EXPECT_EQ(t.getColor(), Bingo::Colors::BLUE);

	EXPECT_NO_THROW(t.setColor(Bingo::Colors::AQUA));

	EXPECT_EQ(t.getColor(), Bingo::Colors::AQUA);
}

TEST(TestTextSurface, TestSetGetBackgroundColor) {
	auto t = Bingo::Surfaces::TextSurface("main", 16, "Hello World", Bingo::Colors::GREEN);

	EXPECT_EQ(t.getBackgroundColor(), Bingo::Colors::TRANSPARENT);

	EXPECT_NO_THROW(t.setBackgroundColor(Bingo::Colors::DARKORCHID));

	EXPECT_EQ(t.getBackgroundColor(), Bingo::Colors::DARKORCHID);
}