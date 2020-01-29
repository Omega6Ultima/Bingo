//Dustin Gehm

#include "VecN.h"

#include <math.h>

using std::sqrt;

float distanceSq(const VecN<float, 2>& p1, const VecN<float, 2>& p2){
	float diffX = p2[0] - p1[0];
	float diffY = p2[1] - p1[1];

	return diffX * diffX + diffY * diffY;
}

float distanceSq(const DynVecN<float>& p1, const DynVecN<float>& p2){
#if _DEBUG
	if (p1.getSize() != 2 || p2.getSize() != 2){
		throw Exception("DynVecN's are not points.");
	}
#endif
	float diffX = p2[0] - p1[0];
	float diffY = p2[1] - p1[1];

	return diffX * diffX + diffY * diffY;
}

float distance(const VecN<float, 2>& p1, const VecN<float, 2>& p2){
	float diffX = p2[0] - p1[0];
	float diffY = p2[1] - p1[1];

	return sqrt(diffX * diffX + diffY * diffY);
}

float distance(const DynVecN<float>& p1, const DynVecN<float>& p2){
#if _DEBUG
	if (p1.getSize() != 2 || p2.getSize() != 2){
		throw Exception("DynVecN's are not points.");
	}
#endif
	float diffX = p2[0] - p1[0];
	float diffY = p2[1] - p1[1];

	return sqrt(diffX * diffX + diffY * diffY);
}

bool intersect(const VecN<int, 4>& rect1, const VecN<int, 4>& rect2){
	if (rect1[0] > rect2[0] + rect2[2] || rect2[0] > rect1[0] + rect1[2]){
		return false;
	}

	if (rect1[1] > rect2[1] + rect2[3] || rect2[1] > rect1[1] + rect1[3]){
		return false;
	}

	return true;
}

bool intersect(const DynVecN<int>& rect1, const DynVecN<int>& rect2){
#if _DEBUG
	if (rect1.getSize() != 4 || rect2.getSize() != 4){
		throw Exception("DynVecN's are not rects.");
	}
#endif
	if (rect1[0] > rect2[0] + rect2[2] || rect2[0] > rect1[0] + rect1[2]){
		return false;
	}

	if (rect1[1] > rect2[1] + rect2[3] || rect2[1] > rect1[1] + rect1[3]){
		return false;
	}

	return true;
}