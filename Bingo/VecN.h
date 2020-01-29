//Dustin Gehm

#pragma once
#ifndef _VECN_H
#define _VECN_H

#include <initializer_list>
#include <math.h>
#include <ostream>

#include "Exception.h"
#include "Utils.h"

using std::acos;
using std::initializer_list;
using std::ostream;

template<class T>
class DynVecN;

template<class T, uint size>
class VecN{
public:
	VecN()
	: vals{} {
		//memset(vals, 0, sizeof(T) * size);
	}

	VecN(T val)
	: vals{} {
		for (uint c = 0; c < size; c++){
			vals[c] = val;
		}
	}

	//Note for future Dustin, do not try to make this initializer_list a reference, it breaks everryything
	VecN(initializer_list<T> list)
	: vals{} {
#if _DEBUG
		if (list.size() < size){
			throw Exception("Not enough elements in initializer_list to fill VecN");
		}
#endif
		uint c = 0;

		for (T elem : list){
			vals[c] = elem;
			c++;
		}
	}

	template<class C>
	VecN(const VecN<C, size>& vec)
	: vals{} {
		for (uint c = 0; c < size; c++){
			vals[c] = static_cast<T>(vec[c]);
		}
	}

	VecN(const DynVecN<T>& vec)
	: vals{} {
#if _DEBUG
		if (size != vec.getSize()){
			throw Exception("VecN and DynVecN are not the same size.");
		}
#endif
		for (uint c = 0; c < size; c++){
			vals[c] = vec[c];
		}
	}

	~VecN(){
		//
	}

	T operator [](const uint index) const{
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with VecN");
		}
#endif
		return vals[index];
	}

	T& operator [](const uint index){
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with VecN");
		}
#endif
		return vals[index];
	}

	VecN<T, size> set(uint index, const T& val) const{
		VecN<T, size> result(*this);
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with VecN");
		}
#endif
		result.vals[c] = val;

		return result;
	}

	VecN<T, size> addTo(uint index, const T& val) const{
		VecN<T, size> result(*this);
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with VecN");
		}
#endif
		result.vals[index] += val;

		return result;
	}

	VecN<T, size> operator -() const{
		VecN<T, size> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] = -result.vals[c];
		}

		return result;
	}

	VecN<T, size> operator +(const VecN<T, size>& other) const{
		VecN<T, size> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] += other.vals[c];
		}

		return result;
	}

	VecN<T, size> operator -(const VecN<T, size>& other) const{
		VecN<T, size> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] -= other.vals[c];
		}

		return result;
	}

	VecN<T, size> operator *(const T& other) const{
		VecN<T, size> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] *= other;
		}

		return result;
	}

	VecN<T, size> operator *(const VecN<T, size>& other) const{
		VecN<T, size> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] *= other.vals[c];
		}

		return result;
	}

	VecN<T, size> operator /(const T& other) const{
		VecN<T, size> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] /= other;
		}

		return result;
	}

	VecN<T, size> operator /(const VecN<T, size>& other) const{
		VecN<T, size> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] /= other.vals[c];
		}

		return result;
	}

	void operator +=(const VecN<T, size>& other){
		for (uint c = 0; c < size; c++){
			vals[c] += other.vals[c];
		}
	}

	void operator -=(const VecN<T, size>& other){
		for (uint c = 0; c < size; c++){
			vals[c] -= other.vals[c];
		}
	}

	void operator *=(const T& other){
		for (uint c = 0; c < size; c++){
			vals[c] *= other;
		}
	}

	void operator *=(const VecN<T, size>& other){
		for (uint c = 0; c < size; c++){
			vals[c] *= other.vals[c];
		}
	}

	void operator /=(const T& other){
		for (uint c = 0; c < size; c++){
			vals[c] /= other;
		}
	}

	void operator /=(const VecN<T, size>& other){
		for (uint c = 0; c < size; c++){
			vals[c] /= other.vals[c];
		}
	}

	bool operator <(const VecN<T, size>& other) const{
		return getVecLength() < other.getVecLength();
	}

	bool operator <=(const VecN<T, size>& other) const{
		return getVecLength() <= other.getVecLength();
	}

	bool operator >(const VecN<T, size>& other) const{
		return getVecLength() > other.getVecLength();
	}

	bool operator >=(const VecN<T, size>& other) const{
		return getVecLength() >= other.getVecLength();
	}

	bool operator ==(const VecN<T, size>& other) const{
		bool result = true;

		for (uint c = 0; c < size; c++){
			if (vals[c] != other.vals[c]){
				result = false;
				break;
			}
		}

		return result;
	}

	bool operator !=(const VecN<T, size>& other) const{
		bool result = true;

		for (uint c = 0; c < size; c++){
			if (vals[c] == other.vals[c]){
				result = false;
				break;
			}
		}

		return result;
	}

	double getVecLength() const{
		double length = 0;

		for (uint c = 0; c < size; c++){
			length += vals[c] * vals[c];
		}

		return pow(length, 1.0f / size);
	}

	void normalize(){
		double len = getVecLength();

		for (uint c = 0; c < size; c++){
			vals[c] = static_cast<T>(vals[c] / len);
		}
	}

	VecN<T, size> normalizeCopy() const{
		VecN<T, size> result(*this);
		double len = getVecLength();

		for (uint c = 0; c < size; c++){
			result.vals[c] = static_cast<T>(result.vals[c] / len);
		}

		return result;
	}

	T dot(const VecN<T, size>& other) const{
		T result = 0;

		for (uint c = 0; c < size; c++){
			result += vals[c] * other.vals[c];
		}

		return result;
	}

	//returns the angle in radians
	double angleBetween(const VecN<T, size>& other) const{
		return acos(dot(other) / (getVecLength() * other.getVecLength()));
	}

	VecN<T, size> reverse() const{
		VecN<T, size> result;

		for (uint c = 0; c < size; c++){
			result.vals[size - 1 - c] = vals[c];
		}

		return result;
	}

	VecN<T, size> perpendicular() const{
		VecN<T, size> result;

		result[0] = vals[1];
		result[1] = -vals[0];

		return result;
	}

	friend ostream& operator <<(ostream& os, const VecN<T, size>& vec){
		os << "V" << size << "{";

		for (uint c = 0; c < size; c++){
			os << vec.vals[c] << ", ";
		}

		os << "\b\b}";

		return os;
	}

private:
	T vals[size];
};

template<class T>
class DynVecN{
public:
	DynVecN(){
		size = 0;
		vals = NULL;
	}

	DynVecN(uint len){
		size = len;
		vals = new T[size];

		memset(vals, 0, sizeof(T) * size);
	}

	DynVecN(uint len, T val){
		size = len;
		vals = new T[size];

		for (uint c = 0; c < size; c++){
			vals[c] = val;
		}
	}

	DynVecN(initializer_list<T> list){
		size = list.size();
		vals = new T[size];

		uint c = 0;

		for (T elem : list){
			vals[c] = elem;
			c++;
		}
	}

	DynVecN(uint len, initializer_list<T> list){
		size = len;
		vals = new T[size];
#if _DEBUG
		if (list.size() < len){
			throw Exception("Not enough elements in initializer_list to fill DynVecN");
		}
#endif
		uint c = 0;

		for (T elem : list){
			vals[c] = elem;
			c++;
		}
	}

	DynVecN(const DynVecN<T>& vec){
		size = vec.size;
		vals = new T[size];

		for (uint c = 0; c < size; c++){
			vals[c] = vec.vals[c];
		}
	}

	template<uint sz>
	DynVecN(const VecN<T, sz>& vec){
		size = sz;
		vals = new T[size];

		for (uint c = 0; c < size; c++){
			vals[c] = vec[c];
		}
	}

	DynVecN<T>& operator =(const DynVecN<T>& vec){
		T* temp = new T[vec.size];

		for (uint c = 0; c < vec.size; c++){
			temp[c] = vec.vals[c];
		}

		if (vals){
			delete[] vals;
		}

		size = vec.size;
		vals = temp;

		return *this;
	}

	template<uint sz>
	DynVecN<T>& operator =(const VecN<T, sz>& vec){
		T* temp = new T[sz];

		for (uint c = 0; c < sz; c++){
			temp[c] = vec[c];
		}

		if (vals){
			delete[] vals;
		}

		size = sz;
		vals = temp;

		return *this;
	}

	template<class C>
	DynVecN(const DynVecN<C>& vec){
		size = vec.size;
		vals = new T[size];

		for (uint c = 0; c < size; c++){
			vals[c] = static_cast<T>(vec.vals[c]);
		}
	}

	~DynVecN(){
		delete[] vals;
	}

	inline uint getSize() const{
		return size;
	}

	void setSize(uint newSize){
		size = newSize;

		if (vals){
			delete[] vals;
		}

		vals = new T[size];

		memset(vals, 0, sizeof(T) * size);
	}

	T operator [](const uint index) const{
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with DynVecN");
		}
#endif
		return vals[index];
	}

	T& operator [](const uint index){
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with DynVecN");
		}
#endif
		return vals[index];
	}

	DynVecN<T> set(uint index, const T& val) const{
		DynVecN<T> result(*this);
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with VecN");
		}
#endif
		result.vals[c] = val;

		return result;
	}

	DynVecN<T> addTo(uint index, const T& val) const{
		DynVecN<T> result(*this);
#if _DEBUG
		if (index >= size){
			throw Exception("Invalid index with DynVecN");
		}
#endif
		result.vals[index] += val;

		return result;
	}

	DynVecN<T> operator -() const{
		DynVecN<T> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] = -result.vals[c];
		}

		return result;
	}

	DynVecN<T> operator +(const DynVecN<T>& other) const{
		DynVecN<T> result(*this);
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			result.vals[c] += other.vals[c];
		}

		return result;
	}

	DynVecN<T> operator -(const DynVecN<T>& other) const{
		DynVecN<T> result(*this);
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			result.vals[c] -= other.vals[c];
		}

		return result;
	}

	DynVecN<T> operator *(const T& other) const{
		DynVecN<T> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] *= other;
		}

		return result;
	}

	DynVecN<T> operator *(const DynVecN<T>& other) const{
		DynVecN<T> result(*this);
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			result.vals[c] *= other.vals[c];
		}

		return result;
	}

	DynVecN<T> operator /(const T& other) const{
		DynVecN<T> result(*this);

		for (uint c = 0; c < size; c++){
			result.vals[c] /= other;
		}

		return result;
	}

	DynVecN<T> operator /(const DynVecN<T>& other) const{
		DynVecN<T> result(*this);
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			result.vals[c] /= other.vals[c];
		}

		return result;
	}

	void operator +=(const DynVecN<T>& other){
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			vals[c] += other.vals[c];
		}
	}

	void operator -=(const DynVecN<T>& other){
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			vals[c] -= other.vals[c];
		}
	}

	void operator *=(const T& other){
		for (uint c = 0; c < size; c++){
			vals[c] *= other;
		}
	}

	void operator *=(const DynVecN<T>& other){
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			vals[c] *= other.vals[c];
		}
	}

	void operator /=(const T& other){
		for (uint c = 0; c < size; c++){
			vals[c] /= other;
		}
	}

	void operator /=(const DynVecN<T>& other){
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			vals[c] /= other.vals[c];
		}
	}

	bool operator <(const DynVecN<T>& other) const{
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		return getVecLength() < other.getVecLength();
	}

	bool operator <=(const DynVecN<T>& other) const{
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		return getVecLength() <= other.getVecLength();
	}

	bool operator >(const DynVecN<T>& other) const{
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		return getVecLength() > other.getVecLength();
	}

	bool operator >=(const DynVecN<T>& other) const{
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		return getVecLength() >= other.getVecLength();
	}

	bool operator ==(const DynVecN<T>& other) const{
		bool result = true;
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			if (vals[c] != other.vals[c]){
				result = false;
				break;
			}
		}

		return result;
	}

	bool operator !=(const DynVecN<T>& other) const{
		bool result = true;
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		for (uint c = 0; c < size; c++){
			if (vals[c] == other.vals[c]){
				result = false;
				break;
			}
		}

		return result;
	}

	double getVecLength() const{
		double length = 0;

		for (uint c = 0; c < size; c++){
			length += vals[c] * vals[c];
		}

		return pow(length, 1.0f / size);
	}

	void normalize(){
		double len = getVecLength();

		for (uint c = 0; c < size; c++){
			vals[c] = static_cast<T>(vals[c] / len);
		}
	}

	DynVecN<T> normalizeCopy() const{
		DynVecN<T> result(*this);
		double len = getVecLength();

		for (uint c = 0; c < size; c++){
			result.vals[c] = static_cast<T>(result.vals[c] / len);
		}

		return result;
	}

	T dot(const DynVecN<T>& other) const{
		T result = 0;
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif

		for (uint c = 0; c < size; c++){
			result += vals[c] * other.vals[c];
		}

		return result;
	}

	double angleBetween(const DynVecN<T>& other) const{
#if _DEBUG
		if (size != other.size){
			throw Exception("Incompatible sizes for DynVecN");
		}
#endif
		return acos(dot(other) / (getVecLength() * other.getVecLength()));
	}

	DynVecN<T> reverse() const{
		DynVecN<T> result(size);

		for (uint c = 0; c < size; c++){
			result.vals[size - 1 - c] = vals[c];
		}

		return result;
	}

	DynVecN<T> perpendicular() const{
		DynVecN<T> result(size);

		result[0] = vals[1];
		result[1] = -vals[0];

		return result;
	}

	friend ostream& operator <<(ostream& os, const DynVecN<T>& vec){
		os << "V" << vec.size << "{";

		for (uint c = 0; c < vec.size; c++){
			os << vec.vals[c] << ", ";
		}

		os << "\b\b}";

		return os;
	}

private:
	uint size;
	T* vals;
};

//distance squared between 2 points
float distanceSq(const VecN<float, 2>& p1, const VecN<float, 2>& p2);
float distanceSq(const DynVecN<float>& p1, const DynVecN<float>& p2);

//distance between 2 points
float distance(const VecN<float, 2>& p1, const VecN<float, 2>& p2);
float distance(const DynVecN<float>& p1, const DynVecN<float>& p2);

//assumes rect = {x, y, w, h}
bool intersect(const VecN<int, 4>& rect1, const VecN<int, 4>& rect2);
bool intersect(const DynVecN<int>& rect1, const DynVecN<int>& rect2);

#endif