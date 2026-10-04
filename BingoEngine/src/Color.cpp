//Dustin Gehm

#include "Color.h"
#include "Utils.h"

using Bingo::Colors::Color;
using Bingo::Utils::Warn;

Color::Color(uchar r, uchar g, uchar b) {
	values[0] = r;
	values[1] = g;
	values[2] = b;
	values[3] = 255;
}

Color::Color(uchar r, uchar g, uchar b, uchar a) {
	values[0] = r;
	values[1] = g;
	values[2] = b;
	values[3] = a;
}

Color::~Color() {
	//
}

void Color::setRed(uchar r) {
	values[0] = r;
}

void Color::setGreen(uchar g) {
	values[1] = g;
}

void Color::setBlue(uchar b) {
	values[2] = b;
}

void Color::setAlpha(uchar a) {
	values[3] = a;
}

Color Color::inverse() const {
	Color result(255 - getRed(), 255 - getGreen(), 255 - getBlue(), getAlpha());

	return result;
}

Color Color::inverseHue() const {
	Color result(255 - getBlue(), 255 - getGreen(), 255 - getRed(), getAlpha());

	return result;
}

bool Color::match(const Color& other) const {
	return getRed() == other.getRed() &&
			getGreen() == other.getGreen() &&
			getBlue() == other.getBlue();
}

bool Color::exactMatch(const Color& other) const {
	return getRed() == other.getRed() &&
			getGreen() == other.getGreen() &&
			getBlue() == other.getBlue()&&
			getAlpha() == other.getAlpha();
}

bool Color::operator==(const Color& other) const {
	return match(other);
}

bool Color::operator!=(const Color& other) const {
	return !match(other);
}

Color Color::operator*(const uchar other) const {
	return operator*(other / 255.0);
}

Color Color::operator*(const double other) const {
#if _DEBUG
	if (other < 0 || other > 1) {
		Warn("Color modulator is not in the range 0 - 1\n");
	}
#endif
	return Color(static_cast<uchar>(getRed() * other),
				static_cast<uchar>(getGreen() * other),
				static_cast<uchar>(getBlue() * other),
				static_cast<uchar>(getAlpha() * other));
}

ostream& Bingo::Colors::operator<<(ostream& os, const Color& color) {
	os << "Color{r" << +color.values[0]
		<< ", g" << +color.values[1]
		<< ", b" << +color.values[2]
		<< ", a" << +color.values[3]
		<< "}";

	return os;
}