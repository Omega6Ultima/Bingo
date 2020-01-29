//Dustin Gehm

#include "Color.h"

Color::Color(uchar r, uchar g, uchar b){
	values[0] = r;
	values[1] = g;
	values[2] = b;
	values[3] = 255;
}

Color::Color(uchar r, uchar g, uchar b, uchar a){
	values[0] = r;
	values[1] = g;
	values[2] = b;
	values[3] = a;
}

Color::~Color(){
	//
}

void Color::setRed(uchar r){
	values[0] = r;
}

void Color::setGreen(uchar g){
	values[1] = g;
}

void Color::setBlue(uchar b){
	values[2] = b;
}

void Color::setAlpha(uchar a){
	values[3] = a;
}

Color Color::inverse() const{
	Color result(255 - getRed(), 255 - getGreen(), 255 - getBlue(), getAlpha());

	return result;
}

Color Color::inverseHue() const{
	Color result(255 - getBlue(), 255 - getGreen(), 255 - getRed(), getAlpha());

	return result;
}