#pragma once

#define _MODEL_H_GTEST

#include "pch.h"
#include "Color.h"
#include "MeshManager.h"
#include "Model.h"
#include "Timer.h"
#include "WindowManager.h"

#undef _MODEL_H_GTEST

TEST(TestTransformMatrices, TestBuildTranslationMatrix) {
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1, 0, 0, -3,
		0, 1, 0, 6,
		0, 0, 1, 3,
		0, 0, 0, 1});

	EXPECT_EQ(Bingo::buildTranslationMatrix(-3, 6, 3), mat);
}

TEST(TestTransformMatrices, TestBuildTranslationMatrix2) {
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1, 0, 0, 4.5,
		0, 1, 0, .1,
		0, 0, 1, -6.25,
		0, 0, 0, 1});

	EXPECT_EQ(Bingo::buildTranslationMatrix(4.5, .1, -6.25), mat);
}

TEST(TestTransformMatrices, TestBuildScaleMatrix) {
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		3, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1 });

	EXPECT_EQ(Bingo::buildScaleMatrix(3, 2, -1), mat);
}

TEST(TestTransformMatrices, TestBuildScaleMatrix2) {
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		.1, 0, 0, 0,
		0, 8.66, 0, 0,
		0, 0, .1, 0,
		0, 0, 0, 1 });

	EXPECT_EQ(Bingo::buildScaleMatrix(.1, 8.66, .1), mat);
}

TEST(TestTransformMatrices, TestBuildScaleMatrix3) {
	double sX = 3;
	double sY = 4;
	double sZ = 5;
	double aX = 1;
	double aY = 0;
	double aZ = 1;
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1 + (sX - 1) * aX * aX,		(sX  - 1) * aY * aX,		(sX - 1) * aZ * aX,		0,
		(sY - 1) * aX * aY,			1 + (sY - 1) * aY * aY,		(sY - 1) * aZ * aY,		0,
		(sZ - 1) * aX * aZ,			(sZ -1) * aY * aZ,			1 + (sZ -1) * aZ * aZ,	0,
		0,							0,							0,						1 });

	EXPECT_EQ(Bingo::buildScaleMatrix(Bingo::Math::VecN<double, 3>({aX, aY, aZ}), sX, sY, sZ), mat);
}

TEST(TestTransformMatrices, TestBuildScaleMatrix4) {
	double sX = .1;
	double sY = 8.66;
	double sZ = .1;
	double aX = .5;
	double aY = .5;
	double aZ = .5;
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1 + (sX - 1) * aX * aX,		(sX  - 1) * aY * aX,		(sX - 1) * aZ * aX,		0,
		(sY - 1) * aX * aY,			1 + (sY - 1) * aY * aY,		(sY - 1) * aZ * aY,		0,
		(sZ - 1) * aX * aZ,			(sZ -1) * aY * aZ,			1 + (sZ -1) * aZ * aZ,	0,
		0,							0,							0,						1 });

	EXPECT_EQ(Bingo::buildScaleMatrix(Bingo::Math::VecN<double, 3>({aX, aY, aZ}), sX, sY, sZ), mat);
}

TEST(TestTransformMatrices, TestBuildRotationMatrixX) {
	int angle = 45;
	double angleRad = Bingo::Utils::degToRadians(angle);
	double cosA = std::cos(angleRad);
	double sinA = std::sin(angleRad);
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1,		0,			0,			0,
		0,		cosA,		sinA,		0,
		0,		-sinA,		cosA,		0,
		0,		0,			0,			1 });

	EXPECT_EQ(Bingo::buildRotationMatrixX(angle), mat);
}

TEST(TestTransformMatrices, TestBuildRotationMatrixY) {
	int angle = 90;
	double angleRad = Bingo::Utils::degToRadians(angle);
	double cosA = std::cos(angleRad);
	double sinA = std::sin(angleRad);
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		cosA,		0,		-sinA,		0,
		0,			1,		0,			0,
		sinA,		0,		cosA,		0,
		0,			0,		0,			1 });

	EXPECT_EQ(Bingo::buildRotationMatrixY(angle), mat);
}

TEST(TestTransformMatrices, TestBuildRotationMatrixZ) {
	int angle = 135;
	double angleRad = Bingo::Utils::degToRadians(angle);
	double cosA = std::cos(angleRad);
	double sinA = std::sin(angleRad);
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		cosA,		-sinA,		0,		0,
		sinA,		cosA,		0,		0,
		0,			0,			1,		0,
		0,			0,			0,		1 });

	EXPECT_EQ(Bingo::buildRotationMatrixZ(angle), mat);
}

TEST(TestTransformMatrices, TestBuildRotationMatrix) {
	int angle = 45;
	double angleRad = Bingo::Utils::degToRadians(angle);
	double cosA = std::cos(angleRad);
	double sinA = std::sin(angleRad);
	Bingo::Math::VecN<double, 3> axis({ 1, 3.5, 2.5 });
	double aX = axis.getX();
	double aY = axis.getY();
	double aZ = axis.getZ();
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		cosA + aX * aX * (1 - cosA),		aX * aY * (1 - cosA) - aZ * sinA,	aX * aZ * (1 - cosA) + aY * sinA,	0,
		aY * aX * (1 - cosA) + aZ * sinA,	cosA + aY * aY * (1 - cosA),		aY * aZ * (1 - cosA) - aX * sinA,	0,
		aZ * aX * (1 - cosA) - aY * sinA,	aZ * aY * (1 - cosA) + aX * sinA,	cosA + aZ * aZ * (1 - cosA),		0,
		0,									0,									0,									1 });

	EXPECT_EQ(Bingo::buildRotationMatrix(axis, angle), mat);
}

TEST(TestTransformMatrices, TestBuildShearMatrixYZ) {
	double sY = 3.5;
	double sZ = -2.5;
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1,		sY,		sZ,		0,
		0,		1,		0,		0,
		0,		0,		1,		0,
		0,		0,		0,		1 });

	EXPECT_EQ(Bingo::buildShearMatrixYZ(sY, sZ), mat);
}

TEST(TestTransformMatrices, TestBuildShearMatrixXZ) {
	double sX = 3.5;
	double sZ = -2.5;
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1,		0,		0,		0,
		sX,		1,		sZ,		0,
		0,		0,		1,		0,
		0,		0,		0,		1 });

	EXPECT_EQ(Bingo::buildShearMatrixXZ(sX, sZ), mat);
}

TEST(TestTransformMatrices, TestBuildShearMatrixXY) {
	double sX = 3.5;
	double sY = -2.5;
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1,		0,		0,		0,
		0,		1,		0,		0,
		sX,		sY,		1,		0,
		0,		0,		0,		1 });

	EXPECT_EQ(Bingo::buildShearMatrixXY(sX, sY), mat);
}

TEST(TestTransformMatrices, TestBuildProjectionXY) {
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 1 });

	EXPECT_EQ(Bingo::buildProjectionMatrixXY(), mat);
}

TEST(TestTransformMatrices, TestBuildProjectionXZ) {
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		1, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 });

	EXPECT_EQ(Bingo::buildProjectionMatrixXZ(), mat);
}

TEST(TestTransformMatrices, TestBuildProjectionYZ) {
	auto mat = Bingo::Math::Matrix<double, 4, 4>({
		0, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 });

	EXPECT_EQ(Bingo::buildProjectionMatrixYZ(), mat);
}

//TEST(TestCamera3D, TestConstruct) {
//	auto cam = Bingo::Camera3D();
//	auto* cam2 = new Bingo::Camera3D();
//
//	EXPECT_EQ(cam.getPos(), cam2->getPos());
//
//	delete cam2;
//}

TEST(TestCamera3D, TestConstruct2) {
	auto cam = Bingo::Camera3D({ 10, 10, 10 }, { 0, 1, 0 }, { 0, 0, 0 }, 75, 100, 10);

	EXPECT_EQ(cam.getFov(), 75);
	EXPECT_EQ(cam.getFar(), 100);
	EXPECT_EQ(cam.getNear(), 10);

	auto xAxis = Bingo::Math::VecN<double, 3>({ 10, 10, 10 }).normalizeCopy();
	auto yAxis = Bingo::Math::VecN<double, 3>({ 0, 1, 0 }).cross(xAxis).normalizeCopy();

	EXPECT_EQ(cam.getX_Axis(), xAxis);
	EXPECT_EQ(cam.getY_Axis(), yAxis);
	EXPECT_EQ(cam.getZ_Axis(), yAxis.cross(xAxis).normalizeCopy());
}

TEST(TestMeshManager, TestLoadModel) {
	auto& meshMan = Bingo::MeshManager::getSingleton();

	EXPECT_NO_THROW(meshMan.loadModel("Cube.obj"));

}

TEST(TestMeshManager, TestLoadModel_Cube) {
	auto& meshMan = Bingo::MeshManager::getSingleton();
	auto model = meshMan.loadModel("Cube.obj");

	EXPECT_TRUE(model->objects != NULL);
	EXPECT_TRUE(model->objects->size() == 1);
	EXPECT_TRUE(model->objects->find("Cube") != model->objects->end());
}

TEST(TestMeshManager, TestLoadModel_Cube_vertices) {
	auto& meshMan = Bingo::MeshManager::getSingleton();
	auto model = meshMan.loadModel("Cube.obj");

	for (auto& vert : model->objects->at("Cube")->vertices) {
		// Check unique vertices
		size_t instanceCount = 0;

		for (auto& vert2 : model->objects->at("Cube")->vertices) {
			if (vert == vert2) {
				instanceCount++;
			}
		}

		EXPECT_EQ(instanceCount, 1);

		//Check vertex coordinates
		for (auto& xyz : vert) {
			EXPECT_EQ(std::abs(xyz), 1);
		}
	}
}

TEST(TestMeshManager, TestLoadModel_Cube_normals) {
	auto& meshMan = Bingo::MeshManager::getSingleton();
	auto model = meshMan.loadModel("Cube.obj");

	for (auto& norm : model->objects->at("Cube")->normals) {
		// Check unique normals
		size_t instanceCount = 0;

		for (auto& norm2 : model->objects->at("Cube")->normals) {
			if (norm == norm2) {
				instanceCount++;
			}
		}

		EXPECT_EQ(instanceCount, 1);

		//Check normal vectors
		for (auto& xyz : norm) {
			EXPECT_TRUE(std::abs(xyz) == 1 || std::abs(xyz) == 0);
		}
	}
}

TEST(TestMeshManager, TestLoadModel_Cube_faces) {
	auto& meshMan = Bingo::MeshManager::getSingleton();
	auto model = meshMan.loadModel("Cube.obj");

	// Check expected sizes for Cube.obj
	EXPECT_TRUE(model->objects->at("Cube")->faces.size() == 2);
	EXPECT_TRUE(model->objects->at("Cube")->faces.at(NULL)->size() == 0);
	EXPECT_TRUE(model->objects->at("Cube")->faces.rbegin()->second->size() == 6);

	for (auto& faceByMaterial : model->objects->at("Cube")->faces) {
		for (auto& face : *faceByMaterial.second) {
			// Check face has 1 textureVertex and normal per vertex
			EXPECT_EQ(face.vertices.size(), face.normal.size());
			EXPECT_EQ(face.vertices.size(), face.textureVertices.size());

			// Check for valid indices
			for (auto& vert : face.vertices) {
				EXPECT_LE(vert, model->objects->at("Cube")->vertices.size());
			}

			for (auto& vert : face.textureVertices) {
				EXPECT_LE(vert, model->objects->at("Cube")->textureVertices.size());
			}

			for (auto& vert : face.normal) {
				EXPECT_LE(vert, model->objects->at("Cube")->normals.size());
			}
		}
	}
}

TEST(TestMeshManager, TestLoadModel2) {
	// Testing model caching
	auto& meshMan = Bingo::MeshManager::getSingleton();
	std::string modelName = "Monkey.obj";
	auto timer = Bingo::Time::Timer();
	uint diff1 = 0;
	uint diff2 = 0;

	timer.start();
	EXPECT_NO_THROW(meshMan.loadModel(modelName));
	diff1 = timer.end();

	std::cout << "Took " << diff1 << " milliseconds to load " << modelName << std::endl;

	timer.start();
	EXPECT_NO_THROW(meshMan.loadModel(modelName));
	diff2 = timer.end();

	std::cout << "Took " << diff2 << " milliseconds to load " << modelName << std::endl;

	EXPECT_LT(diff2, diff1);
}

TEST(TestMeshManager, TestLoadModel3) {
	auto& meshMan = Bingo::MeshManager::getSingleton();

	EXPECT_ANY_THROW(meshMan.loadModel("NonExisto.dwg"));
}

TEST(TestMeshManager, TestDraw) {
	auto& meshMan = Bingo::MeshManager::getSingleton();
	// TODO why is this necessary!?
	auto img = Bingo::Surfaces::Surface("images/SplitSquare.png");
	std::string modelName = "Cube.obj";
	auto s = Bingo::Surfaces::Surface(200, 200);
	auto timer = Bingo::Time::Timer();
	uint diff = 0;

	s.saveRenderTarget();
	s.setRenderTarget();
	s.fill(Bingo::Colors::DARKGRAY);

	// pos, up, interest, fov, far near
	Bingo::Camera3D cam({ 1, 1, -1 }, { 0, 1, 0 }, { 0, 0, 0 }, 70, 50, .1);
	auto model = meshMan.loadModel(modelName);

	{
		// draw before transforms
		s.setDrawColor(Bingo::Colors::BLACK);
		meshMan.draw(s, model, cam);
	}

	model->setTranslation({10, 0, 0});
	model->setRotation({ 0, 15, 0 });
	model->setScale(50);
	model->commitTransforms();

	s.setDrawColor(Bingo::Colors::RED);
	timer.start();
	EXPECT_NO_THROW(meshMan.draw(s, model, cam));
	diff = timer.end();

	std::cout << "Took " << diff << " milliseconds to draw " << modelName << std::endl;

	s.restoreRenderTarget();

	Bingo::Surfaces::WindowManager::getSingleton().draw(s, 200, 100);
}