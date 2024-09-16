//Dustin Gehm

#pragma once
#ifndef _VECN_H
#define _VECN_H

#if _DEBUG
#include <array>
#endif
#include <initializer_list>
#include <math.h>
#include <ostream>

#include "Exception.h"
#include "Utils.h"

using std::acos;
using std::initializer_list;
using std::ostream;
using std::to_string;

namespace Bingo {

	namespace Math {

		template<class T>
		class DynVecN;

		template<class T, uint size>
		class VecN {
		public:
			VecN()
				: vals{} {
			}

			//Note for developer, do not try to make this initializer_list a reference, it breaks everryything
			VecN(initializer_list<T> list)
				: vals{} {
#if _DEBUG
				if (list.size() < size) {
					throw Exception("Not enough elements in initializer_list to fill VecN");
				}
#endif
				uint c = 0;

				for (T elem : list) {
					vals[c] = elem;
					c++;
				}
			}

			template<class C>
			VecN(const VecN<C, size>& vec)
				: vals{} {
				for (uint c = 0; c < size; c++) {
					vals[c] = static_cast<T>(vec[c]);
				}
			}

			VecN(const DynVecN<T>& vec)
				: vals{} {
#if _DEBUG
				if (size != vec.getSize()) {
					throw Exception("VecN and DynVecN are not the same size.");
				}
#endif
				for (uint c = 0; c < size; c++) {
					vals[c] = vec[c];
				}
			}

			~VecN() {
				//
			}

			void fill(T val) {
				for (uint c = 0; c < size; c++) {
					vals[c] = val;
				}
			}

			T operator [](const uint index) const {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with VecN");
				}
#endif
				return vals[index];
			}

			T& operator [](const uint index) {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with VecN");
				}
#endif
#if _DEBUG
				return *(vals.data() + index);
#else
				return vals[index];
#endif
			}

			void set(uint index, T val) {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with VecN");
				}
#endif
				vals[index] = val;
			}

			inline T get(uint index) const {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with VecN");
				}
#endif
				return vals[index];
			}

			inline T& get(uint index) {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with VecN");
				}
#endif
				return vals[index];
			}

			void setX(T val) {
#if _DEBUG
				if (size < 1) {
					throw Exception("VecN has no X value");
				}
#endif
				vals[0] = val;
			}

			inline T getX() const {
#if _DEBUG
				if (size < 1) {
					throw Exception("VecN has no X value");
				}
#endif
				return vals[0];
			}

			inline T& getX() {
#if _DEBUG
				if (size < 1) {
					throw Exception("VecN has no X value");
				}
#endif
				return vals[0];
			}

			void setY(T val) {
#if _DEBUG
				if (size < 2) {
					throw Exception("VecN has no Y value");
				}
#endif
				vals[1] = val;
			}

			inline T getY() const {
#if _DEBUG
				if (size < 2) {
					throw Exception("VecN has no Y value");
				}
#endif
				return vals[1];
			}

			inline T& getY() {
#if _DEBUG
				if (size < 2) {
					throw Exception("VecN has no Y value");
				}
#endif
				return vals[1];
			}

			void setZ(T val) {
#if _DEBUG
				if (size < 3) {
					throw Exception("VecN has no Z value");
				}
#endif
				vals[2] = val;
			}

			inline T getZ() const {
#if _DEBUG
				if (size < 3) {
					throw Exception("VecN has no Z value");
				}
#endif
				return vals[2];
			}

			inline T& getZ() {
#if _DEBUG
				if (size < 3) {
					throw Exception("VecN has no Z value");
				}
#endif
				return vals[2];
			}

			void setW(T val) {
#if _DEBUG
				if (size < 4) {
					throw Exception("VecN has no W value");
				}
#endif
				vals[3] = val;
			}

			inline T getW() const {
#if _DEBUG
				if (size < 4) {
					throw Exception("VecN has no W value");
				}
#endif
				return vals[3];
			}

			inline T& getW() {
#if _DEBUG
				if (size < 4) {
					throw Exception("VecN has no W value");
				}
#endif
				return vals[3];
			}

			const T* const data() const {
				return vals.data();
			}

			const T* const begin() const {
				return vals.data();
			}

			const T* const end() const {
				return vals.data() + size;
			}

			VecN<T, size> addTo(uint index, const T& val) const {
				VecN<T, size> result(*this);
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with VecN");
				}
#endif
				result.vals[index] += val;

				return result;
			}

			VecN<T, size> operator -() const {
				VecN<T, size> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] = -result.vals[c];
				}

				return result;
			}

			VecN<T, size> operator +(const VecN<T, size>& other) const {
				VecN<T, size> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] += other.vals[c];
				}

				return result;
			}

			VecN<T, size> operator -(const VecN<T, size>& other) const {
				VecN<T, size> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] -= other.vals[c];
				}

				return result;
			}

			VecN<T, size> operator *(const T& other) const {
				VecN<T, size> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] *= other;
				}

				return result;
			}

			VecN<T, size> operator *(const VecN<T, size>& other) const {
				VecN<T, size> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] *= other.vals[c];
				}

				return result;
			}

			VecN<T, size> operator /(const T& other) const {
				VecN<T, size> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] /= other;
				}

				return result;
			}

			VecN<T, size> operator /(const VecN<T, size>& other) const {
				VecN<T, size> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] /= other.vals[c];
				}

				return result;
			}

			void operator +=(const VecN<T, size>& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] += other.vals[c];
				}
			}

			void operator -=(const VecN<T, size>& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] -= other.vals[c];
				}
			}

			void operator *=(const T& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] *= other;
				}
			}

			void operator *=(const VecN<T, size>& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] *= other.vals[c];
				}
			}

			void operator /=(const T& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] /= other;
				}
			}

			void operator /=(const VecN<T, size>& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] /= other.vals[c];
				}
			}

			bool operator <(const VecN<T, size>& other) const {
				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						return vals[c] < other.vals[c];
					}
				}

				return false;
			}

			bool operator <=(const VecN<T, size>& other) const {
				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						return vals[c] < other.vals[c];
					}
				}

				return true;
			}

			bool operator >(const VecN<T, size>& other) const {
				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						return vals[c] > other.vals[c];
					}
				}

				return false;
			}

			bool operator >=(const VecN<T, size>& other) const {
				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						return vals[c] > other.vals[c];
					}
				}

				return true;
			}

			bool operator ==(const VecN<T, size>& other) const {
				bool result = true;

				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						result = false;
						break;
					}
				}

				return result;
			}

			bool operator !=(const VecN<T, size>& other) const {
				bool result = false;

				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						result = true;
						break;
					}
				}

				return result;
			}

			double magnitude() const {
				double length = 0;

				for (uint c = 0; c < size; c++) {
					length += vals[c] * vals[c];
				}

				return sqrt(length);
			}

			void normalize() {
				double len = magnitude();

				if (len != 0.0) {
					for (uint c = 0; c < size; c++) {
						vals[c] = static_cast<T>(vals[c] / len);
					}
				}
			}

			VecN<T, size> normalizeCopy() const {
				VecN<T, size> result(*this);
				double len = magnitude();

				if (len != 0.0) {
					for (uint c = 0; c < size; c++) {
						result.vals[c] = static_cast<T>(result.vals[c] / len);
					}
				}

				return result;
			}

			T dot(const VecN<T, size>& other) const {
				T result = 0;

				for (uint c = 0; c < size; c++) {
					result += vals[c] * other.vals[c];
				}

				return result;
			}

			VecN<T, size> cross(const VecN<T, size>& other) const {
				VecN<T, size> result;

				if (size != 3) {
					throw Exception("VecN cross product is not valid except in 3D");
				}

				result[0] = (vals[1] * other.vals[2]) - (vals[2] * other.vals[1]);
				result[1] = (vals[2] * other.vals[0]) - (vals[0] * other.vals[2]);
				result[2] = (vals[0] * other.vals[1]) - (vals[1] * other.vals[0]);

				return result;
			}

			//returns the angle in degrees
			double angleBetween(const VecN<T, size>& other) const {
				return Utils::radToDegrees(acos(dot(other) / (magnitude() * other.magnitude())));
			}

			VecN<T, size> reverse() const {
				VecN<T, size> result;

				for (uint c = 0; c < size; c++) {
					result.vals[size - 1 - c] = vals[c];
				}

				return result;
			}

			VecN<T, size> perpendicular() const {
				VecN<T, size> result;

				if (size != 2) {
					throw Exception("VecN must be of size 2 for perpendicular");
				}

				result[0] = vals[1];
				result[1] = -vals[0];

				return result;
			}

			VecN<T, size> projectOnto(const VecN<T, size>& other) const {
				VecN<T, size> otherNorm = other.normalizeCopy();
				return otherNorm * dot(otherNorm);
			}

			friend ostream& operator <<(ostream& os, const VecN<T, size>& vec) {
				os << "V" << size << "{";

				for (uint c = 0; c < size; c++) {
					os << vec.vals[c] << ", ";
				}

				//print 2 backspace characters to erase the last comma
				os << "\b\b}";

				return os;
			}

		private:
			// Note to developer:
			// Do not try to change this in DEBUG to something else for debugging purposes
			// You've tried vectors (vector<bool> has no data())
			std::array<T, size> vals;

			friend class VecN;
		};

		template<class T>
		class DynVecN {
		public:
			DynVecN() {
				size = 0;
				vals = NULL;
			}

			DynVecN(uint len) {
				size = len;
				vals = new T[size];

				memset(vals, 0, sizeof(T) * size);
			}

			DynVecN(initializer_list<T> list) {
				size = list.size();
				vals = new T[size];

				uint c = 0;

				for (T elem : list) {
					vals[c] = elem;
					c++;
				}
			}

			DynVecN(uint len, initializer_list<T> list) {
				size = len;
				vals = new T[size];
#if _DEBUG
				if (list.size() < len) {
					throw Exception("Not enough elements in initializer_list to fill DynVecN");
				}
#endif
				uint c = 0;

				for (T elem : list) {
					vals[c] = elem;
					c++;
				}
			}

			DynVecN(uint len, T arr[]) {
				size = len;
				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = arr[c];
				}
			}

			DynVecN(const DynVecN<T>& vec) {
				size = vec.size;
				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = vec.vals[c];
				}
			}

			template<uint sz>
			DynVecN(const VecN<T, sz>& vec) {
				size = sz;
				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = vec[c];
				}
			}

			DynVecN<T>& operator =(const DynVecN<T>& vec) {
				T* temp = new T[vec.size];

				for (uint c = 0; c < vec.size; c++) {
					temp[c] = vec.vals[c];
				}

				if (vals) {
					delete[] vals;
				}

				size = vec.size;
				vals = temp;

				return *this;
			}

			template<uint sz>
			DynVecN<T>& operator =(const VecN<T, sz>& vec) {
				T* temp = new T[sz];

				for (uint c = 0; c < sz; c++) {
					temp[c] = vec[c];
				}

				if (vals) {
					delete[] vals;
				}

				size = sz;
				vals = temp;

				return *this;
			}

			template<class C>
			DynVecN(const DynVecN<C>& vec) {
				size = vec.size;
				vals = new T[size];

				for (uint c = 0; c < size; c++) {
					vals[c] = static_cast<T>(vec.vals[c]);
				}
			}

			~DynVecN() {
				delete[] vals;
			}

			void fill(T val) {
				for (uint c = 0; c < size; c++) {
					vals[c] = val;
				}
			}

			inline uint getSize() const {
				return size;
			}

			void setSize(uint newSize) {
				if (newSize != size) {
					T* newVals = new T[newSize];
					memset(newVals, 0, sizeof(T) * newSize);
					
					for (size_t i = 0; i < MIN(size, newSize); i++) {
						newVals[i] = vals[i];
					}

					delete[] vals;
					vals = newVals;
					size = newSize;
				}
			}

			T operator [](const uint index) const {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with DynVecN");
				}
#endif
				return vals[index];
			}

			T& operator [](const uint index) {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with DynVecN");
				}
#endif
				return vals[index];
			}

			void set(uint index, T val) {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with DynVecN");
				}
#endif
				vals[index] = val;
			}

			T get(const uint index) const {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with DynVecN");
				}
#endif
				return vals[index];
			}

			T& get(const uint index) {
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with DynVecN");
				}
#endif
				return vals[index];
			}

			void push_back(T val) {
				setSize(size + 1);

				vals[size - 1] = val;
			}

			inline T getX() const {
#if _DEBUG
				if (size < 1) {
					throw Exception("VecN has no X value");
				}
#endif
				return vals[0];
			}

			inline T& getX() {
#if _DEBUG
				if (size < 1) {
					throw Exception("VecN has no X value");
				}
#endif
				return vals[0];
			}

			void setX(T val) {
#if _DEBUG
				if (size < 1) {
					throw Exception("VecN has no X value");
				}
#endif
				vals[0] = val;
			}

			inline T getY() const {
#if _DEBUG
				if (size < 2) {
					throw Exception("VecN has no Y value");
				}
#endif
				return vals[1];
			}

			inline T& getY() {
#if _DEBUG
				if (size < 2) {
					throw Exception("VecN has no Y value");
				}
#endif
				return vals[1];
			}

			void setY(T val) {
#if _DEBUG
				if (size < 2) {
					throw Exception("VecN has no Y value");
				}
#endif
				vals[1] = val;
			}

			inline T getZ() const {
#if _DEBUG
				if (size < 3) {
					throw Exception("VecN has no Z value");
				}
#endif
				return vals[2];
			}

			inline T& getZ() {
#if _DEBUG
				if (size < 3) {
					throw Exception("VecN has no Z value");
				}
#endif
				return vals[2];
			}

			void setZ(T val) {
#if _DEBUG
				if (size < 3) {
					throw Exception("VecN has no Z value");
				}
#endif
				vals[2] = val;
			}

			inline T getW() const {
#if _DEBUG
				if (size < 4) {
					throw Exception("VecN has no X value");
				}
#endif
				return vals[3];
			}

			inline T& getW() {
#if _DEBUG
				if (size < 4) {
					throw Exception("VecN has no X value");
				}
#endif
				return vals[3];
			}

			void setW(T val) {
#if _DEBUG
				if (size < 4) {
					throw Exception("VecN has no W value");
				}
#endif
				vals[3] = val;
			}

			const T* const data() const {
				return vals;
			}

			const T* const begin() const {
				return vals;
			}

			const T* const end() const {
				return vals + size;
			}

			DynVecN<T> addTo(uint index, const T& val) const {
				DynVecN<T> result(*this);
#if _DEBUG
				if (index >= size) {
					throw Exception("Invalid index with DynVecN");
				}
#endif
				result.vals[index] += val;

				return result;
			}

			DynVecN<T> operator -() const {
				DynVecN<T> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] = -result.vals[c];
				}

				return result;
			}

			DynVecN<T> operator +(const DynVecN<T>& other) const {
				DynVecN<T> result(*this);
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					result.vals[c] += other.vals[c];
				}

				return result;
			}

			DynVecN<T> operator -(const DynVecN<T>& other) const {
				DynVecN<T> result(*this);
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					result.vals[c] -= other.vals[c];
				}

				return result;
			}

			DynVecN<T> operator *(const T& other) const {
				DynVecN<T> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] *= other;
				}

				return result;
			}

			DynVecN<T> operator *(const DynVecN<T>& other) const {
				DynVecN<T> result(*this);
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					result.vals[c] *= other.vals[c];
				}

				return result;
			}

			DynVecN<T> operator /(const T& other) const {
				DynVecN<T> result(*this);

				for (uint c = 0; c < size; c++) {
					result.vals[c] /= other;
				}

				return result;
			}

			DynVecN<T> operator /(const DynVecN<T>& other) const {
				DynVecN<T> result(*this);
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					result.vals[c] /= other.vals[c];
				}

				return result;
			}

			void operator +=(const DynVecN<T>& other) {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					vals[c] += other.vals[c];
				}
			}

			void operator -=(const DynVecN<T>& other) {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					vals[c] -= other.vals[c];
				}
			}

			void operator *=(const T& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] *= other;
				}
			}

			void operator *=(const DynVecN<T>& other) {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					vals[c] *= other.vals[c];
				}
			}

			void operator /=(const T& other) {
				for (uint c = 0; c < size; c++) {
					vals[c] /= other;
				}
			}

			void operator /=(const DynVecN<T>& other) {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					vals[c] /= other.vals[c];
				}
			}

			bool operator <(const DynVecN<T>& other) const {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				return magnitude() < other.magnitude();
			}

			bool operator <=(const DynVecN<T>& other) const {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				return magnitude() <= other.magnitude();
			}

			bool operator >(const DynVecN<T>& other) const {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				return magnitude() > other.magnitude();
			}

			bool operator >=(const DynVecN<T>& other) const {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				return magnitude() >= other.magnitude();
			}

			bool operator ==(const DynVecN<T>& other) const {
				bool result = true;
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						result = false;
						break;
					}
				}

				return result;
			}

			bool operator !=(const DynVecN<T>& other) const {
				bool result = false;
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				for (uint c = 0; c < size; c++) {
					if (vals[c] != other.vals[c]) {
						result = true;
						break;
					}
				}

				return result;
			}

			double magnitude() const {
				double length = 0;

				for (uint c = 0; c < size; c++) {
					length += vals[c] * vals[c];
				}

				return sqrt(length);
			}

			void normalize() {
				double len = magnitude();

				for (uint c = 0; c < size; c++) {
					vals[c] = static_cast<T>(vals[c] / len);
				}
			}

			DynVecN<T> normalizeCopy() const {
				DynVecN<T> result(*this);
				double len = magnitude();

				for (uint c = 0; c < size; c++) {
					result.vals[c] = static_cast<T>(result.vals[c] / len);
				}

				return result;
			}

			T dot(const DynVecN<T>& other) const {
				T result = 0;
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif

				for (uint c = 0; c < size; c++) {
					result += vals[c] * other.vals[c];
				}

				return result;
			}

			DynVecN<T> cross(const DynVecN<T>& other) const {
				DynVecN<T> result(size);

				if (size != 3) {
					throw Exception("VecN cross product is not valid except in 3D");
				}

				result[0] = (vals[1] * other.vals[2]) - (vals[2] * other.vals[1]);
				result[1] = (vals[2] * other.vals[0]) - (vals[0] * other.vals[2]);
				result[2] = (vals[0] * other.vals[1]) - (vals[1] * other.vals[0]);

				return result;
			}

			double angleBetween(const DynVecN<T>& other) const {
#if _DEBUG
				if (size != other.size) {
					throw Exception("Incompatible sizes for DynVecN");
				}
#endif
				return Utils::radToDegrees(acos(dot(other) / (magnitude() * other.magnitude())));
			}

			DynVecN<T> reverse() const {
				DynVecN<T> result(size);

				for (uint c = 0; c < size; c++) {
					result.vals[size - 1 - c] = vals[c];
				}

				return result;
			}

			DynVecN<T> perpendicular() const {
				DynVecN<T> result(size);

				result[0] = vals[1];
				result[1] = -vals[0];

				return result;
			}

			DynVecN<T> projectOnto(DynVecN<T>& other) {
				if (size != other.size) {
					throw Exception("Trying to project DynVecN's with different sizes");
				}

				DynVecN<T> otherNorm = other.normalizeCopy();

				return otherNorm * dot(otherNorm);
			}

			friend ostream& operator <<(ostream& os, const DynVecN<T>& vec) {
				os << "V" << vec.size << "{";

				if (vec.size > 0) {
					for (uint c = 0; c < vec.size; c++) {
						os << vec.vals[c] << ", ";
					}

					//print 2 backspace characters to erase the last comma
					os << "\b\b}";
				}
				else {
					os << '}';
				}

				return os;
			}

		private:
			uint size;
			T* vals;

			friend class DynVecN;
		};

	}

}

#endif