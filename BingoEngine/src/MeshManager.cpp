#include "MeshManager.h"

#include <fstream>
#include <math.h>

#include "Exception.h"
#include "FileManager.h"

using std::cos;
using std::ifstream;
using std::ios;
using std::getline;
using std::sin;
using std::tan;

using Bingo::Camera3D;
using Bingo::Colors::Color;
using Bingo::Exception;
using Bingo::FileManager;
using Bingo::Math::Matrix;
using Bingo::Math::VecN;
using Bingo::MeshManager;
using Bingo::OBJ_Material;
using Bingo::OBJ_Mesh;
using Bingo::Surfaces::Surface;
using Bingo::Utils::degToRadians;
using Bingo::Utils::getLineAfter;
using Bingo::Utils::split;
using Bingo::Utils::strip;

Matrix<double, 4, 4> Bingo::buildTranslationMatrix(double x, double y, double z) {
	auto result = Matrix<double, 4, 4>::identity();

	result.get(3, 0) = x;
	result.get(3, 1) = y;
	result.get(3, 2) = z;

	return result;
}

Matrix<double, 4, 4> Bingo::buildScaleMatrix(double x, double y, double z) {
	auto result = Matrix<double, 4, 4>::identity();

	result.get(0, 0) = x;
	result.get(1, 1) = y;
	result.get(2, 2) = z;

	return result;
}

Matrix<double, 4, 4> Bingo::buildScaleMatrix(VecN<double, 3>& axis, double x, double y, double z) {
	auto result = Matrix<double, 4, 4>::identity();

	result.get(0, 0) = 1.0 + (x - 1.0) * axis.getX() * axis.getX();
	result.get(0, 1) = (x - 1.0) * axis.getX() * axis.getY();
	result.get(0, 2) = (x - 1.0) * axis.getX() * axis.getZ();

	result.get(1, 0) = (y - 1.0) * axis.getX() * axis.getY();
	result.get(1, 1) = 1.0 + (y - 1.0) * axis.getY() * axis.getY();
	result.get(1, 2) = (y - 1.0) * axis.getY() * axis.getZ();

	result.get(2, 0) = (z - 1.0) * axis.getX() * axis.getZ();
	result.get(2, 1) = (z - 1.0) * axis.getY() * axis.getZ();
	result.get(2, 2) = 1.0 + (z - 1.0) * axis.getZ() * axis.getZ();

	return result;
}

Matrix<double, 4, 4> Bingo::buildRotationMatrixX(int degrees) {
	auto result = Matrix<double, 4, 4>::identity();
	double radians = degToRadians(degrees);

	result.get(1, 1) = cos(radians);
	result.get(1, 2) = -sin(radians);
	result.get(2, 1) = sin(radians);
	result.get(2, 2) = cos(radians);

	return result;
}

Matrix<double, 4, 4> Bingo::buildRotationMatrixY(int degrees) {
	auto result = Matrix<double, 4, 4>::identity();
	double radians = degToRadians(degrees);

	result.get(0, 0) = cos(radians);
	result.get(0, 2) = sin(radians);
	result.get(2, 0) = -sin(radians);
	result.get(2, 2) = cos(radians);

	return result;
}

Matrix<double, 4, 4> Bingo::buildRotationMatrixZ(int degrees) {
	auto result = Matrix<double, 4, 4>::identity();
	double radians = degToRadians(degrees);

	result.get(0, 0) = cos(radians);
	result.get(0, 1) = -sin(radians);
	result.get(2, 0) = sin(radians);
	result.get(2, 1) = cos(radians);

	return result;
}

Matrix<double, 4, 4> Bingo::buildRotationMatrix(VecN<double, 3>& axis, int degrees) {
	auto result = Matrix<double, 4, 4>::identity();
	double radians = degToRadians(degrees);

	result.get(0, 0) = axis.getX() * axis.getX() * (1.0 - cos(degrees)) + cos(degrees);
	result.get(0, 1) = axis.getX() * axis.getY() * (1.0 - cos(degrees)) + axis.getZ() * sin(degrees);
	result.get(0, 2) = axis.getX() * axis.getZ() * (1.0 - cos(degrees)) - axis.getY() * sin(degrees);

	result.get(1, 0) = axis.getX() * axis.getY() * (1.0 - cos(degrees)) - axis.getZ() * sin(degrees);
	result.get(1, 1) = axis.getY() * axis.getY() * (1.0 - cos(degrees)) + cos(degrees);
	result.get(1, 2) = axis.getY() * axis.getZ() * (1.0 - cos(degrees)) + axis.getX() * sin(degrees);

	result.get(2, 0) = axis.getX() * axis.getZ() * (1.0 - cos(degrees)) + axis.getY() * sin(degrees);
	result.get(2, 1) = axis.getY() * axis.getZ() * (1.0 - cos(degrees)) - axis.getX() * sin(degrees);
	result.get(2, 2) = axis.getZ() * axis.getZ() * (1.0 - cos(degrees)) + cos(degrees);

	return result;
}

Matrix<double, 4, 4> Bingo::buildShearMatrixYZ(double y, double z) {
	auto result = Matrix<double, 4, 4>::identity();

	result.get(0, 1) = y;
	result.get(0, 2) = z;

	return result;
}

Matrix<double, 4, 4> Bingo::buildShearMatrixXZ(double x, double z) {
	auto result = Matrix<double, 4, 4>::identity();

	result.get(1, 0) = x;
	result.get(1, 2) = z;

	return result;
}

Matrix<double, 4, 4> Bingo::buildShearMatrixXY(double x, double y) {
	auto result = Matrix<double, 4, 4>::identity();

	result.get(2, 0) = x;
	result.get(2, 1) = y;

	return result;
}

Camera3D::Camera3D()
	: Positional3D({ 0, 0, 0 }) {
	up = { 0, 1, 0 };
	coi = { 1, 1, 1 };
	xAxis = (pos - coi).normalizeCopy();
	yAxis = (up.cross(xAxis)).normalizeCopy();
	zAxis = (zAxis.cross(xAxis)).normalizeCopy();
	fov = 30.0;
	far = 1000.0;
	near = 0.1;
}

Camera3D::Camera3D(VecN<double, 3> pos, Math::VecN<double, 3> upDir, VecN<double, 3> centerOfInterest, double fieldOfView, double farDistance, double nearDistance)
	: Positional3D(pos) {
	up = upDir;
	coi = centerOfInterest;
	xAxis = (pos - coi).normalizeCopy();
	yAxis = (up.cross(xAxis)).normalizeCopy();
	zAxis = (yAxis.cross(xAxis)).normalizeCopy();
	fov = fieldOfView;
	far = farDistance;
	near = nearDistance;
}

OBJ_Mesh::~OBJ_Mesh() {
	for (auto iter = objects.begin(); iter != objects.end(); iter++) {
		for (auto iter2 = iter->second->faces.begin(); iter2 != iter->second->faces.end(); iter2++) {
			if (iter2->second) {
				delete iter2->second;
			}
		}

		if (iter->second) {
			delete iter->second;
		}
	}
}

void OBJ_Mesh::setTranslation(VecN<double, 3> val) {
	translation = val;

	dirty = true;
}

void OBJ_Mesh::setRotation(int degrees) {
	rotation = degrees;

	dirty = true;
}

void OBJ_Mesh::setScale(double val) {
	scale = val;

	dirty = true;
}

void OBJ_Mesh::computeMatrices() {
	if (dirty) {
		transformMatrix = buildTranslationMatrix(translation[0], translation[1], translation[2]);
		transformMatrix *= buildRotationMatrixY(rotation);
		transformMatrix *= buildScaleMatrix(scale, scale, scale);

		dirty = false;
	}
}

MeshManager::MeshManager(string path) {
	modelPath = path;
}

MeshManager::~MeshManager() {
	for (auto iter = loadedObjModels.begin(); iter != loadedObjModels.end(); iter++) {
		delete iter->second;
	}
}

OBJ_Mesh* MeshManager::loadOBJ_Model(string filename) {
	if (loadedObjModels.find(filename) == loadedObjModels.end()) {
		FileManager* fileMan = FileManager::getSingletonPtr();
		OBJ_Mesh* objMesh = new OBJ_Mesh;
		OBJ_Mesh::OBJ_Object* curObject = NULL;
		vector<Math::VecN<Math::VecN<uint, 3>, 4>>* curFaces = NULL;

		if (!fileMan->checkFile(modelPath + filename)) {
			throw Exception("No such OBJ model file");
		}

		ifstream infile(modelPath + filename, ios::in);
		string line;

		while (getline(infile, line)) {
			line = strip(line);

			if (line.starts_with('#')) {
				continue;
			}
			else if (line.starts_with("mtllib")) {
				vector<string> materialFiles = split(strip(getLineAfter(line, "mtllib")));

				for (auto iter = materialFiles.begin(); iter != materialFiles.end(); iter++) {
					loadOBJ_Material(*iter);
				}
			}
			else if (line.starts_with("o ")) {
				curObject = new OBJ_Mesh::OBJ_Object();
				curObject->faces[NULL] = new vector<Math::VecN<Math::VecN<uint, 3>, 4>>();

				objMesh->objects[strip(getLineAfter(line, "o "))] = curObject;
			}
			else if (line.starts_with("v ")) {
				vector<string> values = split(strip(getLineAfter(line, "v ")));

				if (!curObject) {
					curObject = new OBJ_Mesh::OBJ_Object();
					curObject->faces[NULL] = new vector<Math::VecN<Math::VecN<uint, 3>, 4>>();

					objMesh->objects["default"] = curObject;
				}

				curObject->vertices.push_back({ atof(values[0].c_str()), atof(values[1].c_str()), atof(values[2].c_str()), 1.0 });

				if (values.size() > 3) {
					curObject->vertices.back()[3] = atof(values[3].c_str());
				}
			}
			else if (line.starts_with("vt ")) {
				vector<string> values = split(strip(getLineAfter(line, "vt ")));

				if (!curObject) {
					curObject = new OBJ_Mesh::OBJ_Object();
					curObject->faces[NULL] = new vector<Math::VecN<Math::VecN<uint, 3>, 4>>();

					objMesh->objects["default"] = curObject;
				}

				curObject->textureVertices.push_back({ atof(values[0].c_str()), atof(values[1].c_str()), 0.0 });

				if (values.size() > 2) {
					curObject->textureVertices.back()[2] = atof(values[2].c_str());
				}
			}
			else if (line.starts_with("vn ")) {
				vector<string> values = split(strip(getLineAfter(line, "vn ")));

				if (!curObject) {
					curObject = new OBJ_Mesh::OBJ_Object();
					curObject->faces[NULL] = new vector<Math::VecN<Math::VecN<uint, 3>, 4>>();

					objMesh->objects["default"] = curObject;
				}

				curObject->normals.push_back({ atof(values[0].c_str()), atof(values[1].c_str()), 0.0 });

				if (values.size() > 2) {
					curObject->normals.back()[2] = atof(values[2].c_str());
				}
			}
			else if (line.starts_with("usemtl ")) {
				if (curObject) {
					string materialName = strip(getLineAfter(line, "usemtl "));

					for (auto iter = loadedObjMaterials.begin(); iter != loadedObjMaterials.end(); iter++) {
						if (iter->second.find(materialName) != iter->second.end()) {
							curFaces = new vector<Math::VecN<Math::VecN<uint, 3>, 4>>();

							curObject->faces[iter->second[materialName]] = curFaces;

							break;
						}
					}
				}
			}
			else if (line.starts_with("s ")) {
				//smoothing group, ignore?
			}
			else if (line.starts_with("f ")) {
				if (curObject) {
					VecN<VecN<uint, 3>, 4> face;
					vector<string> values = split(strip(getLineAfter(line, "f ")));

					for (uint c = 0; c < values.size(); c++) {
						vector<string> faceValues = split(strip(values[c]), "/");

						for (uint d = 0; d < faceValues.size(); d++) {
							face[c][d] = atoi(faceValues[d].c_str());
						}
					}

					if (curFaces) {
						curFaces->push_back(face);
					}
					else {
						curObject->faces[NULL]->push_back(face);
					}
				}
			}
		}

		loadedObjModels[filename] = objMesh;
	}

	return loadedObjModels[filename];
}

void MeshManager::draw(Surface& surf, OBJ_Mesh* mesh) {
	Camera3D cam({ 0, 0, 0 }, { 0, 1, 0 }, { 1, 1, 1 }, 30.0, 100.0, 1.0);
	auto world2Cam = Matrix<double, 4, 4>::identity();
	double tVal = 1.0 / tan(degToRadians(cam.getFov()));

	world2Cam.get(0, 0) = -tVal;
	world2Cam.get(1, 1) = -tVal;
	world2Cam.get(2, 2) = cam.getFar() / (cam.getNear() - cam.getFar());
	world2Cam.get(2, 3) = cam.getFar() * cam.getNear() / (cam.getNear() - cam.getFar());

	auto camAxis = Matrix<double, 4, 4>::identity();

	for (uint c = 0; c < 3; c++) {
		camAxis.get(c, 0) = cam.getX_Axis()[c];
		camAxis.get(c, 1) = cam.getY_Axis()[c];
		camAxis.get(c, 2) = cam.getZ_Axis()[c];
	}

	world2Cam = world2Cam.matrixMultiply(camAxis.transpose());
	world2Cam = world2Cam.matrixMultiply(buildTranslationMatrix(-cam.getPosX(), -cam.getPosY(), -cam.getPosZ()));

	auto camera2Scene = Matrix<double, 4, 4>::identity();

	camera2Scene = camera2Scene.matrixMultiply(buildTranslationMatrix(surf.getWidth() / 2.0, surf.getHeight() / 2.0, 0));
	camera2Scene = camera2Scene.matrixMultiply(Matrix<double, 4, 4>({ 1.0, 0.0, 0.0, 0.0,  0.0, -1.0, 0.0, 0.0,  0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 0.0, 1.0 }));
	camera2Scene = camera2Scene.matrixMultiply(buildScaleMatrix((surf.getWidth() / 2.0) - 1, (surf.getHeight() / 2.0) - 1, 1));

	mesh->computeMatrices();

	surf.saveRenderTarget();
	surf.setRenderTarget();

	for (auto objIter = mesh->objects.begin(); objIter != mesh->objects.end(); objIter++) {
		vector<VecN<double, 4>> vertexList;

		for (auto vertIter = objIter->second->vertices.begin(); vertIter != objIter->second->vertices.end(); vertIter++) {
			//vertexList.push_back((world2Cam * mesh->transformMatrix) * (*vertIter));

			//vertexList.back() = vertexList.back() * (1.0 / vertexList.back().getW());

			//vertexList.back() = camera2Scene * vertexList.back();

			vertexList.push_back(*vertIter);
			vertexList.back().getX() = (vertexList.back().getX() * cam.getNear()) / vertexList.back().getZ();
			vertexList.back().getY() = (vertexList.back().getY() * cam.getNear()) / vertexList.back().getZ();

			uint right = surf.getWidth() / 2;
			uint bottom = surf.getHeight() / 2;

			//vertexList.back().getX() = ((vertexList.back().getX() * 2.0) / (right - 0)) - ((right + 0) / (right - 0));
			//vertexList.back().getY() = ((vertexList.back().getY() * 2.0) / (0 - bottom)) - ((0 + bottom) / (0 - bottom));

			//vertexList.back().getX() = (vertexList.back().getX() + 1) / (2.0 * right);
			//vertexList.back().getY() = (1 - vertexList.back().getY()) / (2.0 * bottom);

			vertexList.back().getX() *= right;
			vertexList.back().getY() *= bottom;
		}

		vector<VecN<int, 2>> drawPoints;

		for (auto faceMapIter = objIter->second->faces.begin(); faceMapIter != objIter->second->faces.end(); faceMapIter++) {
			for (auto faceListIter = faceMapIter->second->begin(); faceListIter != faceMapIter->second->end(); faceListIter++) {
				for (uint c = 0; c < 4; c++) {
					if ((*faceListIter)[c][0] != 0) {
						int index = (*faceListIter)[c][0] - 1;
						int nextIndex = 0;

						if ((*faceListIter)[(c + 1) % 4][0] != 0) {
							nextIndex = (*faceListIter)[(c + 1) % 4][0] - 1;
						}
						else {
							nextIndex = (*faceListIter)[(c + 2) % 4][0] - 1;
						}

						drawPoints.push_back({ static_cast<int>(vertexList[index].getX()), static_cast<int>(vertexList[index].getY()) });
						drawPoints.push_back({ static_cast<int>(vertexList[nextIndex].getX()), static_cast<int>(vertexList[nextIndex].getY()) });
					}
				}

				surf.drawLines(drawPoints);
				drawPoints.clear();
			}
		}
	}

	surf.restoreRenderTarget();
}

void MeshManager::loadOBJ_Material(string filename) {
	if (loadedObjMaterials.find(filename) == loadedObjMaterials.end()) {
		FileManager* fileMan = FileManager::getSingletonPtr();
		OBJ_Material* objMaterial = NULL;

		if (!fileMan->checkFile(modelPath + filename)) {
			throw Exception("No such OBJ Material file");
		}

		ifstream infile(modelPath + filename, ios::in);
		string line;

		while (getline(infile, line)) {
			line = strip(line);

			if (line.starts_with('#')) {
				continue;
			}
			else if (line.starts_with("newmtl")) {
				if (objMaterial) {
					loadedObjMaterials[filename][objMaterial->name] = objMaterial;
				}

				objMaterial = new OBJ_Material;
				objMaterial->name = strip(getLineAfter(line, "newmtl"));
			}
			else if (line.starts_with("Ns ")) {
				if (objMaterial) {
					objMaterial->specularFocus = atof(strip(getLineAfter(line, "Ns ")).c_str());
				}
			}
			else if (line.starts_with("Ka ")) {
				if (objMaterial) {
					vector<string> values = split(strip(getLineAfter(line, "Na ")));
					double redVal = atof(values[0].c_str());
					double greenVal = atof(values[1].c_str());
					double blueVal = atof(values[2].c_str());

					objMaterial->ambientColor = Color(
						static_cast<uchar>(redVal * 255.0),
						static_cast<uchar>(greenVal * 255.0),
						static_cast<uchar>(blueVal * 255.0));
				}
			}
			else if (line.starts_with("Kd ")) {
				if (objMaterial) {
					vector<string> values = split(strip(getLineAfter(line, "Na ")));
					double redVal = atof(values[0].c_str());
					double greenVal = atof(values[1].c_str());
					double blueVal = atof(values[2].c_str());

					objMaterial->diffuseColor = Color(
						static_cast<uchar>(redVal * 255.0),
						static_cast<uchar>(greenVal * 255.0),
						static_cast<uchar>(blueVal * 255.0));
				}
			}
			else if (line.starts_with("Ks ")) {
				if (objMaterial) {
					vector<string> values = split(strip(getLineAfter(line, "Na ")));
					double redVal = atof(values[0].c_str());
					double greenVal = atof(values[1].c_str());
					double blueVal = atof(values[2].c_str());

					objMaterial->specularColor = Color(
						static_cast<uchar>(redVal * 255.0),
						static_cast<uchar>(greenVal * 255.0),
						static_cast<uchar>(blueVal * 255.0));
				}
			}
			else if (line.starts_with("Ni ")) {
				if (objMaterial) {
					objMaterial->opticalDensity = atof(strip(getLineAfter(line, "Ni ")).c_str());
				}
			}
			else if (line.starts_with("d ")) {
				if (objMaterial) {
					objMaterial->dissolveFactor = atof(strip(getLineAfter(line, "d ")).c_str());
				}
			}
			else if (line.starts_with("illum ")) {
				if (objMaterial) {
					switch (atoi(strip(getLineAfter(line, "illum ")).c_str())) {
					case 0:
						objMaterial->illumination = OBJ_Material::IlluminationModel::CONSTANT;
						break;
					case 1:
						objMaterial->illumination = OBJ_Material::IlluminationModel::DIFFUSE;
						break;
					case 2:
						objMaterial->illumination = OBJ_Material::IlluminationModel::DIFFUSE_SPECULAR;
						break;
					default:
						objMaterial->illumination = OBJ_Material::IlluminationModel::DIFFUSE_SPECULAR;
						break;
					}
				}
			}
			else if (line.starts_with("map_Kd ")) {
				if (objMaterial) {
					objMaterial->diffuseFile = strip(getLineAfter(line, "map_Kd "));
				}
			}
		}

		if (objMaterial) {
			loadedObjMaterials[filename][objMaterial->name] = objMaterial;
		}
	}
}
