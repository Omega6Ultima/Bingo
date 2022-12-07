//Dustin Gehm
/*a class used to both listen for events and
dispatch calls to other classes that need to
run when certain events happen*/

#pragma once
#ifndef _EVENTMANAGER_H
#define _EVENTMANAGER_H

#include <map>
#include <set>
#include <vector>

#include <SDL_keycode.h>

#include "Singleton.h"
#include "Timer.h"
#include "VecN.h"

using std::map;
using std::set;
using std::vector;

#define KEY_COOLDOWN 50

namespace Bingo {

	namespace Events {

		class EventListener;

	}

	class EventManager :public Singleton<EventManager> {
	public:
		enum EventListenType {
			LISTEN_SYSTEM = 0,
			LISTEN_KEY,
			LISTEN_MOUSE
		};
		enum EventType {
			EVT_QUIT = 0,
			EVT_WINDOWEVENT,
			EVT_KEYUP,
			EVT_KEYDOWN,
			EVT_MOUSEMOVE,
			EVT_MOUSEBUTTONDOWN,
			EVT_MOUSEBUTTONUP
		};
		enum WindowEvent {
			WIN_SHOWN = 1,
			WIN_HIDDEN,
			WIN_EXPOSED,
			WIN_MOVED,
			WIN_RESIZED,
			WIN_SIZECHANGED,
			WIN_MINIMIZED,
			WIN_MAXIMIZED,
			WIN_RESTORED,
			WIN_ENTER,
			WIN_LEAVE,
			WIN_GAINFOCUS,
			WIN_LOSTFOCUS,
			WIN_CLOSE
		};
		enum KeyCode {
			K_RETURN = SDLK_RETURN,
			K_ESCAPE = SDLK_ESCAPE,
			K_BACKSPACE = SDLK_BACKSPACE,
			K_TAB = SDLK_TAB,
			K_SPACE = SDLK_SPACE,

			K_EXCLAIM = SDLK_EXCLAIM,
			K_DBLQUOTE = SDLK_QUOTEDBL,
			K_HASH = SDLK_HASH,
			K_PERCENT = SDLK_PERCENT,
			K_DOLLAR = SDLK_DOLLAR,
			K_AMPERSAND = SDLK_AMPERSAND,
			K_QUOTE = SDLK_QUOTE,
			K_LEFTPAREN = SDLK_LEFTPAREN,
			K_RIGHTPAREN = SDLK_RIGHTPAREN,
			K_ASTERISK = SDLK_ASTERISK,
			K_PLUS = SDLK_PLUS,
			K_COMMA = SDLK_COMMA,
			K_MINUS = SDLK_MINUS,
			K_PERIOD = SDLK_PERIOD,
			K_SLASH = SDLK_SLASH,

			K_0 = SDLK_0,
			K_1 = SDLK_1,
			K_2 = SDLK_2,
			K_3 = SDLK_3,
			K_4 = SDLK_4,
			K_5 = SDLK_5,
			K_6 = SDLK_6,
			K_7 = SDLK_7,
			K_8 = SDLK_8,
			K_9 = SDLK_9,

			K_COLON = SDLK_COLON,
			K_SEMICOLON = SDLK_SEMICOLON,
			K_LESS = SDLK_LESS,
			K_EQUALS = SDLK_EQUALS,
			K_GREATER = SDLK_GREATER,
			K_QUESTION = SDLK_QUESTION,
			K_AT = SDLK_AT,
			K_LEFTBRACKET = SDLK_LEFTBRACKET,
			K_BACKSLASH = SDLK_BACKSLASH,
			K_RIGHTBRACKET = SDLK_RIGHTBRACKET,
			K_CARET = SDLK_CARET,
			K_UNDERSCORE = SDLK_UNDERSCORE,
			K_GRAVE = SDLK_BACKQUOTE,

			K_a = SDLK_a,
			K_b = SDLK_b,
			K_c = SDLK_c,
			K_d = SDLK_d,
			K_e = SDLK_e,
			K_f = SDLK_f,
			K_g = SDLK_g,
			K_h = SDLK_h,
			K_i = SDLK_i,
			K_j = SDLK_j,
			K_k = SDLK_k,
			K_l = SDLK_l,
			K_m = SDLK_m,
			K_n = SDLK_n,
			K_o = SDLK_o,
			K_p = SDLK_p,
			K_q = SDLK_q,
			K_r = SDLK_r,
			K_s = SDLK_s,
			K_t = SDLK_t,
			K_u = SDLK_u,
			K_v = SDLK_v,
			K_w = SDLK_w,
			K_x = SDLK_x,
			K_y = SDLK_y,
			K_z = SDLK_z,

			K_CAPSLOCK = SDLK_CAPSLOCK,

			K_F1 = SDLK_F1,
			K_F2 = SDLK_F2,
			K_F3 = SDLK_F3,
			K_F4 = SDLK_F4,
			K_F5 = SDLK_F5,
			K_F6 = SDLK_F6,
			K_F7 = SDLK_F7,
			K_F8 = SDLK_F8,
			K_F9 = SDLK_F9,
			K_F10 = SDLK_F10,
			K_F11 = SDLK_F11,
			K_F12 = SDLK_F12,

			K_PRINTSCREEN = SDLK_PRINTSCREEN,
			K_SCROLLLOCK = SDLK_SCROLLLOCK,
			K_PAUSE = SDLK_PAUSE,
			K_INSERT = SDLK_INSERT,
			K_HOME = SDLK_HOME,
			K_PAGEUP = SDLK_PAGEUP,
			K_DELETE = SDLK_DELETE,
			K_END = SDLK_END,
			K_PAGEDOWN = SDLK_PAGEDOWN,
			K_RIGHT = SDLK_RIGHT,
			K_LEFT = SDLK_LEFT,
			K_DOWN = SDLK_DOWN,
			K_UP = SDLK_UP,

			K_NUMLOCKCLEAR = SDLK_NUMLOCKCLEAR,
			K_KP_DIVIDE = SDLK_KP_DIVIDE,
			K_KP_MULTIPLY = SDLK_KP_MULTIPLY,
			K_KP_MINUS = SDLK_KP_MINUS,
			K_KP_PLUS = SDLK_KP_PLUS,
			K_KP_ENTER = SDLK_KP_ENTER,
			K_KP_1 = SDLK_KP_1,
			K_KP_2 = SDLK_KP_2,
			K_KP_3 = SDLK_KP_3,
			K_KP_4 = SDLK_KP_4,
			K_KP_5 = SDLK_KP_5,
			K_KP_6 = SDLK_KP_6,
			K_KP_7 = SDLK_KP_7,
			K_KP_8 = SDLK_KP_8,
			K_KP_9 = SDLK_KP_9,
			K_KP_0 = SDLK_KP_0,
			K_KP_PERIOD = SDLK_KP_PERIOD,

			K_APPLICATION = SDLK_APPLICATION,
			K_POWER = SDLK_POWER,
			K_KP_EQUALS = SDLK_KP_EQUALS,
			K_F13 = SDLK_F13,
			K_F14 = SDLK_F14,
			K_F15 = SDLK_F15,
			K_F16 = SDLK_F16,
			K_F17 = SDLK_F17,
			K_F18 = SDLK_F18,
			K_F19 = SDLK_F19,
			K_F20 = SDLK_F20,
			K_F21 = SDLK_F21,
			K_F22 = SDLK_F22,
			K_F23 = SDLK_F23,
			K_F24 = SDLK_F24,
		};
		typedef SDL_Keymod KeyMod;
		enum MouseButton {
			MB_LEFT = 1,
			MB_CENTER,
			MB_RIGHT,
			MB_BACK,
			MB_FORWARD
		};

		EventManager();
		~EventManager();

		void update();
		/*add a listener whose handleEvent function
		will be called when an event of that type happens*/
		void registerListener(Events::EventListener* listener, EventListenType type, uint priority = 0);
		/*removes a listener from the category of that type*/
		void unregisterListener(Events::EventListener* listener, EventListenType type);
		void cancelEvent();
	private:
		map<uint, vector<Events::EventListener*>> keyListeners;
		map<uint, vector<Events::EventListener*>> mouseListeners;
		map<uint, vector<Events::EventListener*>> systemListeners;
		bool processEvent = true;
	};

	namespace Events {

		using Math::VecN;
		using Time::Timer;

		class EventListener {
		public:
			EventListener();
			~EventListener();
		protected:
			/*the function to overwrite in your own class to handle events*/
			virtual void handleEvent(EventManager::EventType evt) = 0;
			/*encapsulates all the possible extra info you need from events*/
			union EventData {
				int windowCode;
				struct {
					int keyCode;
					uchar keyRepeat;
					ushort mods;
				};
				int mouseDelta[2];
				int mouseButton;
			} eventData;
		private:
			friend class EventManager;
		};

		class QuitListener : virtual public EventListener {
		public:
			QuitListener();
			~QuitListener();

			inline bool getDone() const {
				return done;
			}

		protected:
			void handleEvent(EventManager::EventType e) override;

		private:
			bool done = false;
		};

		class KeyListener : virtual public EventListener {
		public:
			KeyListener();
			~KeyListener();

			bool checkKeyDown(EventManager::KeyCode key, uint cooldown = KEY_COOLDOWN);
			static char getKeySymbol(EventManager::KeyCode key, EventManager::KeyMod mod);

		protected:
			virtual void handleEvent(EventManager::EventType e) override;

		private:
			set<int> codes;
			map<int, uint> codeTimes;
			Timer timer;
		};

		class MouseListener : virtual public EventListener {
		public:
			MouseListener();
			~MouseListener();

			bool checkMouseButtonDown(EventManager::MouseButton button, uint cooldown = KEY_COOLDOWN);
			VecN<int, 2> getMousePos();

		protected:
			virtual void handleEvent(EventManager::EventType e) override;

		private:
			set<int> codes;
			map<int, uint> codeTimes;
			Timer timer;
		};

	}

}
#endif