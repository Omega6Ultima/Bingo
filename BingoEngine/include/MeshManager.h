//handles loading 3D models

#pragma once
#ifndef _MESH_MANAGER_H
#define _MESH_MANAGER_H

#include <map>
#include <memory>
#include <string>

#include "Color.h"
#include "Core.h"
#include "Matrix.h"
#include "Model.h"
#include "Positional.h"
#include "Singleton.h"
#include "Surface.h"
#include "Utils.h"
#include "VecN.h"

namespace Bingo {

	Math::Matrix<double, 4, 4> buildTranslationMatrix(double x, double y, double z);

	Math::Matrix<double, 4, 4> buildScaleMatrix(double x, double y, double z);
	Math::Matrix<double, 4, 4> buildScaleMatrix(Math::VecN<double, 3>& axis, double x, double y, double z);

	inline Math::Matrix<double, 4, 4> buildScaleMatrix(double s) {
		return buildScaleMatrix(s, s, s);
	}

	inline Math::Matrix<double, 4, 4> buildScaleMatrix(Math::VecN<double, 3>& axis, double k) {
		return buildScaleMatrix(axis, k, k, k);
	}

	Math::Matrix<double, 4, 4> buildRotationMatrixX(int degrees);
	Math::Matrix<double, 4, 4> buildRotationMatrixY(int degrees);
	Math::Matrix<double, 4, 4> buildRotationMatrixZ(int degrees);
	Math::Matrix<double, 4, 4> buildRotationMatrix(Math::VecN<double, 3>& axis, int degrees);

	Math::Matrix<double, 4, 4> buildShearMatrixYZ(double y, double z);
	Math::Matrix<double, 4, 4> buildShearMatrixXZ(double x, double z);
	Math::Matrix<double, 4, 4> buildShearMatrixXY(double x, double y);

	Math::Matrix<double, 4, 4> buildProjectionMatrixXY();
	Math::Matrix<double, 4, 4> buildProjectionMatrixXZ();
	Math::Matrix<double, 4, 4> buildProjectionMatrixYZ();

	class Camera3D :public Math::Positional3D {
	public:
		//Camera3D();
		Camera3D(Math::VecN<double, 3> pos, Math::VecN<double, 3> upDir, Math::VecN<double, 3> centerOfInterest, double fieldOfView, double farDistance, double nearDistance);

		inline double getFov() const {
			return fov;
		}

		inline double getFar() const {
			return far;
		}

		inline double getNear() const {
			return near;
		}

		inline Math::VecN<double, 3> getX_Axis() const {
			return xAxis;
		}

		inline Math::VecN<double, 3> getY_Axis() const {
			return yAxis;
		}

		inline Math::VecN<double, 3> getZ_Axis() const {
			return zAxis;
		}

	private:
		Math::VecN<double, 3> up;
		Math::VecN<double, 3> coi;
		Math::VecN<double, 3> xAxis;
		Math::VecN<double, 3> yAxis;
		Math::VecN<double, 3> zAxis;
		double fov;
		double far;
		double near;
	};
	
	class MeshManager : public Singleton<MeshManager>, public Core::Manager {
	public:

	public:
		MeshManager(std::string path);
		~MeshManager();

		std::shared_ptr<Models::Model> loadModel(std::string filename);

		void draw(Surfaces::Surface& screen, std::shared_ptr<Models::Model> model, Camera3D& cam);

	private:
		void loadModel_OBJ(std::string filename);
		void loadMaterial_OBJ(std::string filename);

	private:
		typedef Bingo::Math::VecN<double, 2> Point2D;
		typedef Bingo::Math::VecN<double, 3> Point3D;

		std::string modelPath;
		// Map filename to object pointers and material pointers into loadedObjects/loadedMaterials
		std::map<string, std::pair<std::map<std::string, std::shared_ptr<Models::Object>>, std::map<std::string, std::shared_ptr<Models::Material>>>> loadedModels;
		std::map<string, std::map<string, std::shared_ptr<Models::Object>>> loadedObjects;
		std::map<string, std::map<string, std::shared_ptr<Models::Material>>> loadedMaterials;
	};

}

#endif
