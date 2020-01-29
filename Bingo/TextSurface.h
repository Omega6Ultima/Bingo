//Dustin Gehm
//A subclass of Surface meant for rendered text

#pragma once
#ifndef _TEXTSURFACE_H
#define _TEXTSURFACE_H

#include "FontManager.h"
#include "Surface.h"

class TextSurface : public Surface{
public:
	TextSurface(string fontName, int size, string text, Color color);
	~TextSurface();

	void setFont(string fontName);
	inline string getFont() const{
		return font;
	}

	void setFontSize(int size);
	inline int getFontSize() const{
		return fontSize;
	}

	inline int getLetterHeight() const{
		return TTF_FontHeight(FontManager::getSingleton().getFont(font, fontSize));
	}

	inline int getLetterWidth(char ch) const{
		int minX, maxX;

		if (TTF_GlyphMetrics(FontManager::getSingleton().getFont(font, fontSize), ch, &minX, &maxX, NULL, NULL, NULL) == 0){
			return maxX - minX;
		}

		return -1;
	}

	inline int getTextWidth() const{
		int width;

		if (TTF_SizeText(FontManager::getSingleton().getFont(font, fontSize), text.c_str(), &width, NULL) == 0){
			return width;
		}

		return -1;
	}

	inline int getTextHeight() const{
		int height;

		if (TTF_SizeText(FontManager::getSingleton().getFont(font, fontSize), text.c_str(), NULL, &height) == 0){
			return height;
		}

		return -1;
	}

	void setText(string text);
	inline string getText() const{
		return text;
	}

	void setTextPadding(uint pad);
	inline uint getTextPadding() const{
		return textPadding;
	}

	void setColor(Color textColor);
	inline Color getColor() const{
		return color;
	}

	void setBackgroundColor(Color color);
	inline Color getBackgroundColor() const{
		return backgroundColor;
	}

protected:
	virtual void renderTexture() override;

private:
	string font = "";
	int fontSize = 1;
	string text = "";
	uint textPadding = 0;
	Color color = BLACK;
	Color backgroundColor = TRANSPARENT;
};

#endif