#pragma once
#ifndef _MODEL_H
#define _MODEL_H

#include <map>
#include <memory>
#include <vector>

#include "Color.h"
#include "Matrix.h"
#include "VecN.h"

namespace Bingo{

	class MeshManager;

	namespace Models {

		struct Material {
			std::string name = "";
			Bingo::Colors::Color ambientColor = Bingo::Colors::Color(0, 0, 0);
			Bingo::Colors::Color diffuseColor = Bingo::Colors::Color(0, 0, 0);
			Bingo::Colors::Color specularColor = Bingo::Colors::Color(0, 0, 0);
			double specularFocus = 0.0;
			double opticalDensity = 0.0;
			double dissolveFactor = 0.0;

			enum class IlluminationModel : uchar {
				CONSTANT = 0,
				DIFFUSE = 1,
				DIFFUSE_SPECULAR = 2,
			} illumination = IlluminationModel::CONSTANT;

			std::string diffuseFile = "";
		};

		struct Face {
			std::vector<uint> vertices;
			std::vector<uint> textureVertices;
			std::vector<uint> normal;
		};

		struct Object {
			~Object();

			std::vector<Bingo::Math::VecN<double, 4>> vertices;
			std::vector<Bingo::Math::VecN<double, 3>> textureVertices;
			std::vector<Bingo::Math::VecN<double, 3>> normals;
			std::map<std::shared_ptr<Material>, std::vector<Face>*> faces;
		};

		class Model {
		public:
			Model();
			Model(std::map<std::string, std::shared_ptr<Object>>* objs,
				std::map<std::string, std::shared_ptr<Material>>* mats);

		public:
			~Model();

			void setTranslation(Bingo::Math::VecN<double, 3> val);
			inline Math::VecN<double, 3> getTranslation() const {
				return translation;
			}

			void setRotation(Bingo::Math::VecN<int, 3> degrees);
			inline Bingo::Math::VecN<int, 3> getRotation() const {
				return rotation;
			}

			void setScale(double val);
			inline double getScale() const {
				return scale;
			}

			// Apply set translation, rotation, and scale to model
			void commitTransforms();

// Here to enable Google Test to unit test this
#ifdef _MODEL_H_GTEST
		public:
#else
		private:
#endif
			std::map<std::string, std::shared_ptr<Object>>* objects = NULL;
			std::map<std::string, std::shared_ptr<Material>>* materials = NULL;

		private:
			Bingo::Math::Matrix<double, 4, 4> transformMatrix = Bingo::Math::Matrix<double, 4, 4>::identity();
			Bingo::Math::VecN<double, 3> translation = { 0, 0, 0 };
			Bingo::Math::VecN<int, 3> rotation = { 0, 0, 0 };
			double scale = 1.0;
			bool dirty = false;

			friend class MeshManager;
		};
	}
}

#endif