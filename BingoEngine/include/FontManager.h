//Dustin Gehm
//A manager to handle and contain all used fonts

#pragma once
#ifndef _FONTMANAGER_H
#define _FONTMANAGER_H

#include <map>
#include <string>

#include <SDL_ttf.h>

#include "Core.h"
#include "Singleton.h"

using std::map;
using std::pair;
using std::string;

namespace Bingo {

	using Core::Manager;

	class FontManager : public Singleton<FontManager>, public Manager {
	public:
		FontManager(string path);
		~FontManager();

		void addFont(string fontName, int size);
		bool hasFont(string fontName, int size);
		TTF_Font* getFont(string fontName, int size);
		void nickname(string fontName, string nick);

	private:
		string fontPath = "";
		map<pair<string, int>, TTF_Font* > fonts;
		map<string, string> nicknames;
	};

}

#endif