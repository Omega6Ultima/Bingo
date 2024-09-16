#pragma once
#include "pch.h"
#include "Matrix.h"

TEST(TestMatrix, TestConstruct) {
	auto mat = Bingo::Math::Matrix<int, 2, 2>();
	auto mat2 = new Bingo::Math::Matrix<int, 2, 2>();

	EXPECT_EQ(mat.get(0, 0), 0);
	EXPECT_EQ(mat.get(0, 1), 0);
	EXPECT_EQ(mat.get(1, 0), 0);
	EXPECT_EQ(mat.get(1, 1), 0);

	EXPECT_EQ(mat2->get(0, 0), 0);
	EXPECT_EQ(mat2->get(0, 1), 0);
	EXPECT_EQ(mat2->get(1, 0), 0);
	EXPECT_EQ(mat2->get(1, 1), 0);

	delete mat2;
}

TEST(TestMatrix, TestConstruct2) {
	auto mat = Bingo::Math::Matrix<int, 2, 2>(42);
	auto mat2 = new Bingo::Math::Matrix<int, 2, 2>(42);

	EXPECT_EQ(mat.get(0, 0), 42);
	EXPECT_EQ(mat.get(0, 1), 42);
	EXPECT_EQ(mat.get(1, 0), 42);
	EXPECT_EQ(mat.get(1, 1), 42);

	EXPECT_EQ(mat2->get(0, 0), 42);
	EXPECT_EQ(mat2->get(0, 1), 42);
	EXPECT_EQ(mat2->get(1, 0), 42);
	EXPECT_EQ(mat2->get(1, 1), 42);

	delete mat2;
}

TEST(TestMatrix, TestConstruct3) {
	auto mat = Bingo::Math::Matrix<int, 2, 2>({
		1, 2,
		4, 8,
		});
	auto mat2 = new Bingo::Math::Matrix<int, 2, 2>({
		1, 2,
		4, 8,
		});

	EXPECT_EQ(mat.get(0, 0), 1);
	EXPECT_EQ(mat.get(0, 1), 2);
	EXPECT_EQ(mat.get(1, 0), 4);
	EXPECT_EQ(mat.get(1, 1), 8);

	EXPECT_EQ(mat2->get(0, 0), 1);
	EXPECT_EQ(mat2->get(0, 1), 2);
	EXPECT_EQ(mat2->get(1, 0), 4);
	EXPECT_EQ(mat2->get(1, 1), 8);

	delete mat2;
}

TEST(TestMatrix, TestConstruct4) {
	// Note for developer, C++20 cannot find the constructors used here
	auto mat = Bingo::Math::Matrix<int, 2, 2>(Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 }));
	auto mat2 = new Bingo::Math::Matrix<int, 2, 2>(Bingo::Math::VecN<int, 4>({ 1, 2, 4, 8 }));

	EXPECT_EQ(mat.get(0, 0), 1);
	EXPECT_EQ(mat.get(0, 1), 2);
	EXPECT_EQ(mat.get(1, 0), 4);
	EXPECT_EQ(mat.get(1, 1), 8);

	EXPECT_EQ(mat2->get(0, 0), 1);
	EXPECT_EQ(mat2->get(0, 1), 2);
	EXPECT_EQ(mat2->get(1, 0), 4);
	EXPECT_EQ(mat2->get(1, 1), 8);

	delete mat2;
}

TEST(TestMatrix, TestConstruct5) {
	auto dMat = Bingo::Math::DynMatrix<int>(2, 2, { 1, 2, 4, 8 });
	auto mat = Bingo::Math::Matrix<int, 2, 2>(dMat);
	auto mat2 = new Bingo::Math::Matrix<int, 2, 2>(dMat);

	EXPECT_EQ(mat.get(0, 0), 1);
	EXPECT_EQ(mat.get(0, 1), 2);
	EXPECT_EQ(mat.get(1, 0), 4);
	EXPECT_EQ(mat.get(1, 1), 8);

	EXPECT_EQ(mat2->get(0, 0), 1);
	EXPECT_EQ(mat2->get(0, 1), 2);
	EXPECT_EQ(mat2->get(1, 0), 4);
	EXPECT_EQ(mat2->get(1, 1), 8);

	delete mat2;
}

TEST(TestMatrix, TestIdentity) {
	auto iden3 = Bingo::Math::Matrix<int, 3, 3>::identity();
	auto other3 = Bingo::Math::Matrix<int, 3, 3>({
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
		});

	EXPECT_EQ(iden3, other3);
}

TEST(TestMatrix, TestIdentity2) {
	auto iden5 = Bingo::Math::Matrix<int, 5, 5>::identity();
	auto other5 = Bingo::Math::Matrix<int, 5, 5>({
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0,
		0, 0, 0, 0, 1,
		});

	EXPECT_EQ(iden5, other5);
}

TEST(TestMatrix, TestGet) {
	auto mat = Bingo::Math::Matrix<int, 2, 3>({
		1, 2, 3,
		4, 5, 6,
		});

	EXPECT_EQ(mat.get(0, 0), 1);
	EXPECT_EQ(mat.get(0, 1), 2);
	EXPECT_EQ(mat.get(0, 2), 3);
	EXPECT_EQ(mat.get(1, 0), 4);
	EXPECT_EQ(mat.get(1, 1), 5);
	EXPECT_EQ(mat.get(1, 2), 6);
}

TEST(TestMatrix, TestGetRow) {
	auto mat = Bingo::Math::Matrix<int, 2, 3>({
		1, 2, 3,
		4, 5, 6,
		});
	auto r1 = Bingo::Math::VecN<int, 3>({ 1, 2, 3 });
	auto r2 = Bingo::Math::VecN<int, 3>({ 4, 5, 6 });

	EXPECT_EQ(mat.getRow(0), r1);
	EXPECT_EQ(mat.getRow(1), r2);
}

TEST(TestMatrix, TestGetCol) {
	auto mat = Bingo::Math::Matrix<int, 2, 3>({
		1, 2, 3,
		4, 5, 6,
		});
	auto c1 = Bingo::Math::VecN<int, 2>({ 1, 4 });
	auto c2 = Bingo::Math::VecN<int, 2>({ 2, 5 });
	auto c3 = Bingo::Math::VecN<int, 2>({ 3, 6 });

	EXPECT_EQ(mat.getCol(0), c1);
	EXPECT_EQ(mat.getCol(1), c2);
	EXPECT_EQ(mat.getCol(2), c3);
}

TEST(TestMatrix, TestTranspose) {
	auto mat1 = Bingo::Math::Matrix<int, 2, 4>({
		1, 2, 3, 4,
		5, 6, 7, 8,
		});
	auto mat2 = Bingo::Math::Matrix<int, 4, 2>({
		1, 5,
		2, 6,
		3, 7,
		4, 8,
		});

	EXPECT_EQ(mat1.transpose(), mat2);
}

TEST(TestMatrix, TestMultiply) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		});
	auto mat2 = Bingo::Math::Matrix<int, 3, 1>({
		1,
		1,
		1,
		});
	auto mat3 = Bingo::Math::Matrix<int, 3, 1>({
		3,
		3,
		3,
		});

	EXPECT_EQ(mat1.matrixMultiply(mat2), mat3);
}

TEST(TestMatrix, TestMultiply2) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});
	auto mat2 = Bingo::Math::Matrix<int, 3, 1>({
		1,
		2,
		3,
		});
	auto mat3 = Bingo::Math::Matrix<int, 3, 1>({
		14,
		32,
		50,
		});

	EXPECT_EQ(mat1.matrixMultiply(mat2), mat3);
}

TEST(TestMatrix, TestDeterminant) {
	auto mat1 = Bingo::Math::Matrix<int, 2, 2>({
		4, 6,
		3, 8,
		});

	EXPECT_EQ(mat1.determinant(), 14);
}

TEST(TestMatrix, TestDeterminant2) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 4, 6,
		7, 2, 5,
		9, 8, 3,
		});

	EXPECT_EQ(mat1.determinant(), 290);
}

TEST(TestMatrix, TestDeterminant3) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		6, 1, 1,
		4, -2, 5,
		2, 8, 7,
		});

	EXPECT_EQ(mat1.determinant(), -306);
}

TEST(TestMatrix, TestDeterminant4) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		7, 1, 3,
		2, 4, 1,
		1, 5, 1,
		});

	EXPECT_EQ(mat1.determinant(), 10);
}

TEST(TestMatrix, TestInverse) {
	auto mat1 = Bingo::Math::Matrix<float, 2, 2>({
		4.0f, 7.0f,
		2.0f, 6.0f,
		});

	auto mat2 = Bingo::Math::Matrix<float, 2, 2>({
		.6f, -.7f,
		-.2f, .4f,
		});

	EXPECT_EQ(mat1.inverse(), mat2);
}

TEST(TestMatrix, TestInverse2) {
	auto mat1 = Bingo::Math::Matrix<float, 2, 2>({
		2.0f, 1.0f,
		7.0f, 4.0f,
		});

	auto mat2 = Bingo::Math::Matrix<float, 2, 2>({
		 4.0f, -1.0f,
		-7.0f,  2.0f,
		});

	EXPECT_EQ(mat1.inverse(), mat2);
}

TEST(TestMatrix, TestInverse3) {
	auto mat1 = Bingo::Math::Matrix<float, 2, 2>({
		0.0f, 0.0f,
		7.0f, 4.0f,
		});

	EXPECT_THROW(mat1.inverse(), Bingo::Exception);
}

TEST(TestMatrix, TestMinor) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 2, 2>({
		1, 3,
		7, 9,
		});

	EXPECT_EQ(mat1.minor(1, 1), mat2);
}

TEST(TestMatrix, TestMinor2) {
	auto mat1 = Bingo::Math::Matrix<int, 4, 4>({
		 1,  2,  3,  4,
		 5,  6,  7,  8,
		 9, 10, 11, 12,
		13, 14, 15, 16,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		 1,  3,  4,
		 5,  7,  8,
		13, 15, 16,
		});

	EXPECT_EQ(mat1.minor(2, 1), mat2);
}

TEST(TestMatrix, TestCofactor) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		6, 4, 3,
		9, 2, 5,
		1, 7, 8,
		});

	EXPECT_EQ(mat1.cofactor(1, 1), 45);
}

TEST(TestMatrix, TestCofactor2) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 9, 3,
		2, 5, 4,
		3, 7, 8,
		});

	EXPECT_EQ(mat1.cofactor(1, 0), -51);
}

TEST(TestMatrix, TestNegative) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		-1, -2, -3,
		-4, -5, -6,
		-7, -8, -9
		});

	EXPECT_EQ(-mat1, mat2);
}

TEST(TestMatrix, TestAdd) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		10, 10, 10,
		10, 10, 10,
		10, 10, 10,
		});

	EXPECT_EQ(mat1 + mat2, mat3);
}

TEST(TestMatrix, TestSub) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		-8, -6, -4,
		-2,  0,  2,
		 4,  6,  8,
		});

	EXPECT_EQ(mat1 - mat2, mat3);
}

TEST(TestMatrix, TestMulSingle) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		 2,  4,  6,
		 8, 10, 12,
		14, 16,  18,
		});

	EXPECT_EQ(mat1 * 2, mat2);
}

TEST(TestMatrix, TestMulStraight) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		 9, 16, 21,
		24, 25, 24,
		21, 16,  9,
		});

	EXPECT_EQ(mat1 * mat2, mat3);
}

TEST(TestMatrix, TestDivSingle) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	EXPECT_EQ(mat1 / 2, mat2);
}

TEST(TestMatrix, TestDiv) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		2, 2, 2,
		2, 2, 2,
		2, 2, 2,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	EXPECT_EQ(mat1 / mat2, mat3);
}

TEST(TestMatrix, TestAddEqual) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		10, 10, 10,
		10, 10, 10,
		10, 10, 10,
		});

	mat1 += mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestMatrix, TestSubEqual) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		-8, -6, -4,
		-2,  0,  2,
		 4,  6,  8,
		});

	mat1 -= mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestMatrix, TestMulEqualSingle) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		 2,  4,  6,
		 8, 10, 12,
		14, 16,  18,
		});

	mat1 *= 2;

	EXPECT_EQ(mat1, mat2);
}

TEST(TestMatrix, TestMulEqualStraight) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		 9, 16, 21,
		24, 25, 24,
		21, 16,  9,
		});

	mat1 *= mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestMatrix, TestDivEqualSingle) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	mat1 /= 2;

	EXPECT_EQ(mat1, mat2);
}

TEST(TestMatrix, TestDivEqual) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		2, 2, 2,
		2, 2, 2,
		2, 2, 2,
		});

	auto mat3 = Bingo::Math::Matrix<int, 3, 3>({
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	mat1 /= mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestMatrix, TestEqual) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	EXPECT_TRUE(mat1 == mat2);
}

TEST(TestMatrix, TestNotEqual) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::Matrix<int, 3, 3>({
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	EXPECT_TRUE(mat1 != mat2);
}

TEST(TestMatrix, TestStreamInsert) {
	auto mat1 = Bingo::Math::Matrix<int, 3, 3>({
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	EXPECT_NO_THROW(std::cout << mat1);
}

TEST(TestDynMatrix, TestIdentity) {
	auto iden3 = Bingo::Math::DynMatrix<int>::identity(3);
	auto other3 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
		});

	EXPECT_EQ(iden3, other3);
}

TEST(TestDynMatrix, TestIdentity2) {
	auto iden5 = Bingo::Math::DynMatrix<int>::identity(5);
	auto other5 = Bingo::Math::DynMatrix<int>(5, 5, {
		1, 0, 0, 0, 0,
		0, 1, 0, 0, 0,
		0, 0, 1, 0, 0,
		0, 0, 0, 1, 0,
		0, 0, 0, 0, 1,
		});

	EXPECT_EQ(iden5, other5);
}

TEST(TestDynMatrix, TestTranspose) {
	auto mat1 = Bingo::Math::DynMatrix<int>(2, 4, {
		1, 2, 3, 4,
		5, 6, 7, 8,
		});
	auto mat2 = Bingo::Math::DynMatrix<int>(4, 2, {
		1, 5,
		2, 6,
		3, 7,
		4, 8,
		});

	EXPECT_EQ(mat1.transpose(), mat2);
}

TEST(TestDynMatrix, TestMultiply) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 1, 1,
		1, 1, 1,
		1, 1, 1,
		});
	auto mat2 = Bingo::Math::DynMatrix<int>(3, 1, {
		1,
		1,
		1,
		});
	auto mat3 = Bingo::Math::DynMatrix<int>(3, 1, {
		3,
		3,
		3,
		});

	EXPECT_EQ(mat1.matrixMultiply(mat2), mat3);
}

TEST(TestDynMatrix, TestMultiply2) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});
	auto mat2 = Bingo::Math::DynMatrix<int>(3, 1, {
		1,
		2,
		3,
		});
	auto mat3 = Bingo::Math::DynMatrix<int>(3, 1, {
		14,
		32,
		50,
		});

	EXPECT_EQ(mat1.matrixMultiply(mat2), mat3);
}

TEST(TestDynMatrix, TestDeterminant) {
	auto mat1 = Bingo::Math::DynMatrix<int>(2, 2, {
		4, 6,
		3, 8,
		});

	EXPECT_EQ(mat1.determinant(), 14);
}

TEST(TestDynMatrix, TestDeterminant2) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 4, 6,
		7, 2, 5,
		9, 8, 3,
		});

	EXPECT_EQ(mat1.determinant(), 290);
}

TEST(TestDynMatrix, TestDeterminant3) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		6, 1, 1,
		4, -2, 5,
		2, 8, 7,
		});

	EXPECT_EQ(mat1.determinant(), -306);
}

TEST(TestDynMatrix, TestDeterminant4) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		7, 1, 3,
		2, 4, 1,
		1, 5, 1,
		});

	EXPECT_EQ(mat1.determinant(), 10);
}

TEST(TestDynMatrix, TestInverse) {
	auto mat1 = Bingo::Math::DynMatrix<float>(2, 2, {
		4.0f, 7.0f,
		2.0f, 6.0f,
		});

	auto mat2 = Bingo::Math::DynMatrix<float>(2, 2, {
		.6f, -.7f,
		-.2f, .4f,
		});

	EXPECT_EQ(mat1.inverse(), mat2);
}

TEST(TestDynMatrix, TestInverse2) {
	auto mat1 = Bingo::Math::DynMatrix<float>(2, 2, {
		2.0f, 1.0f,
		7.0f, 4.0f,
		});

	auto mat2 = Bingo::Math::DynMatrix<float>(2, 2, {
		 4.0f, -1.0f,
		-7.0f,  2.0f,
		});

	EXPECT_EQ(mat1.inverse(), mat2);
}

TEST(TestDynMatrix, TestInverse3) {
	auto mat1 = Bingo::Math::DynMatrix<float>(2, 2, {
		0.0f, 0.0f,
		7.0f, 4.0f,
		});

	EXPECT_THROW(mat1.inverse(), Bingo::Exception);
}

TEST(TestDynMatrix, TestMinor) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(2, 2, {
		1, 3,
		7, 9,
		});

	EXPECT_EQ(mat1.minor(1, 1), mat2);
}

TEST(TestDynMatrix, TestMinor2) {
	auto mat1 = Bingo::Math::DynMatrix<int>(4, 4, {
		 1,  2,  3,  4,
		 5,  6,  7,  8,
		 9, 10, 11, 12,
		13, 14, 15, 16,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		 1,  3,  4,
		 5,  7,  8,
		13, 15, 16,
		});

	EXPECT_EQ(mat1.minor(2, 1), mat2);
}

TEST(TestDynMatrix, TestCofactor) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		6, 4, 3,
		9, 2, 5,
		1, 7, 8,
		});

	EXPECT_EQ(mat1.cofactor(1, 1), 45);
}

TEST(TestDynMatrix, TestCofactor2) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 9, 3,
		2, 5, 4,
		3, 7, 8,
		});

	EXPECT_EQ(mat1.cofactor(1, 0), -51);
}

TEST(TestDynMatrix, TestNegative) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		-1, -2, -3,
		-4, -5, -6,
		-7, -8, -9
		});

	EXPECT_EQ(-mat1, mat2);
}

TEST(TestDynMatrix, TestAdd) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		10, 10, 10,
		10, 10, 10,
		10, 10, 10,
		});

	EXPECT_EQ(mat1 + mat2, mat3);
}

TEST(TestDynMatrix, TestSub) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		-8, -6, -4,
		-2,  0,  2,
		 4,  6,  8,
		});

	EXPECT_EQ(mat1 - mat2, mat3);
}

TEST(TestDynMatrix, TestMulSingle) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		 2,  4,  6,
		 8, 10, 12,
		14, 16,  18,
		});

	EXPECT_EQ(mat1 * 2, mat2);
}

TEST(TestDynMatrix, TestMulStraight) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		 9, 16, 21,
		24, 25, 24,
		21, 16,  9,
		});

	EXPECT_EQ(mat1 * mat2, mat3);
}

TEST(TestDynMatrix, TestDivSingle) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	EXPECT_EQ(mat1 / 2, mat2);
}

TEST(TestDynMatrix, TestDiv) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		2, 2, 2,
		2, 2, 2,
		2, 2, 2,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	EXPECT_EQ(mat1 / mat2, mat3);
}

TEST(TestDynMatrix, TestAddEqual) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		10, 10, 10,
		10, 10, 10,
		10, 10, 10,
		});

	mat1 += mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestDynMatrix, TestSubEqual) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		-8, -6, -4,
		-2,  0,  2,
		 4,  6,  8,
		});

	mat1 -= mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestDynMatrix, TestMulEqualSingle) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		 2,  4,  6,
		 8, 10, 12,
		14, 16,  18,
		});

	mat1 *= 2;

	EXPECT_EQ(mat1, mat2);
}

TEST(TestDynMatrix, TestMulEqualStraight) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		 9, 16, 21,
		24, 25, 24,
		21, 16,  9,
		});

	mat1 *= mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestDynMatrix, TestDivEqualSingle) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	mat1 /= 2;

	EXPECT_EQ(mat1, mat2);
}

TEST(TestDynMatrix, TestDivEqual) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		10, 20, 30,
		40, 50, 60,
		70, 80, 90,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		2, 2, 2,
		2, 2, 2,
		2, 2, 2,
		});

	auto mat3 = Bingo::Math::DynMatrix<int>(3, 3, {
		 5, 10, 15,
		20, 25, 30,
		35, 40, 45,
		});

	mat1 /= mat2;

	EXPECT_EQ(mat1, mat3);
}

TEST(TestDynMatrix, TestEqual) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	EXPECT_TRUE(mat1 == mat2);
}

TEST(TestDynMatrix, TestNotEqual) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	auto mat2 = Bingo::Math::DynMatrix<int>(3, 3, {
		9, 8, 7,
		6, 5, 4,
		3, 2, 1,
		});

	EXPECT_TRUE(mat1 != mat2);
}

TEST(TestDynMatrix, TestStreamInsert) {
	auto mat1 = Bingo::Math::DynMatrix<int>(3, 3, {
		1, 2, 3,
		4, 5, 6,
		7, 8, 9,
		});

	EXPECT_NO_THROW(std::cout << mat1);
}
