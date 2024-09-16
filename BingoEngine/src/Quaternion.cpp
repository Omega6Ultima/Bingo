#include "Quaternion.h"

#include <math.h>

using Bingo::Math::Quaternion;

#define ONE_THRESHOLD .999999

Quaternion::Quaternion()
	:values({ 0, 0, 0, 0 }) {
	//
}

Quaternion::Quaternion(double w, double x, double y, double z)
	: values({ w, x, y, z }) {
	//
}

Quaternion::Quaternion(VecN<double, 3> point)
	: values({ 0,  point[0], point[1], point[2] }) {
	//
}

Quaternion::Quaternion(VecN<double, 3> axis, double degrees) {
	VecN<double, 3> normAxis = axis.normalizeCopy();
	double angle = Utils::degToRadians(degrees);
	double i = sin(angle / 2);

	values = { cos(angle / 2), i * normAxis[0], i * normAxis[1], i * normAxis[2] };
}

Quaternion Quaternion::identity() {
	return Quaternion(1, 0, 0, 0);
}

void Quaternion::setW(double w) {
	values[0] = w;
}

void Quaternion::setX(double x) {
	values[1] = x;
}

void Quaternion::setY(double y) {
	values[2] = y;
}

void Quaternion::setZ(double z) {
	values[3] = z;
}

double Quaternion::magnitude() const {
	double total = 0;

	for (uint c = 0; c < 4; c++) {
		total += values[c] * values[c];
	}

	return sqrt(total);
}

double Quaternion::magnitudeSq() const {
	double total = 0;

	for (uint c = 0; c < 4; c++) {
		total += values[c] * values[c];
	}

	return total;
}

Quaternion Quaternion::conjugate() const {
	Quaternion result(*this);

	result.setX(-result.getX());
	result.setY(-result.getY());
	result.setZ(-result.getZ());

	return result;
}

Quaternion Quaternion::inverse() const {
	double mag = magnitude();
	return  conjugate() / (mag * mag);
}

double Quaternion::dot(const Quaternion& other) const {
	return getW() * other.getW() +
		getX() * other.getX() +
		getY() * other.getY() +
		getZ() * other.getZ();
}

Quaternion Quaternion::operator-() const {
	return Quaternion(-getW(), -getX(), -getY(), -getZ());
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
	return Quaternion(
		getW() - other.getW(),
		getX() - other.getX(),
		getY() - other.getY(),
		getZ() - other.getZ()
	);
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
	Quaternion result;

	result.setW(
		getW() * other.getW() -
		getX() * other.getX() -
		getY() * other.getY() -
		getZ() * other.getZ()
	);
	result.setX(
		getW() * other.getX() +
		getX() * other.getW() +
		getY() * other.getZ() -
		getZ() * other.getY()
	);

	result.setY(
		getW() * other.getY() -
		getX() * other.getZ() +
		getY() * other.getW() +
		getZ() * other.getX()
	);
	result.setZ(
		getW() * other.getZ() +
		getX() * other.getY() -
		getY() * other.getX() +
		getZ() * other.getW()
	);

	return result;
}

Quaternion Quaternion::operator*(double other) const {
	Quaternion result(*this);

	for (uint c = 0; c < 4; c++) {
		result.values[c] *= other;
	}

	return result;
}

Quaternion Quaternion::operator/(double other) const {
	Quaternion result(*this);

	for (uint c = 0; c < 4; c++) {
		result.values[c] /= other;
	}

	return result;
}

bool Quaternion::operator==(const Quaternion& other) const {
	return (getW() == other.getW()) &&
		(getX() == other.getX()) &&
		(getY() == other.getY()) &&
		(getZ() == other.getZ());
}

Quaternion Bingo::Math::pow(const Quaternion& q, uint p) {
	Quaternion result(q);

	for (uint c = 1; c < p; c++) {
		result = result * q;
	}

	return result;
}

Quaternion Bingo::Math::slerp(const Quaternion& start, const Quaternion& end, double lerp) {
	Quaternion result;
	double cosAng = start.dot(end);

	if (cosAng < 0.0) {
		cosAng = start.dot(-end);
	}

	double startMul = 1;
	double endMul = 0;

	if (cosAng > ONE_THRESHOLD) {
		//quaternions are very close, use linear interpolation
		startMul = 1.0 - lerp;
		endMul = lerp;
	}
	else {
		double sinAng = sqrt(1 - cosAng * cosAng);
		double ang = atan2(sinAng, cosAng);
		double oneOverSinAng = 1.0 / sinAng;

		startMul = sin((1.0 - lerp) * ang) * oneOverSinAng;
		endMul = sin(lerp * ang) * oneOverSinAng;
	}

	result.setW(start.getW() * startMul + end.getW() * endMul);
	result.setX(start.getX() * startMul + end.getX() * endMul);
	result.setY(start.getY() * startMul + end.getY() * endMul);
	result.setZ(start.getZ() * startMul + end.getZ() * endMul);

	return result;
}

Quaternion Bingo::Math::squad(const Quaternion& q0, const Quaternion& q1, const Quaternion& q2, const Quaternion& q3, double lerp) {
	return slerp(slerp(q0, q1, lerp), slerp(q2, q3, lerp), 2 * lerp * (1 - lerp));
}

ostream& Bingo::Math::operator<<(ostream& os, const Quaternion& q) {
	os << "Q{";
	os << q.getW() << ", ";
	os << q.getX() << ", ";
	os << q.getY() << ", ";
	os << q.getZ() << "}";

	return os;
}