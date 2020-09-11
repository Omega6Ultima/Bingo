//Dustin Gehm

#ifndef _COORDMAP_H
#define _COORDMAP_H

//#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS
//
//#include <hash_map>
//
//#include "VecN.h"
//
//using std::hash_map;
//
//template<class T>
//class CoordMap {
//public:
//	CoordMap<T>() {
//		//
//	}
//
//	~CoordMap<T>();
//
//	void insert(int x, int y, T val) {
//		vals[VecN<int, 2>({ x, y })] = val;
//	}
//
//	bool find(int x, int y) {
//		return vals.find(VecN<int, 2>(x, y)) != vals.end();
//	}
//
//	T get(int x, int y) {
//		return vals[VecN<int, 2>({ x, y })];
//	}
//
//private:
//	//what about storing a grid of nodes
//	//where the actual position in the grid is the coords
//	//and there are pointers that point to neighboring nodes
//	hash_map<VecN<int, 2>, T> vals;
//};

#endif