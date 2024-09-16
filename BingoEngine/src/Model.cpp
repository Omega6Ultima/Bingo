#include "MeshManager.h"
#include "Model.h"

Bingo::Models::Object::~Object() {
	for (auto iter = faces.begin(); iter != faces.end(); iter++) {
		delete iter->second;
	}
}

Bingo::Models::Model::Model() {
	objects = NULL;
	materials = NULL;
}

Bingo::Models::Model::Model(std::map<std::string, std::shared_ptr<Object>>* objs,
	std::map<std::string, std::shared_ptr<Material>>* mats) {
	objects = objs;
	materials = mats;
}

Bingo::Models::Model::~Model() {
	//
}

void Bingo::Models::Model::setTranslation(Bingo::Math::VecN<double, 3> val) {
	translation = val;

	dirty = true;
}

void Bingo::Models::Model::setRotation(Bingo::Math::VecN<int, 3> degrees) {
	rotation = degrees;

	dirty = true;
}

void Bingo::Models::Model::setScale(double val) {
	scale = val;

	dirty = true;
}

void Bingo::Models::Model::commitTransforms() {
	if (dirty) {
		transformMatrix = buildTranslationMatrix(translation[0], translation[1], translation[2]).matrixMultiply(
			buildRotationMatrixX(rotation[0])).matrixMultiply(
				buildRotationMatrixY(rotation[1])).matrixMultiply(
					buildRotationMatrixZ(rotation[2])).matrixMultiply(
						buildScaleMatrix(scale, scale, scale));

		dirty = false;
	}
}