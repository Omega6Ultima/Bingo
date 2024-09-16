#pragma once
#include "pch.h"
#include "PhysicalObject.h"

TEST(TestPhyObj, TestConstruct) {
	auto po = Bingo::Math::PhysicalObject();
	auto po2 = new Bingo::Math::PhysicalObject();

	EXPECT_EQ(po.getPosX(), 0);
	EXPECT_EQ(po.getPosY(), 0);

	EXPECT_EQ(po.getVelX(), 0.0f);
	EXPECT_EQ(po.getVelY(), 0.0f);

	EXPECT_EQ(po.getAccX(), 0.0f);
	EXPECT_EQ(po.getAccY(), 0.0f);

	EXPECT_EQ(po2->getPosX(), 0);
	EXPECT_EQ(po2->getPosY(), 0);

	EXPECT_EQ(po2->getVelX(), 0.0f);
	EXPECT_EQ(po2->getVelY(), 0.0f);

	EXPECT_EQ(po2->getAccX(), 0.0f);
	EXPECT_EQ(po2->getAccY(), 0.0f);

	delete po2;
}

TEST(TestPhyObj, TestConstruct2) {
	auto po = Bingo::Math::PhysicalObject({ 3, 4 });
	auto po2 = new Bingo::Math::PhysicalObject({ 3, 4 });

	EXPECT_EQ(po.getPosX(), 3);
	EXPECT_EQ(po.getPosY(), 4);

	EXPECT_EQ(po.getVelX(), 0.0f);
	EXPECT_EQ(po.getVelY(), 0.0f);

	EXPECT_EQ(po.getAccX(), 0.0f);
	EXPECT_EQ(po.getAccY(), 0.0f);

	EXPECT_EQ(po2->getPosX(), 3);
	EXPECT_EQ(po2->getPosY(), 4);

	EXPECT_EQ(po2->getVelX(), 0.0f);
	EXPECT_EQ(po2->getVelY(), 0.0f);

	EXPECT_EQ(po2->getAccX(), 0.0f);
	EXPECT_EQ(po2->getAccY(), 0.0f);
}

TEST(TestPhyObj, TestConstruct3) {
	auto po = Bingo::Math::PhysicalObject({ 3, 4 }, { 5, 6 });
	auto po2 = new Bingo::Math::PhysicalObject({ 3, 4 }, { 5, 6 });

	EXPECT_EQ(po.getPosX(), 3);
	EXPECT_EQ(po.getPosY(), 4);

	EXPECT_EQ(po.getVelX(), 5.0f);
	EXPECT_EQ(po.getVelY(), 6.0f);

	EXPECT_EQ(po.getAccX(), 0.0f);
	EXPECT_EQ(po.getAccY(), 0.0f);

	EXPECT_EQ(po2->getPosX(), 3);
	EXPECT_EQ(po2->getPosY(), 4);

	EXPECT_EQ(po2->getVelX(), 5.0f);
	EXPECT_EQ(po2->getVelY(), 6.0f);

	EXPECT_EQ(po2->getAccX(), 0.0f);
	EXPECT_EQ(po2->getAccY(), 0.0f);
}

TEST(TestPhyObj, TestConstruct4) {
	auto po = Bingo::Math::PhysicalObject({ 3, 4 }, { 5, 6 }, { 7.8f, 8.9f });
	auto po2 = new Bingo::Math::PhysicalObject({ 3, 4 }, { 5, 6 }, { 7.8f, 8.9f });

	EXPECT_EQ(po.getPosX(), 3);
	EXPECT_EQ(po.getPosY(), 4);

	EXPECT_EQ(po.getVelX(), 5.0f);
	EXPECT_EQ(po.getVelY(), 6.0f);

	EXPECT_EQ(po.getAccX(), 7.8f);
	EXPECT_EQ(po.getAccY(), 8.9f);

	EXPECT_EQ(po2->getPosX(), 3);
	EXPECT_EQ(po2->getPosY(), 4);

	EXPECT_EQ(po2->getVelX(), 5.0f);
	EXPECT_EQ(po2->getVelY(), 6.0f);

	EXPECT_EQ(po2->getAccX(), 7.8f);
	EXPECT_EQ(po2->getAccY(), 8.9f);
}

TEST(TestPhyObj, TestSetGetVel) {
	auto po = Bingo::Math::PhysicalObject();
	auto other = Bingo::Math::VecN<float, 2>({ -10.0f, -10.0f });

	EXPECT_NO_THROW(po.setVel({ -10.0f, -10.f }));

	EXPECT_EQ(po.getVel(), other);
	EXPECT_EQ(po.getVelX(), -10.0f);
	EXPECT_EQ(po.getVelY(), -10.0f);
}

TEST(TestPhyObj, TestSetGetVel2) {
	auto po = Bingo::Math::PhysicalObject();
	auto other = Bingo::Math::VecN<float, 2>({ -10.0f, -10.0f });

	EXPECT_NO_THROW(po.setVelX(-10.0f));
	EXPECT_NO_THROW(po.setVelY(-10.0f));

	EXPECT_EQ(po.getVel(), other);
	EXPECT_EQ(po.getVelX(), -10.0f);
	EXPECT_EQ(po.getVelY(), -10.0f);
}

TEST(TestPhyObj, TestSetGetAcc) {
	auto po = Bingo::Math::PhysicalObject();
	auto other = Bingo::Math::VecN<float, 2>({ -10.0f, -10.0f });

	EXPECT_NO_THROW(po.setAcc({ -10.0f, -10.f }));

	EXPECT_EQ(po.getAcc(), other);
	EXPECT_EQ(po.getAccX(), -10.0f);
	EXPECT_EQ(po.getAccY(), -10.0f);
}

TEST(TestPhyObj, TestSetGetAcc2) {
	auto po = Bingo::Math::PhysicalObject();
	auto other = Bingo::Math::VecN<float, 2>({ -10.0f, -10.0f });

	EXPECT_NO_THROW(po.setAccX(-10.0f));
	EXPECT_NO_THROW(po.setAccY(-10.0f));

	EXPECT_EQ(po.getAcc(), other);
	EXPECT_EQ(po.getAccX(), -10.0f);
	EXPECT_EQ(po.getAccY(), -10.0f);
}

TEST(TestPhyObj, TestSetGetFriction) {
	auto po = Bingo::Math::PhysicalObject();

	EXPECT_NO_THROW(po.setFriction(10.0f));

	EXPECT_EQ(po.getFriction(), 10.0f);
}

TEST(TestPhyObj, TestSetGetGravity) {
	auto po = Bingo::Math::PhysicalObject();

	EXPECT_NO_THROW(po.setGravityEffects(true));

	EXPECT_TRUE(po.getGravityEffects());
}

TEST(TestPhyObj, TestSetGetGravityStr) {
	auto po = Bingo::Math::PhysicalObject();

	EXPECT_NO_THROW(po.setGravityStr(10.0f));

	EXPECT_EQ(po.getGravityStr(), 10.0f);
}

TEST(TestPhyObj, TestSetGetScreenBounding) {
	auto po = Bingo::Math::PhysicalObject();

	auto bounds = Bingo::Math::VecN<int, 4>({ 10, 10, 90, 90 });

	EXPECT_NO_THROW(po.setScreenBounding(true, 5, { 10, 10, 90, 90 }));

	EXPECT_TRUE(po.getScreenBounding());
	EXPECT_EQ(po.getRadius(), 5);
	EXPECT_EQ(po.getScreenBounds(), bounds);
}