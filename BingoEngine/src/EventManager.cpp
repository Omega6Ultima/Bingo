//Dustin Gehm

#include "EventManager.h"

#include <SDL_events.h>

using Bingo::EventManager;
using Bingo::Events::EventListener;
using Bingo::Events::KeyListener;
using Bingo::Events::MouseListener;
using Bingo::Events::QuitListener;
using Bingo::Math::VecN;
using Bingo::Time::Timer;
using Bingo::Utils::Error;

EventManager::EventManager() {
	//
}

EventManager::~EventManager() {
	//
}

void EventManager::update() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		processEvent = true;

		switch (event.type) {
		case SDL_QUIT:
			//for (uint c = 0; c < systemListeners.size(); c++){
			//	memset(&systemListeners[c]->eventData, 0, sizeof(EventListener::EventData));
			//	systemListeners[c]->eventData.windowCode = event.window.event;
			//	systemListeners[c]->handleEvent(EVT_QUIT);
			//}

			for (auto riter = systemListeners.rbegin(); riter != systemListeners.rend(); riter++) {
				if (riter->second.size() == 0) {
					continue;
				}

				for (auto iter = riter->second.begin(); iter != riter->second.end(); iter++) {
					memset(&(*iter)->eventData, 0, sizeof(EventListener::EventData));
					(*iter)->eventData.windowCode = event.window.event;
					(*iter)->handleEvent(EVT_QUIT);

					if (!processEvent) {
						break;
					}
				}

				if (!processEvent) {
					break;
				}
			}

			break;
		case SDL_WINDOWEVENT:
			//for (uint c = 0; c < systemListeners.size(); c++){
			//	memset(&systemListeners[c]->eventData, 0, sizeof(EventListener::EventData));
			//	systemListeners[c]->eventData.windowCode = event.window.event;
			//	systemListeners[c]->handleEvent(EVT_WINDOWEVENT);
			//}

			for (auto riter = systemListeners.rbegin(); riter != systemListeners.rend(); riter++) {
				if (riter->second.size() == 0) {
					continue;
				}

				for (auto iter = riter->second.begin(); iter != riter->second.end(); iter++) {
					memset(&(*iter)->eventData, 0, sizeof(EventListener::EventData));
					(*iter)->eventData.windowCode = event.window.event;
					(*iter)->handleEvent(EVT_WINDOWEVENT);

					if (!processEvent) {
						break;
					}
				}

				if (!processEvent) {
					break;
				}
			}

			break;
		case SDL_KEYDOWN:
			//for (uint c = 0; c < keyListeners.size(); c++){
			//	memset(&keyListeners[c]->eventData, 0, sizeof(EventListener::EventData));
			//	keyListeners[c]->eventData.keyCode = event.key.keysym.sym;
			//	keyListeners[c]->eventData.keyRepeat = event.key.repeat;
			//	keyListeners[c]->handleEvent(EVT_KEYDOWN);
			//}

			for (auto riter = keyListeners.rbegin(); riter != keyListeners.rend(); riter++) {
				if (riter->second.size() == 0) {
					continue;
				}

				for (auto iter = riter->second.begin(); iter != riter->second.end(); iter++) {
					memset(&(*iter)->eventData, 0, sizeof(EventListener::EventData));
					(*iter)->eventData.keyCode = event.key.keysym.sym;
					(*iter)->eventData.mods = event.key.keysym.mod;
					(*iter)->eventData.keyRepeat = event.key.repeat;
					(*iter)->handleEvent(EVT_KEYDOWN);

					if (!processEvent) {
						break;
					}
				}

				if (!processEvent) {
					break;
				}
			}

			break;
		case SDL_KEYUP:
			//for (uint c = 0; c < keyListeners.size(); c++){
			//	memset(&keyListeners[c]->eventData, 0, sizeof(EventListener::EventData));
			//	keyListeners[c]->eventData.keyCode = event.key.keysym.sym;
			//	keyListeners[c]->eventData.keyRepeat = event.key.repeat;
			//	keyListeners[c]->handleEvent(EVT_KEYUP);
			//}

			for (auto riter = keyListeners.rbegin(); riter != keyListeners.rend(); riter++) {
				if (riter->second.size() == 0) {
					continue;
				}

				for (auto iter = riter->second.begin(); iter != riter->second.end(); iter++) {
					memset(&(*iter)->eventData, 0, sizeof(EventListener::EventData));
					(*iter)->eventData.keyCode = event.key.keysym.sym;
					(*iter)->eventData.mods = event.key.keysym.mod;
					(*iter)->eventData.keyRepeat = event.key.repeat;
					(*iter)->handleEvent(EVT_KEYUP);

					if (!processEvent) {
						break;
					}
				}

				if (!processEvent) {
					break;
				}
			}

			break;
		case SDL_MOUSEMOTION:
			//for (uint c = 0; c < mouseListeners.size(); c++){
			//	memset(&mouseListeners[c]->eventData, 0, sizeof(EventListener::EventData));
			//	mouseListeners[c]->eventData.mouseDelta[0] = event.motion.xrel;
			//	mouseListeners[c]->eventData.mouseDelta[1] = event.motion.yrel;
			//	mouseListeners[c]->handleEvent(EVT_MOUSEMOVE);
			//}

			for (auto riter = mouseListeners.rbegin(); riter != mouseListeners.rend(); riter++) {
				if (riter->second.size() == 0) {
					continue;
				}

				for (auto iter = riter->second.begin(); iter != riter->second.end(); iter++) {
					memset(&(*iter)->eventData, 0, sizeof(EventListener::EventData));
					(*iter)->eventData.mouseDelta[0] = event.motion.xrel;
					(*iter)->eventData.mouseDelta[1] = event.motion.yrel;
					(*iter)->handleEvent(EVT_MOUSEMOVE);

					if (!processEvent) {
						break;
					}
				}

				if (!processEvent) {
					break;
				}
			}

			break;
		case SDL_MOUSEBUTTONDOWN:
			//for (uint c = 0; c < mouseListeners.size(); c++){
			//	memset(&mouseListeners[c]->eventData, 0, sizeof(EventListener::EventData));
			//	mouseListeners[c]->eventData.mouseButton = event.button.button;
			//	mouseListeners[c]->handleEvent(EVT_MOUSEBUTTONDOWN);
			//}

			for (auto riter = mouseListeners.rbegin(); riter != mouseListeners.rend(); riter++) {
				//cerr << riter->second.size() << std::endl;
				if (riter->second.size() == 0) {
					//cerr << "size 0\n";
					continue;
				}

				for (auto iter = riter->second.begin(); iter != riter->second.end(); iter++) {
					memset(&(*iter)->eventData, 0, sizeof(EventListener::EventData));
					(*iter)->eventData.mouseButton = event.button.button;
					(*iter)->handleEvent(EVT_MOUSEBUTTONDOWN);

					if (!processEvent) {
						break;
					}
				}

				if (!processEvent) {
					break;
				}
			}

			break;
		case SDL_MOUSEBUTTONUP:
			//for (uint c = 0; c < mouseListeners.size(); c++){
			//	memset(&mouseListeners[c]->eventData, 0, sizeof(EventListener::EventData));
			//	mouseListeners[c]->eventData.mouseButton = event.button.button;
			//	mouseListeners[c]->handleEvent(EVT_MOUSEBUTTONUP);
			//}

			for (auto riter = mouseListeners.rbegin(); riter != mouseListeners.rend(); riter++) {
				if (riter->second.size() == 0) {
					continue;
				}

				for (auto iter = riter->second.begin(); iter != riter->second.end(); iter++) {
					memset(&(*iter)->eventData, 0, sizeof(EventListener::EventData));
					(*iter)->eventData.mouseButton = event.button.button;
					(*iter)->handleEvent(EVT_MOUSEBUTTONUP);

					if (!processEvent) {
						break;
					}
				}

				if (!processEvent) {
					break;
				}
			}

			break;
		}
	}
}

void EventManager::registerListener(EventListener* listener, EventListenType type, uint priority) {
	switch (type) {
	case EventManager::LISTEN_SYSTEM:
		//systemListeners.push_back(listener);
		systemListeners[priority].push_back(listener);
		break;
	case EventManager::LISTEN_KEY:
		//keyListeners.push_back(listener);
		keyListeners[priority].push_back(listener);
		break;
	case EventManager::LISTEN_MOUSE:
		//mouseListeners.push_back(listener);
		mouseListeners[priority].push_back(listener);
		break;
	default:
		Error("Invalid EventListenType: " + type);
	}

	//cerr << "Registering \t" << listener << ": " << type << std::endl;
}

void EventManager::unregisterListener(EventListener* listener, EventListenType type) {
	//cerr << "Unregistering \t" << listener << ": " << type << std::endl;

	switch (type) {
	case EventManager::LISTEN_SYSTEM:
		for (auto iter = systemListeners.begin(); iter != systemListeners.end(); iter++) {
			//if (*iter == listener){
			//	systemListeners.erase(iter);
			//	break;
			//}
			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++) {
				if (*iter2 == listener) {
					iter2 = iter->second.erase(iter2);

					if (iter2 == iter->second.end()) {
						break;
					}
				}
			}

			if (iter->second.empty()) {
				iter = systemListeners.erase(iter);

				if (iter == systemListeners.end()) {
					break;
				}
			}
		}
		break;
	case EventManager::LISTEN_KEY:
		for (auto iter = keyListeners.begin(); iter != keyListeners.end(); iter++) {
			//if (*iter == listener){
			//	keyListeners.erase(iter);
			//	break;
			//}

			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++) {
				if (*iter2 == listener) {
					iter2 = iter->second.erase(iter2);

					if (iter2 == iter->second.end()) {
						break;
					}
				}
			}

			if (iter->second.empty()) {
				iter = keyListeners.erase(iter);

				if (iter == keyListeners.end()) {
					break;
				}
			}
		}
		break;
	case EventManager::LISTEN_MOUSE:
		for (auto iter = mouseListeners.begin(); iter != mouseListeners.end(); iter++) {
			//if (*iter == listener){
			//	mouseListeners.erase(iter);
			//	break;
			//}

			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); iter2++) {
				if (*iter2 == listener) {
					iter2 = iter->second.erase(iter2);

					if (iter2 == iter->second.end()) {
						break;
					}
				}
			}

			if (iter->second.empty()) {
				iter = mouseListeners.erase(iter);

				if (iter == mouseListeners.end()) {
					break;
				}
			}
		}
		break;
	default:
		Error("Invalid EventListType: " + type);
	}
}

void EventManager::cancelEvent() {
	processEvent = false;
}

EventListener::EventListener() {
	memset(&eventData, 0, sizeof(EventListener::EventData));
}

EventListener::~EventListener() {
	//
}

QuitListener::QuitListener() {
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_SYSTEM);
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_KEY);
}

QuitListener::~QuitListener() {
	//
}

void QuitListener::handleEvent(EventManager::EventType e) {
	if (e == EventManager::EVT_QUIT) {
		done = true;
	}
	else if (e == EventManager::EVT_WINDOWEVENT) {
		if (eventData.windowCode == EventManager::WIN_CLOSE) {
			done = true;
		}
	}
	else if (e == EventManager::EVT_KEYDOWN) {
		if (eventData.keyCode == EventManager::K_ESCAPE) {
			done = true;
		}
	}
}

KeyListener::KeyListener() {
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_KEY);
}

KeyListener::~KeyListener() {
	//
}

bool KeyListener::checkKeyDown(EventManager::KeyCode key, uint cooldown) {
	if (codes.find(key) != codes.end()) {
		uint curTicks = timer.getTicks();

		if (codeTimes.find(key) == codeTimes.end() ||
			curTicks - codeTimes[key] >= cooldown) {
			codeTimes[key] = curTicks;

			return true;
		}
	}

	return  false;
}

char KeyListener::getKeySymbol(EventManager::KeyCode key, EventManager::KeyMod mod) {
	switch (key) {
	case EventManager::K_ESCAPE:
		return '\033';
	case EventManager::K_GRAVE:
		return mod & KMOD_SHIFT ? '~' : '`';
	case EventManager::K_EXCLAIM:
	case EventManager::K_1:
		return mod & KMOD_SHIFT ? '!' : '1';
	case EventManager::K_AT:
	case EventManager::K_2:
		return mod & KMOD_SHIFT ? '@' : '2';
	case EventManager::K_HASH:
	case EventManager::K_3:
		return mod & KMOD_SHIFT ? '#' : '3';
	case EventManager::K_DOLLAR:
	case EventManager::K_4:
		return mod & KMOD_SHIFT ? '$' : '4';
	case EventManager::K_PERCENT:
	case EventManager::K_5:
		return mod & KMOD_SHIFT ? '%' : '5';
	case EventManager::K_CARET:
	case EventManager::K_6:
		return mod & KMOD_SHIFT ? '^' : '6';
	case EventManager::K_AMPERSAND:
	case EventManager::K_7:
		return mod & KMOD_SHIFT ? '&' : '7';
	case EventManager::K_ASTERISK:
	case EventManager::K_8:
		return mod & KMOD_SHIFT ? '*' : '8';
	case EventManager::K_LEFTPAREN:
	case EventManager::K_9:
		return mod & KMOD_SHIFT ? '(' : '9';
	case EventManager::K_RIGHTPAREN:
	case EventManager::K_0:
		return mod & KMOD_SHIFT ? ')' : '0';
	case EventManager::K_UNDERSCORE:
	case EventManager::K_MINUS:
		return mod & KMOD_SHIFT ? '_' : '-';
	case EventManager::K_EQUALS:
	case EventManager::K_PLUS:
		return mod & KMOD_SHIFT ? '+' : '=';
	case EventManager::K_BACKSPACE:
		return '\b';
	case EventManager::K_TAB:
		return '\t';
	case EventManager::K_a:
	case EventManager::K_b:
	case EventManager::K_c:
	case EventManager::K_d:
	case EventManager::K_e:
	case EventManager::K_f:
	case EventManager::K_g:
	case EventManager::K_h:
	case EventManager::K_i:
	case EventManager::K_j:
	case EventManager::K_k:
	case EventManager::K_l:
	case EventManager::K_m:
	case EventManager::K_n:
	case EventManager::K_o:
	case EventManager::K_p:
	case EventManager::K_q:
	case EventManager::K_r:
	case EventManager::K_s:
	case EventManager::K_t:
	case EventManager::K_u:
	case EventManager::K_v:
	case EventManager::K_w:
	case EventManager::K_x:
	case EventManager::K_y:
	case EventManager::K_z:
		return mod & KMOD_SHIFT ? toupper(key) : key;
	case EventManager::K_LEFTBRACKET:
		return mod & KMOD_SHIFT ? '{' : '[';
	case EventManager::K_RIGHTBRACKET:
		return mod & KMOD_SHIFT ? '}' : ']';
	case EventManager::K_BACKSLASH:
		return mod & KMOD_SHIFT ? '|' : '\\';
	case EventManager::K_COLON:
	case EventManager::K_SEMICOLON:
		return mod & KMOD_SHIFT ? ':' : ';';
	case EventManager::K_DBLQUOTE:
	case EventManager::K_QUOTE:
		return mod & KMOD_SHIFT ? '"' : '\'';
	case EventManager::K_RETURN:
		return '\n';
	case EventManager::K_GREATER:
	case EventManager::K_COMMA:
		return mod & KMOD_SHIFT ? '<' : ',';
	case EventManager::K_LESS:
	case EventManager::K_PERIOD:
		return mod & KMOD_SHIFT ? '>' : '.';
	case EventManager::K_QUESTION:
	case EventManager::K_SLASH:
		return mod & KMOD_SHIFT ? '?' : '/';
	case EventManager::K_SPACE:
		return ' ';
	case EventManager::K_KP_0:
		return '0';
	case EventManager::K_KP_1:
		return '1';
	case EventManager::K_KP_2:
		return '2';
	case EventManager::K_KP_3:
		return '3';
	case EventManager::K_KP_4:
		return '4';
	case EventManager::K_KP_5:
		return '5';
	case EventManager::K_KP_6:
		return '6';
	case EventManager::K_KP_7:
		return '7';
	case EventManager::K_KP_8:
		return '8';
	case EventManager::K_KP_9:
		return '9';
	case EventManager::K_KP_PERIOD:
		return '.';
	case EventManager::K_KP_DIVIDE:
		return '/';
	case EventManager::K_KP_MULTIPLY:
		return '*';
	case EventManager::K_KP_MINUS:
		return '-';
	case EventManager::K_KP_PLUS:
		return '+';
	case EventManager::K_KP_ENTER:
		return '\n';
	}

	return 0;
}

void KeyListener::handleEvent(EventManager::EventType e) {
	if (e == EventManager::EVT_KEYDOWN) {
		if (codes.find(eventData.keyCode) == codes.end()) {
			codes.insert(eventData.keyCode);
		}
	}
	else if (e == EventManager::EVT_KEYUP) {
		codes.erase(eventData.keyCode);
		codeTimes.erase(eventData.keyCode);
	}
}

MouseListener::MouseListener() {
	EventManager::getSingleton().registerListener(this, EventManager::LISTEN_MOUSE);
}

MouseListener::~MouseListener() {
	//
}

bool MouseListener::checkMouseButtonDown(EventManager::MouseButton button, uint cooldown) {
	if (codes.find(button) != codes.end()) {
		uint curTicks = timer.getTicks();

		if (codeTimes.find(button) == codeTimes.end() ||
			curTicks - codeTimes[button] >= cooldown) {
			codeTimes[button] = curTicks;

			return true;
		}
	}

	return false;
}

VecN<int, 2> MouseListener::getMousePos() {
	VecN<int, 2> mousePos;

	SDL_GetMouseState(&mousePos[0], &mousePos[1]);

	return mousePos;
}

void MouseListener::handleEvent(EventManager::EventType e) {
	if (e == EventManager::EVT_MOUSEBUTTONDOWN) {
		if (codes.find(eventData.mouseButton) == codes.end()) {
			codes.insert(eventData.mouseButton);
		}
	}
	else if (e == EventManager::EVT_MOUSEBUTTONUP) {
		codes.erase(eventData.mouseButton);
		codeTimes.erase(eventData.mouseButton);
	}
}