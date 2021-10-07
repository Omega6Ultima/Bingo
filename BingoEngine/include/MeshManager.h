//handles loading 3D models

#pragma once
#ifndef _MESH_MANAGER_H
#define _MESH_MANAGER_H

#include <map>
#include <string>

#include "Color.h"
#include "Matrix.h"
#include "Positional.h"
#include "Singleton.h"
#include "Surface.h"
#include "Utils.h"
#include "VecN.h"

using std::map;
using std::pair;

namespace Bingo {
	Math::Matrix<double, 4, 4> buildTranslationMatrix(double x, double y, double z);

	Math::Matrix<double, 4, 4> buildScaleMatrix(double x, double y, double z);
	//TODO double check this actually works with different xyz factors
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

	class Camera3D :public Math::Positional3D {
	public:
		Camera3D();
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

	struct OBJ_Material {
		string name;
		Colors::Color ambientColor = Colors::Color(0, 0, 0);
		Colors::Color diffuseColor = Colors::Color(0, 0, 0);
		Colors::Color specularColor = Colors::Color(0, 0, 0);
		double specularFocus;
		double opticalDensity;
		double dissolveFactor;

		enum class IlluminationModel : uchar {
			CONSTANT = 0,
			DIFFUSE = 1,
			DIFFUSE_SPECULAR = 2,
		} illumination;

		string diffuseFile;
	};

	class OBJ_Mesh {
	public:
		~OBJ_Mesh();

		void setTranslation(Math::VecN<double, 3> val);
		inline Math::VecN<double, 3> getTranslation() const {
			return translation;
		}

		void setRotation(int degrees);
		inline double getRotation() const {
			return rotation;
		}

		void setScale(double val);
		inline double getScale() const {
			return scale;
		}

	private:
		struct OBJ_Object {
			vector<Math::VecN<double, 4>> vertices;
			vector<Math::VecN<double, 3>> textureVertices;
			vector<Math::VecN<double, 3>> normals;
			map<OBJ_Material*, vector<Math::VecN<Math::VecN<uint, 3>, 4>>*> faces;
		};

	private:
		void computeMatrices();

	private:
		map<string, OBJ_Object*> objects;
		Math::Matrix<double, 4, 4> transformMatrix = Math::Matrix<double, 4, 4>::identity();

		Math::VecN<double, 3> translation = { 0, 0, 0 };
		int rotation = 0;
		double scale = 1.0;
		bool dirty = false;

		friend class MeshManager;
	};

	class MeshManager : public Bingo::Singleton<MeshManager> {
	public:
		MeshManager(string path);
		~MeshManager();

		OBJ_Mesh* loadOBJ_Model(string filename);

		void draw(Bingo::Surfaces::Surface& screen, OBJ_Mesh* mesh);

	private:
		void loadOBJ_Material(string filename);

	private:
		string modelPath;
		map<string, OBJ_Mesh*> loadedObjModels;
		map<string, map<string, OBJ_Material*>> loadedObjMaterials;
	};

}

#endif
