//Dustin Gehm

#include "GUI.h"
#include "WindowManager.h"

#include <SDL_events.h>

Button::Button(int x, int y, ButtonFunc func, string fontName, int size, string text, Color color)
	: TextSurface(fontName, size, text, color) {
	pos = { static_cast<float>(x), static_cast<float>(y) };
	frameColor = color;
	onButtonClick = func;

	renderTexture();

	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_MOUSE);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_MOUSE, BUTTON_PRIORITY);
}

Button::Button(VecN<int, 2> position, ButtonFunc func, string fontName, int size, string text, Color color)
	: TextSurface(fontName, size, text, color) {
	pos = position;
	frameColor = color;
	onButtonClick = func;

	renderTexture();

	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_MOUSE);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_MOUSE, BUTTON_PRIORITY);
}

void Button::setFrameColor(Color color) {
	frameColor = color;

	markDirty();
}

void Button::setFrameWidth(uint width) {
	frameWidth = width;
	setTextPadding(static_cast<uint>(width * 1.25) + 3);

	markDirty();
}

//void Button::setShadowText(string str){
//	shadowText = str;
//
//	markDirty();
//}

void Button::renderTexture() {
	TextSurface::renderTexture();

	Surface::saveRenderTarget();

	setRenderTarget();
	setDrawColor(frameColor);

	for (uint c = 0; c < frameWidth; c++) {
		drawRect(c, c, width - 2 * c, height - 2 * c, false);
	}

	Surface::restoreRenderTarget();
}

void Button::handleEvent(EventManager::EventType evt) {
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
	: Button(x, y, NULL, fontName, fontSize, startText, color) {
	inputWidth = width;
	inputHeight = height;
	onInputClick = func;
	cursorTime = 0;
	cursorWidth = getLetterWidth('I');

	//renderTexture();

	//unregister the registration from Button constructor
	EventManager::getSingleton().unregisterListener(this, EventManager::LISTEN_KEY);

	//register with higher priority
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_KEY, INPUT_PRIORITY);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_SYSTEM, INPUT_PRIORITY);
}

Input::Input(VecN<int, 2> position, int width, int height, InputFunc func, string fontName, int fontSize, string startText, Color color)
	: Button(position, NULL, fontName, fontSize, startText, color) {
	inputWidth = width;
	inputHeight = height;
	onInputClick = func;
	cursorTime = 0;
	cursorWidth = getLetterWidth('I');

	renderTexture();

	//unregister the registration from Button constructor
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
	//create surface from background of renderer
	//SDL_RenderReadPixels
	//SDL_LockTexture
	//SDL_TEXTUREACCESSSTREAMING

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

DropDown::DropDown(int x, int y, uint w, uint h, vector<string>* values, DropDownFunc func, string fontName, int size, Color color)
	: Button(x, y, NULL, fontName, size, "", color) {
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

void DropDown::setValue(uint index) {
#ifdef _DEBUG
	if (index >= options.size()) {
		throw Exception("Index out of range");
	}
#endif
	selection = index;

	markDirty();
}

void DropDown::renderTexture() {
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

					setValue(sel);

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