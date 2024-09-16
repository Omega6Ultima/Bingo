#pragma once
#include "pch.h"
#include "NBT.h"

TEST(TestNBT_Tag, TestConstruct) {
	auto nbt = Bingo::NBT_Tag<float>("name", 12.5f);
	auto nbt2 = new Bingo::NBT_Tag<float>("name", 12.5f);

	EXPECT_EQ(nbt.getType(), Bingo::NBT_Base::NBT_FLOAT);
	EXPECT_EQ(nbt.getName(), "name");
	EXPECT_EQ(nbt.getData(), 12.5f);

	EXPECT_EQ(nbt2->getType(), Bingo::NBT_Base::NBT_FLOAT);
	EXPECT_EQ(nbt2->getName(), "name");
	EXPECT_EQ(nbt2->getData(), 12.5f);

	delete nbt2;
}

TEST(TestNBT_Tag, TestType) {
	auto nbt = Bingo::NBT_Tag<int>("test", 10);
	auto nbt2 = Bingo::NBT_Tag<Bingo::Math::VecN<int, 3>>("test", Bingo::Math::VecN<int, 3>({ 1, 2, 3 }));

	EXPECT_EQ(nbt.getType(), Bingo::NBT_Base::NBT_INT);
	EXPECT_EQ(nbt2.getType(), Bingo::NBT_Base::NBT_INT_ARRAY);
}

TEST(TestNBT_Tag, TestType2) {
	auto nbt = Bingo::NBT_Tag<double>("test", 42.1);

	EXPECT_EQ(nbt.getType(), Bingo::NBT_Base::NBT_DOUBLE);
}

TEST(TestNBT_Tag, TestType3) {
	auto nbt = Bingo::NBT_Tag<string>("test", "hello world");

	EXPECT_EQ(nbt.getType(), Bingo::NBT_Base::NBT_STRING);
}

TEST(TestNBT_Tag, TestData) {
	auto nbt = Bingo::NBT_Tag<int>("test", 10);

	EXPECT_EQ(nbt.getData(), 10);
}

TEST(TestNBT_Tag, TestData2) {
	auto nbt = Bingo::NBT_Tag<double>("test", 42.1);

	EXPECT_EQ(nbt.getData(), 42.1);
}

TEST(TestNBT_Tag, TestData3) {
	auto nbt = Bingo::NBT_Tag<string>("test", "hello world");

	EXPECT_EQ(nbt.getData(), "hello world");
}

TEST(TestNBT_Tag, TestSetData) {
	auto nbt = Bingo::NBT_Tag<int>("test", 10);

	EXPECT_NO_THROW(nbt.setData(20));

	EXPECT_EQ(nbt.getData(), 20);
}

TEST(TestNBT_Tag, TestSetData2) {
	auto nbt = Bingo::NBT_Tag<double>("test", 42.1);

	EXPECT_NO_THROW(nbt.setData(21.05));

	EXPECT_EQ(nbt.getData(), 21.05);
}

TEST(TestNBT_Tag, TestSetData3) {
	auto nbt = Bingo::NBT_Tag<string>("test", "hello world");

	EXPECT_NO_THROW(nbt.setData("Anteater-eating Ant"));

	EXPECT_EQ(nbt.getData(), "Anteater-eating Ant");
}

TEST(TestNBT_Tag, TestToString) {
	auto nbt = Bingo::NBT_Tag<int>("test", 10);

	EXPECT_EQ(nbt.toString(), "NBT_Tag{test:10}");
}

TEST(TestNBT_Tag, TestToString2) {
	auto nbt = Bingo::NBT_Tag<double>("test", 42.1);

	EXPECT_EQ(nbt.toString(), "NBT_Tag{test:42.1}");
}

TEST(TestNBT_Tag, TestToString3) {
	auto nbt = Bingo::NBT_Tag<string>("test", "hello world");

	EXPECT_EQ(nbt.toString(), "NBT_Tag{test:hello world}");
}

TEST(TestNBT_Compound, TestConstruct) {
	auto nbt = Bingo::NBT_Compound("name");
	auto nbt2 = new Bingo::NBT_Compound("name");

	EXPECT_EQ(nbt.getType(), Bingo::NBT_Base::NBT_COMPOUND);
	EXPECT_EQ(nbt.getName(), "name");

	EXPECT_EQ(nbt2->getType(), Bingo::NBT_Base::NBT_COMPOUND);
	EXPECT_EQ(nbt2->getName(), "name");

	delete nbt2;
}

TEST(TestNBT_Compound, TestGetType) {
	auto nbt = Bingo::NBT_Compound("test compound");

	EXPECT_EQ(nbt.getType(), Bingo::NBT_Base::NBT_COMPOUND);
}

TEST(TestNBT_Compound, TestSetGetTag) {
	auto nbt = Bingo::NBT_Compound("test compound");
	nbt.setTag(new Bingo::NBT_Tag<int>("score", 100));
	nbt.setTag(new Bingo::NBT_Tag<void*>("darkness", (void*)0xffffffff));

	EXPECT_EQ(static_cast<Bingo::NBT_Tag<int>*>(nbt.getTag("score"))->getData(), 100);
	EXPECT_EQ(static_cast<Bingo::NBT_Tag<void*>*>(nbt.getTag("darkness"))->getData(), (void*)0xffffffff);
}

TEST(TestNBT_Compound, TestSetGetTagInd) {
	auto nbt = Bingo::NBT_Compound("test compound");
	nbt.setTag(new Bingo::NBT_Tag<int>("score", 100));
	nbt.setTag(new Bingo::NBT_Tag<void*>("darkness", (void*)0xffffffff));
	nbt.setTag(new Bingo::NBT_Tag<int>("time", 1800));

	EXPECT_EQ(nbt.getTag(0)->getName(), "darkness");
	EXPECT_EQ(nbt.getTag(1)->getName(), "score");
	EXPECT_EQ(nbt.getTag(2)->getName(), "time");
}

TEST(TestNBT_Compound, TestGetTagCount) {
	auto nbt = Bingo::NBT_Compound("test compound");
	nbt.setTag(new Bingo::NBT_Tag<int>("score", 100));
	EXPECT_EQ(nbt.getTagCount(), 1);

	nbt.setTag(new Bingo::NBT_Tag<void*>("darkness", (void*)0xffffffff));
	EXPECT_EQ(nbt.getTagCount(), 2);

	nbt.setTag(new Bingo::NBT_Tag<int>("time", 1800));
	EXPECT_EQ(nbt.getTagCount(), 3);
}

TEST(TestNBT_Compound, TestToString) {
	auto nbt = Bingo::NBT_Compound("test compound");
	nbt.setTag(new Bingo::NBT_Tag<int>("score", 100));
	nbt.setTag(new Bingo::NBT_Tag<void*>("darkness", (void*)0xffffffff));
	nbt.setTag(new Bingo::NBT_Tag<int>("time", 1800));

	EXPECT_EQ(nbt.toString(),
		"NBT_Compound(test compound){\n\tNBT_Tag{darkness:0xFFFFFFFF}\n\tNBT_Tag{score:100}\n\tNBT_Tag{time:1800}\n}");
}

TEST(TestNBT_Compound, TestEqual) {
	auto nbt = Bingo::NBT_Compound("test compound");
	auto nbt2 = Bingo::NBT_Compound("test compound");

	EXPECT_EQ(nbt, nbt2);
}

TEST(TestNBT_Compound, TestEqual2) {
	auto nbt = Bingo::NBT_Compound("test compound");
	nbt.setTag(new Bingo::NBT_Tag<int>("test int", 1));
	// TODO need to solve ambiguous cast issue with NBT_Base
	nbt.setTag(new Bingo::NBT_Tag<Bingo::Math::DynVecN<int>>("test int arr", Bingo::Math::DynVecN<int>(2, { 1, 2 })));

	auto nbt2 = Bingo::NBT_Compound("test compound");
	nbt2.setTag(new Bingo::NBT_Tag<int>("test int", 1));
	nbt2.setTag(new Bingo::NBT_Tag<Bingo::Math::DynVecN<int>>("test int arr", Bingo::Math::DynVecN<int>(2, { 1, 2 })));

	EXPECT_EQ(nbt, nbt2);
}

TEST(TestNBT_Compound, TestNotEqual) {
	auto nbt = Bingo::NBT_Compound("test compound");
	auto nbt2 = Bingo::NBT_Compound("test compound2");

	EXPECT_TRUE(nbt != nbt2);
}

TEST(TestNBT_Compound, TestNotEqual2) {
	auto nbt = Bingo::NBT_Compound("test compound");
	nbt.setTag(new Bingo::NBT_Tag<int>("test int", 1));
	nbt.setTag(new Bingo::NBT_Tag<Bingo::Math::DynVecN<int>>("test int arr", Bingo::Math::DynVecN<int>(2, { 1, 2 })));

	auto nbt2 = Bingo::NBT_Compound("test compound");
	nbt2.setTag(new Bingo::NBT_Tag<int>("test int", 1));
	nbt2.setTag(new Bingo::NBT_Tag<Bingo::Math::DynVecN<int>>("test int arr2", Bingo::Math::DynVecN<int>(2, { 3, 4 })));

	EXPECT_TRUE(nbt != nbt2);
}