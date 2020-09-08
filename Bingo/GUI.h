//Dustin Gehm

#pragma once
#ifndef _GUI_H
#define _GUI_H

#include <vector>

#include "Color.h"
#include "EventManager.h"
#include "TextSurface.h"
#include "Timer.h"

#define BUTTON_PRIORITY 10
#define INPUT_PRIORITY 10
#define INPUT_CURSOR_ACTIVE_TIME 500
#define INPUT_CURSOR_MAX_TIME 1000

using std::distance;
using std::find;
using std::vector;

class Button;
class Input;

template<typename T>
class Slider;

class DropDown;

typedef void(*ButtonFunc)(Button& button, EventManager::MouseButton mouseButton);
typedef void(*InputFunc)(Input& input, EventManager::MouseButton mouseButton);

template<typename T>
using SliderFunc = void(*)(Slider<T>& slider);

typedef void(*DropDownFunc)(DropDown& dropDown);

typedef TextSurface Label;

class Button : public TextSurface, public MouseListener {
public:
	Button(int x, int y, ButtonFunc func, string fontName, int size, string text, Color color = BLACK);
	Button(VecN<int, 2> position, ButtonFunc func, string fontName, int size, string text, Color color = BLACK);
	~Button() = default;

	void setFrameColor(Color color);
	inline Color getFrameColor() const {
		return frameColor;
	}

	void setFrameWidth(uint width);
	inline uint getFrameWidth() const {
		return frameWidth;
	}

	//void setShadowText(string str);
	//inline string getShadowText() const{
	//	return shadowText;
	//}

protected:
	virtual void renderTexture() override;

private:
	virtual void handleEvent(EventManager::EventType evt) override;

private:
	Color frameColor = BLACK;
	bool beingClicked = false;
	uint frameWidth = 1;
	ButtonFunc onButtonClick;
};

class Input : public Button, public KeyListener {
public:
	Input(int x, int y, int width, int height, InputFunc func, string fontName, int fontSize, string startText, Color color = BLACK);
	Input(VecN<int, 2> position, int width, int height, InputFunc func, string fontName, int fontSize, string startText, Color color = BLACK);
	~Input() = default;

	void inputMode();

protected:
	virtual void renderTexture() override;

private:
	virtual void handleEvent(EventManager::EventType evt) override;

private:
	bool input = false;
	string shadowText;
	int inputWidth, inputHeight;
	InputFunc onInputClick;
	Timer timer;
	uint cursorTime = 0;
	uint cursorWidth;
};

template<typename T>
class Slider : public Button {
public:
	enum class Orientation {
		HORIZONTAL,
		VERTICAL,
	};

	Slider(int x, int y, int w, int h, T start, T stop, T step, SliderFunc<T> func, string fontName, int fontSize, Color color = BLACK)
		: Button(x, y, NULL, fontName, fontSize, "", color) {
		width = w;
		height = h;

		for (T c = start; c <= stop; c += step) {
			values.push_back(c);
		}

		onChange = func;
		pixelStep = static_cast<float>(width) / (values.size() + 1);

		renderTexture();
	}

	Slider(VecN<int, 2> position, int width, int height, T start, T stop, T step, SliderFunc<T> func, string fontName, int fontSize, Color color = BLACK)
		: Button(position, NULL, fontName, fontSize, "", color) {
		width = w;
		height = h;

		for (T c = start; c <= stop; c += step) {
			values.push_back(c);
		}

		onChange = func;
		pixelStep = static_cast<float>(width) / (values.size() + 1);

		renderTexture();
	}

	~Slider() = default;

	void increment() {
		if (curIndex < values.size() - 1) {
			curIndex++;
			markDirty();

			if (onChange) {
				onChange(*this);
			}
		}
	}

	void decrement() {
		if (curIndex > 0) {
			curIndex--;
			markDirty();

			if (onChange) {
				onChange(*this);
			}
		}
	}

	void setValue(T value) {
		auto iter = find(values.begin(), values.end(), value);

		if (iter != values.end()) {
			curIndex = distance(values.begin(), iter);
			markDirty();

			if (onChange) {
				onChange(*this);
			}
		}
	}

	inline T getValue() const {
		return values[curIndex];
	}

	void setOrientation(Orientation ori) {
		orientation = ori;

		switch (orientation) {
		case Orientation::HORIZONTAL:
			pixelStep = static_cast<float>(width) / (values.size() + 1);
			break;
		case Orientation::VERTICAL:
			pixelStep = static_cast<float>(height) / (values.size() + 1);
			break;
		}

		markDirty();
	}

	inline Orientation getOrientation() const {
		return orientation;
	}

	inline void operator++() {
		increment();
	}

	inline void operator--() {
		decrement();
	}

	inline void operator++(int) {
		increment();
	}

	inline void operator--(int) {
		decrement();
	}

protected:
	virtual void renderTexture() override {
		if (texture) {
			SDL_DestroyTexture(texture);
			texture = NULL;
		}

		texture = createTexture(width, height);
		setBlendMode(BLEND_BLEND);

		//TextSurface minText(getFont(), getFontSize(), toString(values[0]), getColor());
		//TextSurface maxText(getFont(), getFontSize(), toString(values[values.size() - 1]), getColor());
		//TextSurface curText(getFont(), getFontSize(), toString(values[curIndex]), getColor());

		Surface::saveRenderTarget();

		setRenderTarget();
		clear();
		fill(getBackgroundColor());
		setDrawColor(getColor());

		drawRect(0, 0, width, height, false);

		if (orientation == Orientation::HORIZONTAL) {
			//draw the main horizontal line
			drawLine(0, height / 2, width, height / 2);

			//draw the lines on the ends of the main line
			drawLine(0, 0, 0, height);
			drawLine(width - 1, 0, width - 1, height);

			for (uint c = 0; c < values.size(); c++) {
				int x = static_cast<int>((c + 1) * pixelStep);

				drawLine(x, height / 4, x, height * 3 / 4);
			}

			drawDiamond(static_cast<int>(pixelStep * (curIndex + 1)), height / 2, 10, height / 2);

			//draw(minText, static_cast<int>(pixelStep), height - minText.getHeight());
			//draw(maxText, static_cast<int>(pixelStep * values.size()), height - maxText.getHeight());
			//draw(curText, static_cast<int>(pixelStep * (curIndex + 1)), height - curText.getHeight());
		}
		else if (orientation == Orientation::VERTICAL) {
			//draw the main vertical line
			drawLine(width / 2, 0, width / 2, height);

			//draw the lines on the ends of the main line
			drawLine(0, 0, width, 0);
			drawLine(0, height - 1, width, height - 1);

			for (uint c = 0; c < values.size(); c++) {
				int y = static_cast<int>((c + 1) * pixelStep);

				drawLine(width / 4, y, width * 3 / 4, y);
			}

			drawDiamond(width / 2, static_cast<int>(pixelStep * (curIndex + 1)), width / 2, 10);

			//draw(minText, width - minText.getWidth(), static_cast<int>(pixelStep));
			//draw(maxText, width - maxText.getWidth(), static_cast<int>(pixelStep * values.size()));
			//draw(curText, width - curText.getWidth(), static_cast<int>(pixelStep * (curIndex + 1)));
		}

		Surface::restoreRenderTarget();
	}

	string toString(T val) {
		return "";
	}

private:
	virtual void handleEvent(EventManager::EventType evt) override {
		MouseListener::handleEvent(evt);

		if (evt == EventManager::EVT_MOUSEBUTTONDOWN) {
			VecN<int, 2> mousePos = getMousePos();

			if (pos[0] <= mousePos[0] && mousePos[0] <= pos[0] + width) {
				if (pos[1] <= mousePos[1] && mousePos[1] <= pos[1] + height) {
					if (checkMouseButtonDown(EventManager::MB_LEFT, 1000)) {
						if (orientation == Orientation::HORIZONTAL) {
							if (mousePos[0] < pos[0] + pixelStep * (curIndex + 1)) {
								decrement();
							}
							else if (mousePos[0] > pos[0] + pixelStep * (curIndex + 1)) {
								increment();
							}
						}
						else if (orientation == Orientation::VERTICAL) {
							if (mousePos[1] < pos[1] + pixelStep * (curIndex + 1)) {
								decrement();
							}
							else if (mousePos[1] > pos[1] + pixelStep * (curIndex + 1)) {
								increment();
							}
						}
					}
				}
			}
		}
	}

private:
	vector<T> values;
	uint curIndex = 0;
	float pixelStep;
	SliderFunc<T> onChange;
	Orientation orientation = Orientation::HORIZONTAL;
};

string Slider<char>::toString(char val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%c", val);

	return result;
}

string Slider<uchar>::toString(uchar val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%c", val);

	return result;
}

string Slider<short>::toString(short val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%d", val);

	return result;
}

string Slider<ushort>::toString(ushort val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%d", val);

	return result;
}

string Slider<int>::toString(int val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%d", val);

	return result;
}

string Slider<uint>::toString(uint val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%d", val);

	return result;
}

string Slider<long>::toString(long val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%d", val);

	return result;
}

string Slider<ulong>::toString(ulong val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%d", val);

	return result;
}

string Slider<ullong>::toString(ullong val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%llu", val);

	return result;
}

string Slider<float>::toString(float val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%f", val);

	return result;
}

string Slider<double>::toString(double val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%f", val);

	return result;
}

string Slider<ldouble>::toString(ldouble val) {
	char result[33];
	memset(result, 0, sizeof(result));

	sprintf_s(result, "%f", val);

	return result;
}

class DropDown : public Button {
public:
	DropDown(int x, int y, uint w, uint h, vector<string>* values, DropDownFunc func, string fontName, int size, Color color = BLACK);
	~DropDown() = default;

	void addValue(const string& val);
	void removeValue(const string& val);

	void setText(string) = delete;
	inline string getText() const {
		if (0 <= selection && selection < options.size()) {
			return options[selection];
		}
		else {
			return "";
		}
	}

	void setValue(uint index);
	inline string getValue(uint index) const {
#ifdef _DEBUG
		if (index >= options.size()) {
			throw Exception("Index out of range");
		}
#endif
		return options[index];
	}

	inline string getValue() const {
		return options[selection];
	}

protected:
	virtual void renderTexture() override;

private:
	virtual void handleEvent(EventManager::EventType evt) override;

private:
	bool drop = false;
	int dropDownWidth, dropDownHeight;
	vector<string> options;
	DropDownFunc onChange;
	uint selection = 0;

};

#endif