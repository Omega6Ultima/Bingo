//Dustin Gehm
/*A class to bundle red, green, blue, and alpha
components used in colors*/

#pragma once
#ifndef _COLOR_H
#define _COLOR_H

#include "Utils.h"

class Color{
public:
	Color(uchar r, uchar g, uchar b);
	Color(uchar r, uchar g, uchar b, uchar a);
	~Color();

	/*sets the red component*/
	void setRed(uchar r);
	/*returns the red component*/
	inline uchar getRed() const{
		return values[0];
	}

	/*sets the green component*/
	void setGreen(uchar g);
	/*returns the green component*/
	inline uchar getGreen() const{
		return values[1];
	}

	/*sets the blue component*/
	void setBlue(uchar b);
	/*returns the blue component*/
	inline uchar getBlue() const{
		return values[2];
	}

	/*sets the alpha component*/
	void setAlpha(uchar a);
	/*returns the alpha component*/
	inline uchar getAlpha() const{
		return values[3];
	}

	Color inverse() const;
	Color inverseHue() const;
private:
	uchar values[4];
};

const Color BLACK(0, 0, 0);
const Color WHITE(255, 255, 255);
const Color RED(255, 0, 0);
const Color GREEN(0, 255, 0);
const Color BLUE(0, 0, 255);
const Color TRANSPARENT(255, 255, 255, 0);

const Color AQUA(0, 255, 255);
const Color BROWN(165, 42, 42);
const Color CHARTREUSE(127, 255, 0);
const Color CHOCOLATE(210, 105, 30);
const Color CYAN(0, 255, 255);
const Color DARKBLUE(0, 0, 127);
const Color DARKCYAN(0, 139, 139);
const Color DARKGRAY(169, 169, 169);
const Color DARKGREEN(0, 100, 0);
const Color DARKKHAKI(189, 183, 107);
const Color DARKMAGENTA(139, 0, 139);
const Color DARKORANGE(255, 140, 0);
const Color DARKORCHID(153, 50, 204);
const Color DARKRED(139, 0, 0);
const Color DARKSALMON(233, 150, 122);
const Color DARKTURQUOISE(0, 206, 209);
const Color DARKVIOLET(148, 0, 211);
const Color DEEPPINK(255, 20, 147);
const Color DIMGRAY(105, 105, 105);
const Color FIREBRICK(178, 34, 34);
const Color FORESTGREEN(34, 139, 34);
const Color FUSCHIA(255, 0, 255);
const Color GOLD(255, 215, 0);
const Color GRAY(128, 128, 128);
const Color HOTPINK(255, 105, 180);
const Color KHAKI(240, 230, 140);
const Color LIGHTBLUE(173, 216, 230);
const Color LIGHTCYAN(224, 255, 255);
const Color LIGHTGRAY(211, 211, 211);
const Color LIGHTGREEN(144, 238, 144);
const Color LIGHTPINK(255, 182, 193);
const Color LIGHTSALMON(255, 160, 122);
const Color LIGHTSTEELBLUE(176, 196, 222);
const Color LIME(50, 205, 50);
const Color MAGENTA(255, 0, 255);
const Color MAROON(128, 0, 0);
const Color NAVYBLUE(0, 0, 128);
const Color OLIVE(128, 128, 0);
const Color ORANGE(255, 165, 0);
const Color ORCHID(218, 112, 214);
const Color PINK(255, 192, 203);
const Color PLUM(221, 160, 221);
const Color POWDERBLUE(176, 224, 230);
const Color PURPLE(128, 0, 128);
const Color ROYALBLUE(65, 105, 255);
const Color SALMON(250, 128, 114);
const Color SEAGREEN(46, 139, 87);
const Color SILVER(192, 192, 192);
const Color SKYBLUE(135, 206, 235);
const Color SPRINGGREEN(0, 255, 127);
const Color STEELBLUE(70, 130, 180);
const Color TAN(210, 180, 140);
const Color TEAL(0, 128, 128);
const Color TURQUIOSE(64, 224, 208);
const Color VIOLET(238, 130, 238);
const Color YELLOW(255, 255, 0);

#endif