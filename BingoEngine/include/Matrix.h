//Dustin Gehm

#pragma once
#ifndef _MATRIX_H
#define _MATRIX_H

#define DEBUG_VALS 1

#include <initializer_list>
#include <ostream>
#include <sstream>

#include "Exception.h"
#include "Utils.h"
#include "VecN.h"

using std::endl;
using std::initializer_list;
using std::ostream;
#if _DEBUG
using std::stringstream;
#endif

//TODO add overloads of matrixMultiply that take in a VecN

namespace Bingo {

	namespace Math {

		using Utils::Warn;

		template<class T>
		class DynMatrix;

		template<class T, uint h, uint w>
		class Matrix {
		public:
			Matrix() {
#if DEBUG_VALS
				//
#else
				memset(vals, 0, sizeof(T) * h * w);
#endif
			}

			Matrix(T val) {
#if DEBUG_VALS
				for (uint c = 0; c < h * w; c++) {
					vals[c] = val;
				}
#else
				for (uint c = 0; c < h * w; c++) {
					vals[c] = val;
				}
#endif
			}

			Matrix(initializer_list<T> list) {
#if _DEBUG
				if (list.size() < h * w) {
					throw Exception("Not enough elements in initializer_list to fill Matrix");
				}
#endif
#if DEBUG_VALS
				uint c = 0;

				for (auto elem : list) {
					vals[c] = elem;
					c++;
				}
#else
				uint c = 0;

				for (auto elem : list) {
					vals[c] = elem;
					c++;
				}
#endif
			}

			Matrix(VecN<T, h* w>& vec) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] = vec.vals[c];
				}
			}

			Matrix(const DynMatrix<T>& mat) {
#if _DEBUG
				if (h != mat.h || w != mat.w) {
					throw Exception("Matrix dimensions and DynMatrix dimensions are not the same.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					vals[c] = mat.vals[c];
				}
			}

			~Matrix() {
#if DEBUG_VALS
				//
#else
				//
#endif
			}

			static Matrix<T, h, w> identity() {
				Matrix<T, h, w> iden(0);

#if _DEBUG
				if (h != w) {
					throw Exception("Identity matrix must be a square");
				}
#endif

				for (uint c = 0; c < h; c++) {
					iden.get(c, c) = T(1);
				}

				return iden;
			}

			inline T get(uint row, uint col) const {
#if _DEBUG
				if (row >= h || col >= w) {
					stringstream ss;
					ss << "Invalid indices for Matrix. Indices are r:" << row << ", c:" << col << " size is " << h << "X" << w;
					throw Exception(ss.str());
				}
#endif
				return vals[row * w + col];
			}

			inline T& get(uint row, uint col) {
#if _DEBUG
				if (row >= h || col >= w) {
					stringstream ss;
					ss << "Invalid indices for Matrix. Indices are r:" << row << ", c:" << col << " size is " << h << "X" << w;
					throw Exception(ss.str());
				}
#endif
				return vals[row * w + col];
			}

			VecN<T, h> getCol(uint colInd) {
				VecN<T, h> result(0);

				for (uint c = 0; c < h; c++) {
					result[c] = get(c, colInd);
				}

				return result;
			}

			void setCol(uint colInd, VecN<T, h>& vec) {
				for (uint c = 0; c < h; c++) {
					get(c, colInd) = vec[c];
				}
			}

			VecN<T, w> getRow(uint rowInd) {
				VecN<T, w> result(0);

				for (uint c = 0; c < w; c++) {
					result[c] = get(rowInd, c);
				}

				return result;
			}

			void setRow(uint rowInd, VecN<T, w>& vec) {
				for (uint c = 0; c < w; c++) {
					get(rowInd, c) = vec[c];
				}
			}

			Matrix<T, w, h> transpose() const {
				Matrix<T, w, h> result(0);

				for (uint c = 0; c < h; c++) {
					for (uint d = 0; d < w; d++) {
						result.get(d, c) = get(c, d);
					}
				}

				return result;
			}

			template<uint l>
			Matrix<T, h, l> matrixMultiply(const Matrix<T, w, l>& other) const {
				Matrix<T, h, l> result(0);

				for (uint c = 0; c < h; c++) {
					for (uint d = 0; d < l; d++) {
						T prod = 0;

						for (uint e = 0; e < w; e++) {
							prod += get(c, e) * other.get(e, d);
						}

						result.get(c, d) = prod;
					}
				}

				return result;
			}

			Matrix<T, h, w> operator -() const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] = -result.vals[c];
				}

				return result;
			}

			Matrix<T, h, w> operator +(const Matrix<T, h, w>& other) const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] += other.vals[c];
				}

				return result;
			}

			Matrix<T, h, w> operator -(const Matrix<T, h, w>& other) const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] -= other.vals[c];
				}

				return result;
			}

			Matrix<T, h, w> operator *(const T& other) const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] *= other;
				}

				return result;
			}

			Matrix<T, h, w> operator *(const Matrix<T, h, w>& other) const {
				Matrix<T, w, h> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] *= other.vals[c];
				}

				return result;
			}

			Matrix<T, h, w> operator /(const T& other) const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] /= other;
				}

				return result;
			}

			Matrix<T, h, w> operator /(const Matrix<T, h, w>& other) const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] /= other.vals[c];
				}

				return result;
			}

			void operator +=(const Matrix<T, h, w>& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] += other.vals[c];
				}
			}

			void operator -=(const Matrix<T, h, w>& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] -= other.vals[c];
				}
			}

			void operator *=(const T& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] *= other;
				}
			}

			void operator *=(const Matrix<T, h, w>& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] *= other.vals[c];
				}
			}

			void operator /=(const T& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] /= other;
				}
			}

			void operator /=(const Matrix<T, h, w>& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] /= other.vals[c];
				}
			}

			bool operator ==(const Matrix<T, h, w>& other) const {
				bool result = true;

				for (uint c = 0; c < h * w; c++) {
					if (vals[c] != other.vals[c]) {
						result = false;
						break;
					}
				}

				return result;
			}

			bool operator !=(const Matrix<T, h, w>& other) const {
				bool result = true;

				for (uint c = 0; c < h * w; c++) {
					if (vals[c] == other.vals[c]) {
						result = false;
						break;
					}
				}
			}

			friend ostream& operator <<(ostream& os, const Matrix<T, h, w>& mat) {
				os << "M{";

				for (uint c = 0; c < h; c++) {
					for (uint d = 0; d < w; d++) {
						os << mat.get(c, d) << ", ";
					}

					os << endl << "  ";
				}

				os << "\b\b}";

				return os;
			}

		private:
#if DEBUG_VALS
			VecN<T, h* w> vals;
#else
			T vals[w * h];
#endif

			friend class Matrix;
		};

		template<class T>
		class DynMatrix {
		public:
			DynMatrix() {
				h = 0;
				w = 0;
				vals = NULL;
			}

			DynMatrix(uint height, uint width) {
				h = height;
				w = width;
				vals = new T[h * w];

				memset(vals, 0, sizeof(T) * h * w);
			}

			DynMatrix(uint height, uint width, T val) {
				h = height;
				w = width;
				uint size = h * w;

				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = val;
				}
			}

			DynMatrix(uint height, uint width, initializer_list<T> list) {
				h = height;
				w = width;
				vals = new T[h * w];

				uint c = 0;

				for (auto elem : list) {
					vals[c] = elem;
					c++;
				}
			}

			DynMatrix(uint height, uint width, const DynVecN<T>& vec) {
#if _DEBUG
				if (vec.getSize() < height * width) {
					throw Exception("Not enough elements in DynVecN to fill DynMatrix");
				}
#endif
				h = height;
				w = width;

				vals = new T[h * w];

				for (uint c = 0; c < h * w; c++) {
					vals[c] = vec[c];
				}
			}

			DynMatrix(const DynMatrix<T>& mat) {
				h = mat.h;
				w = mat.w;

				uint size = h * w;

				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = mat.vals[c];
				}
			}

			template<uint height, uint width>
			DynMatrix(const Matrix<T, height, width>& mat) {
				h = height;
				w = width;

				uint size = h * w;

				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = mat.vals[c];
				}
			}

			DynMatrix<T>& operator =(const DynMatrix<T>& mat) {
				uint size = mat.h * mat.w;
				T* temp = new T[size];

				for (uint c = 0; c < size; c++) {
					temp[c] = mat.vals[c];
				}

				if (vals) {
					delete[] vals;
				}

				h = mat.h;
				w = mat.w;
				vals = temp;

				return *this;
			}

			template<uint height, uint width>
			DynMatrix<T>& operator =(const Matrix<T, height, width>& mat) {
				uint size = height * width;
				T* temp = new T[size];

				for (uint c = 0; c < size; c++) {
					temp[c] = mat.vals[c];
				}

				if (vals) {
					delete[] vals;
				}

				h = height;
				w = width;
				vals = temp;

				return *this;
			}

			template<class C>
			DynMatrix(const DynMatrix<C>& mat) {
				h = mat.h;
				w = mat.w;

				uint size = h * w;

				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = T(mat.vals[c]);
				}
			}

			~DynMatrix() {
				delete[] vals;
			}

			inline uint getH() const {
				return h;
			}

			inline uint getW() const {
				return w;
			}

			static DynMatrix<T> identity(uint size) {
				DynMatrix<T> iden(size, size, 0);

				for (uint c = 0; c < size; c++) {
					iden.get(c, c) = T(1);
				}

				return iden;
			}

			inline T get(uint row, uint col) const {
#if _DEBUG
				if (row >= h || col >= w) {
					stringstream ss;
					ss << "Invalid indices for Matrix. Indices are r:" << row << ", c:" << col << " size is " << h << "X" << w;
					throw Exception(ss.str());
				}
#endif
				return vals[row * w + col];
			}

			inline T& get(uint row, uint col) {
#if _DEBUG
				if (row >= h || col >= w) {
					stringstream ss;
					ss << "Invalid indices for Matrix. Indices are r:" << row << ", c:" << col << " size is " << h << "X" << w;
					throw Exception(ss.str());
				}
#endif
				return vals[row * w + col];
			}

			DynVecN<T> getCol(uint colInd) {
				DynVecN<T> result(h, static_cast<T>(0));

				for (uint c = 0; c < result.getSize(); c++) {
					result[c] = get(c, colInd);
				}

				return result;
			}

			void setCol(uint colInd, DynVecN<T>& vec) {
#if _DEBUG
				if (vec.getSize() != h) {
					Warn("Setting a matrix col with a DynVecN of a different size\n");

					return;
				}
#endif
				for (uint c = 0; c < vec.getSize(); c++) {
					get(c, colInd) = vec[c];
				}
			}

			DynVecN<T> getRow(uint rowInd) {
				DynVecN<T> result(w, static_cast<T>(0));

				for (uint c = 0; c < result.getSize(); c++) {
					result[c] = get(rowInd, c);
				}

				return result;
			}

			void setRow(uint rowInd, DynVecN<T>& vec) {
#if _DEBUG
				if (vec.getSize() != w) {
					Warn("Setting a matrix row with a DynVecN of a different size\n");

					return;
				}
#endif
				for (uint c = 0; c < vec.getSize(); c++) {
					get(rowInd, c) = (vec[c]);
				}
			}

			DynMatrix<T> transpose() const {
				DynMatrix<T> result(w, h, 0);

				for (uint c = 0; c < h; c++) {
					for (uint d = 0; d < w; d++) {
						result.get(d, c) = get(c, d);
					}
				}

				return result;
			}

			DynMatrix<T> matrixMultiply(const DynMatrix<T>& other) const {
				DynMatrix<T> result(h, other.w, 0);
#if _DEBUG
				if (w != other.h) {
					throw Exception("Trying to multiply incompatible DynMatrices");
				}
#endif
				for (uint c = 0; c < h; c++) {
					for (uint d = 0; d < other.w; d++) {
						T prod = 0;

						for (uint e = 0; e < w; e++) {
							prod += get(c, e) * other.get(e, d);
						}

						result.get(c, d) = prod;
					}
				}

				return result;
			}

			DynMatrix<T> operator -() const {
				DynMatrix<T> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] = -result.vals[c];
				}

				return result;
			}

			DynMatrix<T> operator +(const DynMatrix<T>& other) const {
				Matrix<T, h, w> result(*this);
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for addition.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					result.vals[c] += other.vals[c];
				}

				return result;
			}

			DynMatrix<T> operator -(const DynMatrix<T>& other) const {
				Matrix<T, h, w> result(*this);
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for subtraction.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					result.vals[c] -= other.vals[c];
				}

				return result;
			}

			DynMatrix<T> operator *(const T& other) const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] *= other;
				}

				return result;
			}

			DynMatrix<T> operator *(const DynMatrix<T>& other) const {
				DynMatrix<T> result(*this);
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for multiplication.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					result.vals[c] *= other.vals[c];
				}

				return result;
			}

			DynMatrix<T> operator /(const T& other) const {
				Matrix<T, h, w> result(*this);

				for (uint c = 0; c < h * w; c++) {
					result.vals[c] /= other;
				}

				return result;
			}

			DynMatrix<T> operator /(const DynMatrix<T>& other) const {
				Matrix<T, h, w> result(*this);
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for division.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					result.vals[c] /= other.vals[c];
				}

				return result;
			}

			void operator +=(const DynMatrix<T>& other) {
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for addition.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					vals[c] += other.vals[c];
				}
			}

			void operator -=(const DynMatrix<T>& other) {
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for subtraction.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					vals[c] -= other.vals[c];
				}
			}

			void operator *=(const T& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] *= other;
				}
			}

			void operator *=(const DynMatrix<T>& other) {
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for multiplication.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					vals[c] *= other.vals[c];
				}
			}

			void operator /=(const T& other) {
				for (uint c = 0; c < h * w; c++) {
					vals[c] /= other;
				}
			}

			void operator /=(const DynMatrix<T>& other) {
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for division.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					vals[c] /= other.vals[c];
				}
			}

			bool operator ==(const DynMatrix<T>& other) const {
				bool result = true;
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for comparision.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					if (vals[c] != other.vals[c]) {
						result = false;
						break;
					}
				}

				return result;
			}

			bool operator !=(const DynMatrix<T>& other) const {
				bool result = true;
#if _DEBUG
				if (h != other.h || w != other.w) {
					throw Exception("Incompatible DynMatrices dimensions for comparision.");
				}
#endif
				for (uint c = 0; c < h * w; c++) {
					if (vals[c] == other.vals[c]) {
						result = false;
						break;
					}
				}
			}

			friend ostream& operator <<(ostream& os, const DynMatrix<T>& mat) {
				os << "M{";

				for (uint c = 0; c < mat.h; c++) {
					for (uint d = 0; d < mat.w; d++) {
						os << mat.get(c, d) << ", ";
					}

					os << endl << "  ";
				}

				os << "\b\b}";

				return os;
			}

		private:
			uint h;
			uint w;
			T* vals;

			friend class DynMatrix;
		};

	}

}

#endif