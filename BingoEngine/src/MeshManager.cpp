#include "MeshManager.h"

#include <fstream>
#include <math.h>
#include <set>

#include "Exception.h"
#include "FileManager.h"
#include "Mathematics.h"

Bingo::Math::Matrix<double, 4, 4> Bingo::buildTranslationMatrix(double x, double y, double z) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(0, 3) = x;
	result.get(1, 3) = y;
	result.get(2, 3) = z;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildScaleMatrix(double x, double y, double z) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(0, 0) = x;
	result.get(1, 1) = y;
	result.get(2, 2) = z;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildScaleMatrix(Bingo::Math::VecN<double, 3>& axis, double x, double y, double z) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

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

Bingo::Math::Matrix<double, 4, 4> Bingo::buildRotationMatrixX(int degrees) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();
	double radians = Bingo::Utils::degToRadians(degrees);
	double sr = sin(radians);
	double cr = cos(radians);

	result.get(1, 1) = cr;
	result.get(1, 2) = sr;
	result.get(2, 1) = -sr;
	result.get(2, 2) = cr;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildRotationMatrixY(int degrees) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();
	double radians = Bingo::Utils::degToRadians(degrees);
	double sr = sin(radians);
	double cr = cos(radians);

	{
		result.get(0, 0) = cr;
		result.get(0, 2) = -sr;
		result.get(2, 0) = sr;
		result.get(2, 2) = cr;
	}

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildRotationMatrixZ(int degrees) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();
	double radians = Bingo::Utils::degToRadians(degrees);
	double sr = sin(radians);
	double cr = cos(radians);

	result.get(0, 0) = cr;
	result.get(0, 1) = -sr;
	result.get(1, 0) = sr;
	result.get(1, 1) = cr;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildRotationMatrix(Bingo::Math::VecN<double, 3>& axis, int degrees) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();
	double radians = Bingo::Utils::degToRadians(degrees);
	double sr = std::sin(radians);
	double cr = std::cos(radians);
	double oneMinus_cr = (1.0 - cr);

	result.get(0, 0) = axis.getX() * axis.getX() * oneMinus_cr + cr;
	result.get(0, 1) = axis.getX() * axis.getY() * oneMinus_cr - axis.getZ() * sr;
	result.get(0, 2) = axis.getX() * axis.getZ() * oneMinus_cr + axis.getY() * sr;

	result.get(1, 0) = axis.getX() * axis.getY() * oneMinus_cr + axis.getZ() * sr;
	result.get(1, 1) = axis.getY() * axis.getY() * oneMinus_cr + cr;
	result.get(1, 2) = axis.getY() * axis.getZ() * oneMinus_cr - axis.getX() * sr;

	result.get(2, 0) = axis.getX() * axis.getZ() * oneMinus_cr - axis.getY() * sr;
	result.get(2, 1) = axis.getY() * axis.getZ() * oneMinus_cr + axis.getX() * sr;
	result.get(2, 2) = axis.getZ() * axis.getZ() * oneMinus_cr + cr;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildShearMatrixYZ(double y, double z) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(0, 1) = y;
	result.get(0, 2) = z;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildShearMatrixXZ(double x, double z) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(1, 0) = x;
	result.get(1, 2) = z;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildShearMatrixXY(double x, double y) {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(2, 0) = x;
	result.get(2, 1) = y;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildProjectionMatrixXY() {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(2, 2) = 0;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildProjectionMatrixXZ() {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(1, 1) = 0;

	return result;
}

Bingo::Math::Matrix<double, 4, 4> Bingo::buildProjectionMatrixYZ() {
	auto result = Bingo::Math::Matrix<double, 4, 4>::identity();

	result.get(0, 0) = 0;

	return result;
}

//Bingo::Camera3D::Camera3D()
//	: Positional3D({ 0, 0, 0 }) {
//	up = { 0, 1, 0 };
//	coi = { 1, 1, 1 };
//	xAxis = (pos - coi).normalizeCopy();
//	yAxis = (up.cross(xAxis)).normalizeCopy();
//	zAxis = (zAxis.cross(xAxis)).normalizeCopy();
//	fov = 30.0;
//	far = 1000.0;
//	near = 0.1;
//}

Bingo::Camera3D::Camera3D(Bingo::Math::VecN<double, 3> pos, Math::VecN<double, 3> upDir, Bingo::Math::VecN<double, 3> centerOfInterest, double fieldOfView, double farDistance, double nearDistance)
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

Bingo::MeshManager::MeshManager(string path) {
	modelPath = path;

	if (!modelPath.ends_with("/")) {
		modelPath += "/";
	}
}

Bingo::MeshManager::~MeshManager() {
	//
}

std::shared_ptr<Bingo::Models::Model> Bingo::MeshManager::loadModel(string filename) {
	std::shared_ptr<Bingo::Models::Model> model = NULL;

	if (loadedModels.find(filename) == loadedModels.end()) {
		if (filename.ends_with(".obj")) {
			loadModel_OBJ(filename);
		}
		else {
			throw Exception("Unsupported model format");
		}
	}

	model = std::make_shared<Bingo::Models::Model>(&loadedModels[filename].first, &loadedModels[filename].second);

	return model;
}

void Bingo::MeshManager::draw(Bingo::Surfaces::Surface& surf, std::shared_ptr<Models::Model> model, Camera3D& cam) {
	Bingo::Math::Matrix<double, 4, 4> pProject(0.0); // perspective projection matrix

	{
		auto n = cam.getZ_Axis();
		auto nx = n.getX();
		auto ny = n.getY();
		auto nz = n.getZ();

		pProject.get(0, 0) = 1 - nx * nx;
		pProject.get(0, 1) = -nx * ny;
		pProject.get(0, 2) = -nx * nz;

		pProject.get(1, 0) = -nx * ny;
		pProject.get(1, 1) = 1 - ny * ny;
		pProject.get(1, 2) = -ny * nz;

		pProject.get(2, 0) = -nx * nz;
		pProject.get(2, 1) = -ny * nz;
		pProject.get(2, 2) = 1 - nz * nz;

		pProject.get(3, 3) = 1.0;
	}

	// Combine projection matrix with transformation matrix
	//pProject = pProject.matrixMultiply(model->transformMatrix);

	std::vector<std::vector<Point2D>> drawFaces;

	for (auto& object : *(model->objects)) {
		for (auto& faceByMat : (object.second->faces)) {
			for (auto& face : *(faceByMat.second)) {
				std::vector<Point2D> drawFace;

				for (auto& faceInd : face.vertices) {
					auto projectedPoint = pProject * object.second->vertices[faceInd];

					drawFace.push_back({
						projectedPoint.getX() + (surf.getWidth() / 2.0),
						projectedPoint.getY() + (surf.getHeight() / 2.0) });
				}

				drawFaces.push_back(drawFace);
			}
		}
	}

	surf.saveRenderTarget();
	surf.setRenderTarget();

	for (auto& drawFace : drawFaces) {
		//surf.drawLines(drawFace);

		for (size_t i = 0; i < drawFace.size(); i++) {
			size_t next = (i + 1) % drawFace.size();
			
			surf.drawLine(static_cast<int>(drawFace[i].getX()), static_cast<int>(drawFace[i].getY()),
						static_cast<int>(drawFace[next].getX()), static_cast<int>(drawFace[next].getY()));
		}
	}

	surf.restoreRenderTarget();
}

//void Bingo::MeshManager::draw(Bingo::Surfaces::Surface& surf, std::shared_ptr<Models::Model> model) {
//	Camera3D cam({ 0, 0, 0 }, { 0, 1, 0 }, { 1, 1, 1 }, 30.0, 100.0, 1.0);
//	auto world2Cam = Bingo::Math::Matrix<double, 4, 4>::identity();
//	double tVal = 1.0 / tan(Bingo::Utils::degToRadians(cam.getFov()));
//
//	world2Cam.get(0, 0) = -tVal;
//	world2Cam.get(1, 1) = -tVal;
//	world2Cam.get(2, 2) = cam.getFar() / (cam.getNear() - cam.getFar());
//	world2Cam.get(2, 3) = cam.getFar() * cam.getNear() / (cam.getNear() - cam.getFar());
//
//	auto camAxis = Bingo::Math::Matrix<double, 4, 4>::identity();
//
//	for (uint c = 0; c < 3; c++) {
//		camAxis.get(c, 0) = cam.getX_Axis()[c];
//		camAxis.get(c, 1) = cam.getY_Axis()[c];
//		camAxis.get(c, 2) = cam.getZ_Axis()[c];
//	}
//
//	world2Cam = world2Cam.matrixMultiply(camAxis.transpose());
//	world2Cam = world2Cam.matrixMultiply(buildTranslationMatrix(-cam.getPosX(), -cam.getPosY(), -cam.getPosZ()));
//
//	auto camera2Scene = Bingo::Math::Matrix<double, 4, 4>::identity();
//
//	camera2Scene = camera2Scene.matrixMultiply(buildTranslationMatrix(surf.getWidth() / 2.0, surf.getHeight() / 2.0, 0));
//	camera2Scene = camera2Scene.matrixMultiply(Bingo::Math::Matrix<double, 4, 4>({ 1.0, 0.0, 0.0, 0.0,  0.0, -1.0, 0.0, 0.0,  0.0, 0.0, 1.0, 0.0,  0.0, 0.0, 0.0, 1.0 }));
//	camera2Scene = camera2Scene.matrixMultiply(buildScaleMatrix((surf.getWidth() / 2.0) - 1, (surf.getHeight() / 2.0) - 1, 1));
//
//	surf.saveRenderTarget();
//	surf.setRenderTarget();
//
//	for (auto objIter = model->objects->begin(); objIter != model->objects->end(); objIter++) {
//		vector<Bingo::Math::VecN<double, 4>> vertexList;
//
//		for (auto vertIter = objIter->second->vertices.begin(); vertIter != objIter->second->vertices.end(); vertIter++) {
//			vertexList.push_back((world2Cam * model->transformMatrix) * (*vertIter));
//			vertexList.back() = vertexList.back() * (1.0 / vertexList.back().getW());
//			vertexList.back() = camera2Scene * vertexList.back();
//
//			vertexList.push_back(*vertIter);
//			vertexList.back().getX() = (vertexList.back().getX() * cam.getNear()) / vertexList.back().getZ();
//			vertexList.back().getY() = (vertexList.back().getY() * cam.getNear()) / vertexList.back().getZ();
//
//			uint right = surf.getWidth() / 2;
//			uint bottom = surf.getHeight() / 2;
//
//			//vertexList.back().getX() = ((vertexList.back().getX() * 2.0) / (right - 0)) - ((right + 0) / (right - 0));
//			//vertexList.back().getY() = ((vertexList.back().getY() * 2.0) / (0 - bottom)) - ((0 + bottom) / (0 - bottom));
//
//			//vertexList.back().getX() = (vertexList.back().getX() + 1) / (2.0 * right);
//			//vertexList.back().getY() = (1 - vertexList.back().getY()) / (2.0 * bottom);
//
//			vertexList.back().getX() *= right;
//			vertexList.back().getY() *= bottom;
//		}
//
//		vector<Bingo::Math::VecN<int, 2>> drawPoints;
//
//		for (auto faceMapIter = objIter->second->faces.begin(); faceMapIter != objIter->second->faces.end(); faceMapIter++) {
//			for (auto faceListIter = faceMapIter->second->begin(); faceListIter != faceMapIter->second->end(); faceListIter++) {
//				for (uint c = 0; c < 4; c++) {
//					if ((*faceListIter)[c][0] != 0) {
//						int index = (*faceListIter)[c][0] - 1;
//						int nextIndex = 0;
//
//						if ((*faceListIter)[(c + 1) % 4][0] != 0) {
//							nextIndex = (*faceListIter)[(c + 1) % 4][0] - 1;
//						}
//						else {
//							nextIndex = (*faceListIter)[(c + 2) % 4][0] - 1;
//						}
//
//						drawPoints.push_back({ static_cast<int>(vertexList[index].getX()), static_cast<int>(vertexList[index].getY()) });
//						drawPoints.push_back({ static_cast<int>(vertexList[nextIndex].getX()), static_cast<int>(vertexList[nextIndex].getY()) });
//					}
//				}
//
//				surf.drawLines(drawPoints);
//				drawPoints.clear();
//			}
//		}
//	}
//
//	surf.restoreRenderTarget();
//}

void Bingo::MeshManager::loadModel_OBJ(string filename) {
	std::map<string, std::shared_ptr<Models::Object>> objects;
	std::map<string, std::shared_ptr<Models::Material>> materials;

	if (loadedObjects.find(filename) == loadedObjects.end()) {
		FileManager* fileMan = FileManager::getSingletonPtr();
		std::shared_ptr<Models::Object> curObject = NULL;
		vector<Models::Face>* curFaces = NULL;

		if (!fileMan->checkFile(modelPath + filename)) {
			throw Exception("No such OBJ model file");
		}

		std::ifstream infile(modelPath + filename, std::ios::in);
		string line;

		while (getline(infile, line)) {
			line = Bingo::Utils::strip(line);

			if (line.starts_with('#')) {
				continue;
			}
			else if (line.starts_with("mtllib")) {
				vector<string> materialFiles = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "mtllib")));

				for (auto iter = materialFiles.begin(); iter != materialFiles.end(); iter++) {
					loadMaterial_OBJ(*iter);
				}
			}
			else if (line.starts_with("o ")) {
				curObject = std::make_shared<Models::Object>();
				curObject->faces[NULL] = new std::vector<Models::Face>();

				std::string objectName = Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "o "));

				loadedObjects[filename][objectName] = curObject;

				objects[objectName] = curObject;
			}
			else if (line.starts_with("v ")) {
				vector<string> values = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "v ")));

				if (!curObject) {
					curObject = std::make_shared<Models::Object>();
					curObject->faces[NULL] = new std::vector<Models::Face>();

					objects["default"] = curObject;
				}

				curObject->vertices.push_back({ atof(values[0].c_str()), atof(values[1].c_str()), atof(values[2].c_str()), 1.0 });

				if (values.size() > 3) {
					curObject->vertices.back()[3] = atof(values[3].c_str());
				}
			}
			else if (line.starts_with("vt ")) {
				vector<string> values = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "vt ")));

				if (!curObject) {
					curObject = std::make_shared<Models::Object>();
					curObject->faces[NULL] = new std::vector<Models::Face>();

					objects["default"] = curObject;
				}

				curObject->textureVertices.push_back({ atof(values[0].c_str()), atof(values[1].c_str()), 0.0 });

				if (values.size() > 2) {
					curObject->textureVertices.back()[2] = atof(values[2].c_str());
				}
			}
			else if (line.starts_with("vn ")) {
				vector<string> values = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "vn ")));

				if (!curObject) {
					curObject = std::make_shared<Models::Object>();
					curObject->faces[NULL] = new std::vector<Models::Face>();

					objects["default"] = curObject;
				}

				curObject->normals.push_back({ atof(values[0].c_str()), atof(values[1].c_str()), 0.0 });

				if (values.size() > 2) {
					curObject->normals.back()[2] = atof(values[2].c_str());
				}
			}
			else if (line.starts_with("usemtl ")) {
				if (curObject) {
					string materialName = Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "usemtl "));

					for (auto iter = loadedMaterials.begin(); iter != loadedMaterials.end(); iter++) {
						if (iter->second.find(materialName) != iter->second.end()) {
							curFaces = new std::vector<Models::Face>();

							curObject->faces[iter->second[materialName]] = curFaces;

							materials[materialName] = iter->second[materialName];

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
					Models::Face face;
					vector<string> values = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "f ")));

					for (size_t i = 0; i < values.size(); i++) {
						vector<string> faceIndices = Bingo::Utils::split(Bingo::Utils::strip(values[i]), "/");

#if _DEBUG
						if (faceIndices.size() != 3) {
							throw Exception("Invalid face format in OBJ file (" + line + ")");
						}
#endif
						face.vertices.push_back(atoi(faceIndices[0].c_str()) - 1);
						face.textureVertices.push_back(atoi(faceIndices[1].c_str()) - 1);
						face.normal.push_back(atoi(faceIndices[2].c_str()) - 1);
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

		//for (auto& obj : objects) {
		//	Bingo::Math::VecN<double, 4> minVals(obj.second->vertices[0]);

		//	for (auto& verts : obj.second->vertices) {
		//		if (verts.getX() < minVals.getX()) {
		//			minVals.setX(verts.getX());
		//		}

		//		if (verts.getY() < minVals.getY()) {
		//			minVals.setY(verts.getY());
		//		}

		//		if (verts.getZ() < minVals.getZ()) {
		//			minVals.setZ(verts.getZ());
		//		}
		//	}

		//	if (minVals.getX() > 0.0) {
		//		minVals.setX(0.0);
		//	}

		//	if (minVals.getY() > 0.0) {
		//		minVals.setY(0.0);
		//	}

		//	if (minVals.getZ() > 0.0) {
		//		minVals.setZ(0.0);
		//	}

		//	minVals = -minVals;

		//	if (minVals != Bingo::Math::VecN<double, 4>({ 0.0, 0.0, 0.0, 0.0 })) {
		//		for (auto& verts : obj.second->vertices) {
		//			verts += minVals;
		//		}
		//	}
		//}

		loadedModels[filename] = std::make_pair(objects, materials);
	}
}

void Bingo::MeshManager::loadMaterial_OBJ(string filename) {
	if (loadedMaterials.find(filename) == loadedMaterials.end()) {
		FileManager* fileMan = FileManager::getSingletonPtr();
		std::shared_ptr<Models::Material> objMaterial = NULL;

		if (!fileMan->checkFile(modelPath + filename)) {
			throw Exception("No such OBJ Material file");
		}

		std::ifstream infile(modelPath + filename, std::ios::in);
		string line;

		while (getline(infile, line)) {
			line = Bingo::Utils::strip(line);

			if (line.starts_with('#')) {
				continue;
			}
			else if (line.starts_with("newmtl")) {
				if (objMaterial) {
					loadedMaterials[filename][objMaterial->name] = objMaterial;
				}

				objMaterial = std::make_shared<Models::Material>();
				objMaterial->name = Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "newmtl"));
			}
			else if (line.starts_with("Ns ")) {
				if (objMaterial) {
					objMaterial->specularFocus = atof(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "Ns ")).c_str());
				}
			}
			else if (line.starts_with("Ka ")) {
				if (objMaterial) {
					vector<string> values = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "Na ")));
					double redVal = atof(values[0].c_str());
					double greenVal = atof(values[1].c_str());
					double blueVal = atof(values[2].c_str());

					objMaterial->ambientColor = Bingo::Colors::Color(
						static_cast<uchar>(redVal * 255.0),
						static_cast<uchar>(greenVal * 255.0),
						static_cast<uchar>(blueVal * 255.0));
				}
			}
			else if (line.starts_with("Kd ")) {
				if (objMaterial) {
					vector<string> values = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "Na ")));
					double redVal = atof(values[0].c_str());
					double greenVal = atof(values[1].c_str());
					double blueVal = atof(values[2].c_str());

					objMaterial->diffuseColor = Bingo::Colors::Color(
						static_cast<uchar>(redVal * 255.0),
						static_cast<uchar>(greenVal * 255.0),
						static_cast<uchar>(blueVal * 255.0));
				}
			}
			else if (line.starts_with("Ks ")) {
				if (objMaterial) {
					vector<string> values = Bingo::Utils::split(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "Na ")));
					double redVal = atof(values[0].c_str());
					double greenVal = atof(values[1].c_str());
					double blueVal = atof(values[2].c_str());

					objMaterial->specularColor = Bingo::Colors::Color(
						static_cast<uchar>(redVal * 255.0),
						static_cast<uchar>(greenVal * 255.0),
						static_cast<uchar>(blueVal * 255.0));
				}
			}
			else if (line.starts_with("Ni ")) {
				if (objMaterial) {
					objMaterial->opticalDensity = atof(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "Ni ")).c_str());
				}
			}
			else if (line.starts_with("d ")) {
				if (objMaterial) {
					objMaterial->dissolveFactor = atof(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "d ")).c_str());
				}
			}
			else if (line.starts_with("illum ")) {
				if (objMaterial) {
					switch (atoi(Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "illum ")).c_str())) {
					case 0:
						objMaterial->illumination = Models::Material::IlluminationModel::CONSTANT;
						break;
					case 1:
						objMaterial->illumination = Models::Material::IlluminationModel::DIFFUSE;
						break;
					case 2:
						objMaterial->illumination = Models::Material::IlluminationModel::DIFFUSE_SPECULAR;
						break;
					default:
						objMaterial->illumination = Models::Material::IlluminationModel::DIFFUSE_SPECULAR;
						break;
					}
				}
			}
			else if (line.starts_with("map_Kd ")) {
				if (objMaterial) {
					objMaterial->diffuseFile = Bingo::Utils::strip(Bingo::Utils::getLineAfter(line, "map_Kd "));
				}
			}
		}

		if (objMaterial) {
			loadedMaterials[filename][objMaterial->name] = objMaterial;
		}
	}
}
