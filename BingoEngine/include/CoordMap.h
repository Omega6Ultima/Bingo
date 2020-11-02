//Dustin Gehm

#ifndef _COORDMAP_H
#define _COORDMAP_H

//#include <hash_map>
#include <map>

#include "VecN.h"

//using std::hash_map;
using std::map;

//template<class T>
//class CoordMap {
//public:
//	CoordMap<T>() {
//		//
//	}
//
//	~CoordMap<T>() {
//		//
//	}
//
//	void insert(int x, int y, T val) {
//		vals[VecN<int, 2>({ x, y })] = new T(val);
//	}
//
//	bool find(int x, int y) {
//		return vals.find(VecN<int, 2>(x, y)) != vals.end();
//	}
//
//	T get(int x, int y) const {
//		return *vals[VecN<int, 2>({ x, y })];
//	}
//
//	T& get(int x, int y) {
//		return *vals[VecN<int, 2>({ x, y })];
//	}
//
//	T operator<<(uint index) const {
//		return *(vals.begin() + index);
//	}
//
//	T& operator<<(uint index) {
//		return *(vals.begin() + index);
//	}
//
//	uint getSize() const {
//		return vals.size();
//	}
//
//	vector<T*> getNeighbors(int x, int y) {
//		vector<T*> neighbors(8, NULL);
//
//		for (uint c = 0; c < neighbors.size(); c++) {
//			if (find(x, y)) {
//				neighbors[c] = &get(x, y);
//			}
//		}
//
//		return neighbors;
//	}
//
//private:
//	//hash_map<VecN<int, 2>, T*> vals;
//	map<VecN<int, 2>, T*> vals;
//};

#endif