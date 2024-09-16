//Dustin Gehm

#include "GUI.h"
#include "Mathematics.h"
#include "WindowManager.h"

#include <SDL_events.h>

using Bingo::Guis::GuiBase;
using Bingo::Guis::Button;
using Bingo::Guis::ButtonText;
using Bingo::Guis::DropDown;
using Bingo::Guis::Input;
using Bingo::Guis::Slider;
using Bingo::Math::DistanceSq;
using Bingo::Math::RotatePoint;
using Bingo::Surfaces::WindowManager;

void GuiBase::setFrameColor(Color color) {
	frameColor = color;
}

void GuiBase::setFrameWidth(uint width) {
	frameWidth = width;
}

void GuiBase::enable() {
	enabled = true;
}

void GuiBase::disable() {
	enabled = false;
}

void GuiBase::hide() {
	hidden = true;
}

void GuiBase::show() {
	hidden = false;
}

Button::Button(int x, int y, ButtonFunc func, uint width, uint height, Color color)
	: Surface(width, height) {
	pos = { static_cast<float>(x), static_cast<float>(y) };
	onButtonClick = func;

	setFrameColor(color);

	renderTexture();

	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_MOUSE);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_MOUSE, BUTTON_PRIORITY);
}

Button::Button(int x, int y, ButtonFunc func, string imgFile, Color color)
	: Surface(imgFile) {
	pos = { static_cast<float>(x), static_cast<float>(y) };
	onButtonClick = func;

	setFrameColor(color);

	renderTexture();

	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_MOUSE);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_MOUSE, BUTTON_PRIORITY);
}

void Button::setFrameColor(Color color) {
	GuiBase::setFrameColor(color);

	markDirty();
}

void Button::setFrameWidth(uint width) {
	GuiBase::setFrameWidth(width);

	markDirty();
}

void Button::hide() {
	GuiBase::hide();

	markDirty();
}

void Button::show() {
	GuiBase::show();

	markDirty();
}

void Button::renderTexture() {
	if (getHidden()) {
		return;
	}

	Surface::renderTexture();

	Surface::saveRenderTarget();

	setRenderTarget();
	setDrawColor(getFrameColor());

	for (uint c = 0; c < getFrameWidth(); c++) {
		drawRect(c, c, width - 2 * c, height - 2 * c, false);
	}

	{
		VecN<int, 2> minPos = getPos();
		VecN<int, 2> rotPoint = getCenter() + minPos;
		uint degrees = getRotation();
		VecN<VecN<int, 2>, 4> points;

		points[0] = RotatePoint(minPos, rotPoint, degrees);
		points[1] = RotatePoint({ minPos[0] + getWidth(), minPos[1] }, rotPoint, degrees);
		points[2] = RotatePoint({ minPos[0], minPos[1] + getHeight() }, rotPoint, degrees);
		points[3] = RotatePoint({ minPos[0] + getWidth(), minPos[1] + getHeight() }, rotPoint, degrees);

		drawCircle(points[0], 5, false);
		drawCircle(points[1], 5, false);
		drawCircle(points[2], 5, false);
		drawCircle(points[3], 5, false);
	}

	Surface::restoreRenderTarget();
}

void Button::handleEvent(EventManager::EventType evt) {
	if (getDisabled()) {
		return;
	}

	MouseListener::handleEvent(evt);

	if (evt == EventManager::EVT_MOUSEBUTTONDOWN) {
		VecN<int, 2> mousePos = getMousePos();

		if (checkPointWithin(mousePos)) {
			beingClicked = true;
			markDirty();

			if (checkMouseButtonDown(EventManager::MB_LEFT, 1000) ||
				checkMouseButtonDown(EventManager::MB_RIGHT, 1000) ||
				checkMouseButtonDown(EventManager::MB_CENTER, 1000)) {
				setFrameColor(getFrameColor().inverse());
				markDirty();
			}

			if (onButtonClick) {
				onButtonClick(*this, static_cast<EventManager::MouseButton>(eventData.mouseButton));
			}
		}
	}
	else if (evt == EventManager::EVT_MOUSEBUTTONUP) {
		if (beingClicked) {
			setFrameColor(getFrameColor().inverse());
			markDirty();
		}

		beingClicked = false;
	}
}

bool Button::checkPointWithin(VecN<int, 2> point) {
	if (getRotation() == 0) {
		VecN<int, 2> minPos = getPos();

		return (minPos[0] <= point[0] && point[0] <= minPos[0] + getWidth()) && (minPos[1] <= point[1] && point[1] <= minPos[1] + getHeight());
	}
	else {
		VecN<int, 2> minPos = getPos();
		VecN<int, 2> rotPoint = getCenter() + minPos;
		uint degrees = getRotation();
		VecN<VecN<int, 2>, 4> points;

		points[0] = RotatePoint(minPos, rotPoint, degrees);
		points[1] = RotatePoint({ minPos[0] + getWidth(), minPos[1] }, rotPoint, degrees);
		points[2] = RotatePoint({ minPos[0], minPos[1] + getHeight() }, rotPoint, degrees);
		points[3] = RotatePoint({ minPos[0] + getWidth(), minPos[1] + getHeight() }, rotPoint, degrees);

		return Math::PointRectIntersect(point, points[0], points[1], points[2], points[3]);
	}

	return false;
}

ButtonText::ButtonText(int x, int y, ButtonTextFunc func, string fontName, int size, string text, Color color)
	: TextSurface(fontName, size, text, color) {
	pos = { static_cast<float>(x), static_cast<float>(y) };
	onButtonClick = func;

	setFrameColor(color);

	renderTexture();

	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_MOUSE);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_MOUSE, BUTTON_PRIORITY);
}

ButtonText::ButtonText(VecN<int, 2> position, ButtonTextFunc func, string fontName, int size, string text, Color color)
	: TextSurface(fontName, size, text, color) {
	pos = position;
	onButtonClick = func;

	setFrameColor(color);

	renderTexture();

	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_MOUSE);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_MOUSE, BUTTON_PRIORITY);
}

void ButtonText::setFrameColor(Color color) {
	GuiBase::setFrameColor(color);

	markDirty();
}

void ButtonText::setFrameWidth(uint width) {
	GuiBase::setFrameWidth(width);

	setTextPadding(static_cast<uint>(width * 1.25) + 3);

	markDirty();
}

void ButtonText::hide() {
	GuiBase::hide();

	markDirty();
}

void ButtonText::show() {
	GuiBase::show();

	markDirty();
}

//void ButtonText::setShadowText(string str){
//	shadowText = str;
//
//	markDirty();
//}

void ButtonText::renderTexture() {
	if (getHidden()) {
		return;
	}

	TextSurface::renderTexture();

	Surface::saveRenderTarget();

	setRenderTarget();
	setDrawColor(getFrameColor());

	for (uint c = 0; c < getFrameWidth(); c++) {
		drawRect(c, c, width - 2 * c, height - 2 * c, false);
	}

	Surface::restoreRenderTarget();
}

void ButtonText::handleEvent(EventManager::EventType evt) {
	if (getDisabled()) {
		return;
	}

	MouseListener::handleEvent(evt);

	if (evt == EventManager::EVT_MOUSEBUTTONDOWN) {
		VecN<int, 2> mousePos = getMousePos();

		if (pos[0] <= mousePos[0] && mousePos[0] <= pos[0] + width) {
			if (pos[1] <= mousePos[1] && mousePos[1] <= pos[1] + height) {
				beingClicked = true;

				if (checkMouseButtonDown(EventManager::MB_LEFT, 1000) ||
					checkMouseButtonDown(EventManager::MB_RIGHT, 1000) ||
					checkMouseButtonDown(EventManager::MB_CENTER, 1000)) {
					setFrameColor(getFrameColor().inverse());
					TextSurface::setBackgroundColor(getBackgroundColor().inverse());
					markDirty();
				}

				if (onButtonClick) {
					onButtonClick(*this, static_cast<EventManager::MouseButton>(eventData.mouseButton));
				}
			}
		}
	}
	else if (evt == EventManager::EVT_MOUSEBUTTONUP) {
		if (beingClicked) {
			setFrameColor(getFrameColor().inverse());
			TextSurface::setBackgroundColor(getBackgroundColor().inverse());
			markDirty();
		}

		beingClicked = false;
	}
}

Input::Input(int x, int y, int width, int height, InputFunc func, string fontName, int fontSize, string startText, Color color)
	: ButtonText(x, y, NULL, fontName, fontSize, startText, color) {
	inputWidth = width;
	inputHeight = height;
	onInputClick = func;
	cursorTime = 0;
	cursorWidth = getLetterWidth('I');

	//renderTexture();

	//unregister the registration from ButtonText constructor
	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_KEY);

	//register with higher priority
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_KEY, INPUT_PRIORITY);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_SYSTEM, INPUT_PRIORITY);
}

Input::Input(VecN<int, 2> position, int width, int height, InputFunc func, string fontName, int fontSize, string startText, Color color)
	: ButtonText(position, NULL, fontName, fontSize, startText, color) {
	inputWidth = width;
	inputHeight = height;
	onInputClick = func;
	cursorTime = 0;
	cursorWidth = getLetterWidth('I');

	renderTexture();

	//unregister the registration from ButtonText constructor
	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_KEY);

	//register with higher priority
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_KEY, INPUT_PRIORITY);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_SYSTEM, INPUT_PRIORITY);
}

void Input::inputMode() {
	input = true;
	cursorTime = 0;
	bool drawingCursor = false;

	//TODO
	// use new events SDL_TEXT_INPUT

	while (input) {
		cursorTime += timer.end();
		timer.start();

		EventManager::getSingleton().update();

		if (cursorTime > INPUT_CURSOR_ACTIVE_TIME && !drawingCursor) {
			drawingCursor = true;
			markDirty();
		}

		if (cursorTime > INPUT_CURSOR_MAX_TIME) {
			drawingCursor = false;
			cursorTime = 0;
			markDirty();
		}

		clearRenderTarget();

		WindowManager::getSingleton().clear();
		WindowManager::getSingleton().draw(*this, getPosX(), getPosY());
		WindowManager::getSingleton().update();
	}

	cursorTime = 0;
	markDirty();
}

void Input::renderTexture() {
	if (getHidden()) {
		return;
	}

	TextSurface::renderTexture();

	SDL_Texture* tex = texture;

	texture = createTexture(MAX(width, inputWidth), MAX(height, inputHeight));
	setBlendMode(BLEND_BLEND);

	Surface::saveRenderTarget();

	setRenderTarget();
	clear();
	fill(getBackgroundColor());

	SDL_Rect dest = { 0, 0, width, height };
	SDL_RenderCopy(WindowManager::getSingleton().getRenderer(), tex, NULL, &dest);

	width = MAX(width, inputWidth);
	height = MAX(height, inputHeight);

	setDrawColor(getFrameColor());

	for (uint c = 0; c < getFrameWidth(); c++) {
		drawRect(c, c, width - 2 * c, height - 2 * c, false);
	}

	if (cursorTime > INPUT_CURSOR_ACTIVE_TIME) {
		setDrawColor(getColor());

		uint textPad = static_cast<uint>(getFrameWidth() * 1.25) + 3;
		drawRect(textPad + getTextWidth(), textPad, cursorWidth, height - 2 * textPad, true);
	}

	Surface::restoreRenderTarget();

	SDL_DestroyTexture(tex);
}

void Input::handleEvent(EventManager::EventType evt) {
	if (getDisabled()) {
		return;
	}

	MouseListener::handleEvent(evt);
	KeyListener::handleEvent(evt);

	if (evt == EventManager::EVT_MOUSEBUTTONDOWN) {
		VecN<int, 2> mousePos = getMousePos();

		if (pos[0] < mousePos[0] && mousePos[0] < pos[0] + width &&
			pos[1] < mousePos[1] && mousePos[1] < pos[1] + height) {
			if (onInputClick) {
				onInputClick(*this, static_cast<EventManager::MouseButton>(eventData.mouseButton));
			}
			else {
				inputMode();
			}
		}
		else {
			if (input) {
				input = false;
				EventManager::getSingleton().cancelEvent();
			}
		}
	}
	//else if (evt == EventManager::EVT_QUIT){
	//	if (input){
	//		input = false;
	//		EventManager::getSingleton().cancelEvent();
	//	}
	//}
	else if (evt == EventManager::EVT_KEYDOWN) {
		if (eventData.keyCode == EventManager::K_ESCAPE ||
			eventData.keyCode == EventManager::K_RETURN) {
			if (input) {
				input = false;
				EventManager::getSingleton().cancelEvent();
			}
		}
		else if (input) {
			if (eventData.keyCode == EventManager::K_BACKSPACE) {
				string text = getText();

				if (text.length() > 0) {
					setText(string(text.begin(), text.end() - 1));
				}
			}
			else {
				char ch = getKeySymbol(static_cast<EventManager::KeyCode>(eventData.keyCode), static_cast<EventManager::KeyMod>(eventData.mods));

				if (ch != 0) {
					setText(getText() + ch);
				}
			}
		}
	}
}

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

DropDown::DropDown(int x, int y, uint w, uint h, vector<string>* values, DropDownFunc func, string fontName, int size, Color color)
	: ButtonText(x, y, NULL, fontName, size, "", color) {
	if (values && !values->empty()) {
		options = *values;
	}

	onChange = func;

	dropDownWidth = w;
	dropDownHeight = h;
}

void DropDown::addValue(const string& val) {
	options.push_back(val);

	markDirty();
}

void DropDown::removeValue(const string& val) {
	for (auto iter = options.begin(); iter != options.end(); iter++) {
		if (*iter == val) {
			iter = options.erase(iter);
		}
	}

	markDirty();
}

void DropDown::setValue(uint index, string value) {
#ifdef _DEBUG
	if (index >= options.size()) {
		throw Exception("Index out of range");
	}
#endif
	//selection = index;
	options[index] = value;

	markDirty();
}

void DropDown::setSelection(uint index) {
#ifdef _DEBUG
	if (index >= options.size()) {
		throw Exception("Index out of range");
	}
#endif
	selection = index;

	markDirty();
}

void DropDown::renderTexture() {
	if (getHidden()) {
		return;
	}

	bool optionsEmpty = options.empty();
	size_t optionsSize = options.size();

	if (!optionsEmpty && 0 <= selection && selection < optionsSize) {
		TextSurface::setText(options[selection]);
	}

	TextSurface::renderTexture();

	SDL_Texture* tex = texture;

	int maxWidth = MAX(width, dropDownWidth);
	int maxHeight = MAX(height, dropDownHeight);

	if (!optionsEmpty && drop) {
		texture = createTexture(maxWidth,
			maxHeight * static_cast<int>(optionsSize));
	}
	else {
		texture = createTexture(maxWidth, maxHeight);
	}

	Surface::saveRenderTarget();

	setRenderTarget();

	setBlendMode(BLEND_BLEND);
	clear();
	fill(getBackgroundColor());

	SDL_Rect dest = { 0, 0, width, maxHeight };
	SDL_RenderCopy(WindowManager::getSingleton().getRenderer(), tex, NULL, &dest);

	TextSurface optionsText(getFont(), getFontSize(), "", getColor());
	optionsText.setTextPadding(getTextPadding());

	int yLevel = dropDownHeight;

	if (drop) {
		for (uint c = 0; c < optionsSize; c++) {
			if (c != selection) {
				optionsText.setText(options[c]);

				draw(optionsText, 0, yLevel);

				setDrawColor(getColor());
				drawLine(0, yLevel, maxWidth, yLevel);

				yLevel += dropDownHeight;
			}
		}
	}

	width = maxWidth;
	height = maxHeight;

	setDrawColor(getFrameColor());

	for (uint c = 0; c < getFrameWidth(); c++) {
		drawRect(c, c, width - 2 * c, height - 2 * c, false);
	}

	drawRect(static_cast<int>(width * .90f), 0, static_cast<int>(width * .10f), height, true);

	setDrawColor(getFrameColor().inverse());
	drawTriangle(static_cast<int>(width * .95f), height / 2, static_cast<uint>(width * .05f), static_cast<uint>(height * .60f), 180.0f, true);

	Surface::restoreRenderTarget();

	SDL_DestroyTexture(tex);

	if (!optionsEmpty && drop) {
		height = maxHeight * optionsSize;
	}
}

void DropDown::handleEvent(EventManager::EventType evt) {
	if (getDisabled()) {
		return;
	}

	MouseListener::handleEvent(evt);

	if (evt == EventManager::EVT_MOUSEBUTTONDOWN) {
		if (eventData.mouseButton == EventManager::MB_LEFT) {
			VecN<int, 2> mousePos = getMousePos();

			if (pos[0] < mousePos[0] && mousePos[0] < pos[0] + width) {
				if (pos[1] < mousePos[1] && mousePos[1] < pos[1] + dropDownHeight) {
					drop = !drop;

					markDirty();

					EventManager::getSingleton().cancelEvent();
				}
				else if (pos[1] < mousePos[1] && mousePos[1] < pos[1] + height) {
					uint sel = static_cast<uint>((mousePos[1] - pos[1]) / dropDownHeight);

					if (sel <= selection) {
						sel--;
					}

					setSelection(sel);

					drop = false;

					markDirty();

					EventManager::getSingleton().cancelEvent();

					if (onChange) {
						onChange(*this);
					}
				}
				else {
					drop = false;

					markDirty();
				}
			}
			else {
				drop = false;

				markDirty();
			}
		}
	}
}