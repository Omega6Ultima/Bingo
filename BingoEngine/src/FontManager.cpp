//Dustin Gehm

#include "FontManager.h"

#include "ThreadManager.h"

using std::endl;

using Bingo::FontManager;
using Bingo::Utils::Error;

FontManager::FontManager(string path) {
	fontPath = path;
}

FontManager::~FontManager() {
	for (auto iter = fonts.begin(); iter != fonts.end(); iter++) {
		//It appears the TTF_Font pointers are auto managed
		//TTF_CloseFont(iter->second);
	}
}

void FontManager::addFont(string fontName, int size) {
	if (nicknames.find(fontName) != nicknames.end()) {
		fontName = nicknames[fontName];
	}

	ATOMIC_LOCK(ThreadManager::fileLock);

	TTF_Font* tempFont = TTF_OpenFont((fontPath + fontName).c_str(), size);

	ATOMIC_UNLOCK(ThreadManager::fileLock);

	if (tempFont == NULL) {
		Error("Could not load ", fontName);
	}

	fonts[make_pair(fontName, size)] = tempFont;
}

bool FontManager::hasFont(string fontName, int size) {
	if (nicknames.find(fontName) != nicknames.end()) {
		fontName = nicknames[fontName];
	}

	return fonts.find(make_pair(fontName, size)) != fonts.end();
}

TTF_Font* FontManager::getFont(string fontName, int size) {
	if (nicknames.find(fontName) != nicknames.end()) {
		fontName = nicknames[fontName];
	}

	if (hasFont(fontName, size)) {
		return fonts[make_pair(fontName, size)];
	}

	return NULL;
}

void FontManager::nickname(string fontName, string nick) {
	if (nicknames.find(nick) == nicknames.end()) {
		nicknames[nick] = fontName;
	}
}