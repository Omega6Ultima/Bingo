#pragma once
#include "pch.h"
#include "Mathematics.h"

TEST(TestMaths, TestDistSq) {
	Bingo::Math::VecN<int, 2> point1({ 0, 0 });
	Bingo::Math::VecN<int, 2> point2({ 5, 5 });

	EXPECT_EQ(Bingo::Math::DistanceSq(point1, point2), 50);
}

TEST(TestMaths, TestDist) {
	Bingo::Math::VecN<int, 2> point1({ 0, 0 });
	Bingo::Math::VecN<int, 2> point2({ 3, 4 });

	EXPECT_EQ(Bingo::Math::Distance(point1, point2), 5);
}

TEST(TestMaths, TestTriArea) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ 0, 0 });
	points.push_back({ 0, 3 });
	points.push_back({ 3, 0 });

	EXPECT_EQ(Bingo::Math::TriangleArea(points[0], points[1], points[2]), 4.5f);
}

TEST(TestMaths, TestTriArea2) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ 0, 0 });
	points.push_back({ 9, 0 });
	points.push_back({ 3, 6 });

	EXPECT_EQ(Bingo::Math::TriangleArea(points[0], points[1], points[2]), 27.0f);
}

TEST(TestMaths, TestRectArea) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ 0, 0 });
	points.push_back({ 9, 0 });
	points.push_back({ 9, 6 });
	points.push_back({ 0, 6 });

	EXPECT_EQ(Bingo::Math::RectArea(points[0], points[1], points[2], points[3]), 54.0f);
}

TEST(TestMaths, TestRectArea2) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ -4, -4 });
	points.push_back({ 4, -4 });
	points.push_back({ 4, 4 });
	points.push_back({ -4, 4 });

	EXPECT_EQ(Bingo::Math::RectArea(points[0], points[1], points[2], points[3]), 64.0f);
}

TEST(TestMaths, TestRotatePoint) {
	Bingo::Math::VecN<int, 2> point({ 5, 0 });
	Bingo::Math::VecN<int, 2> origin({ 0, 0 });

	Bingo::Math::VecN<int, 2> rotateResult({ 0, 5 });

	EXPECT_EQ(Bingo::Math::RotatePoint(point, origin, 90), rotateResult);
}

TEST(TestMaths, TestRotatePoint2) {
	Bingo::Math::VecN<int, 2> point({ 0, 5 });
	Bingo::Math::VecN<int, 2> origin({ 0, 0 });

	Bingo::Math::VecN<int, 2> rotateResult({ 0, -5 });

	EXPECT_EQ(Bingo::Math::RotatePoint(point, origin, 180), rotateResult);
}

TEST(TestMaths, TestRotatePoint3) {
	Bingo::Math::VecN<int, 2> point({ 5, 5 });
	Bingo::Math::VecN<int, 2> origin({ 10, 10 });

	Bingo::Math::VecN<int, 2> rotateResult({ 15, 15 });

	EXPECT_EQ(Bingo::Math::RotatePoint(point, origin, 180), rotateResult);
}

TEST(TestMaths, TestPointRectInt) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ 0, 0 });
	points.push_back({ 4, 0 });
	points.push_back({ 4, 4 });
	points.push_back({ 0, 4 });

	Bingo::Math::VecN<int, 2> testPoint({ 2, 2 });

	EXPECT_TRUE(Bingo::Math::PointRectIntersect(testPoint, points[0], points[1], points[2], points[3]));
}

TEST(TestMaths, TestPointRectInt2) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ 0, 0 });
	points.push_back({ 10, 0 });
	points.push_back({ 10, 10 });
	points.push_back({ 0, 10 });

	Bingo::Math::VecN<int, 2> testPoint({ -50, -23 });

	EXPECT_FALSE(Bingo::Math::PointRectIntersect(testPoint, points[0], points[1], points[2], points[3]));
}

TEST(TestMaths, TestPointRectInt3) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ 0, 0 });
	points.push_back({ 10, 0 });
	points.push_back({ 10, 10 });
	points.push_back({ 0, 10 });

	Bingo::Math::VecN<int, 2> testPoint({ 11, 10 });

	EXPECT_FALSE(Bingo::Math::PointRectIntersect(testPoint, points[0], points[1], points[2], points[3]));
}

TEST(TestMaths, TestPointRectInt_Rot) {
	std::vector<Bingo::Math::VecN<int, 2>> points;

	points.push_back({ 5, 10 });
	points.push_back({ 10, 5 });
	points.push_back({ 15, 10 });
	points.push_back({ 10, 15 });

	Bingo::Math::VecN<int, 2> testPoint({ 16, 10 });

	EXPECT_FALSE(Bingo::Math::PointRectIntersect(testPoint, points[0], points[1], points[2], points[3]));
}

TEST(TestMaths, TestRectRectInt) {
	std::vector<Bingo::Math::VecN<int, 2>> points1;

	points1.push_back({ 0, 0 });
	points1.push_back({ 4, 0 });
	points1.push_back({ 4, 4 });
	points1.push_back({ 0, 4 });

	std::vector<Bingo::Math::VecN<int, 2>> points2;

	points2.push_back({ 3, 3 });
	points2.push_back({ 7, 3 });
	points2.push_back({ 7, 7 });
	points2.push_back({ 3, 7 });

	EXPECT_TRUE(Bingo::Math::RectRectIntersect(points1[0], points1[1], points1[2], points1[3], points2[0], points2[1], points2[2], points2[3]));
}

TEST(TestMaths, TestRectRectInt2) {
	std::vector<Bingo::Math::VecN<int, 2>> points1;

	points1.push_back({ -4, -4 });
	points1.push_back({ 0, -4 });
	points1.push_back({ 0, 0 });
	points1.push_back({ -4, 0 });

	std::vector<Bingo::Math::VecN<int, 2>> points2;

	points2.push_back({ 1, 1 });
	points2.push_back({ 5, 1 });
	points2.push_back({ 5, 5 });
	points2.push_back({ 1, 5 });

	EXPECT_FALSE(Bingo::Math::RectRectIntersect(points1[0], points1[1], points1[2], points1[3], points2[0], points2[1], points2[2], points2[3]));
}

TEST(TestMaths, TestCotan) {
	EXPECT_NEAR(Bingo::Math::cotan(45), 1.0, 0.0001);
	EXPECT_NEAR(Bingo::Math::cotan(90), 0.0, 0.0001);
	EXPECT_NEAR(Bingo::Math::cotan(175), -11.430052, 0.0001);
}