//Dustin Gehm

#include "NBT.h"

using Bingo::NBT_Base;
using Bingo::NBT_Compound;
using Bingo::NBT_Tag;
using Bingo::Utils::tabs;

const string NBT_Base::NBT_TypeStr[] = {
	"NBT_COMPOUND",
	"NBT_COMPOUND_END",
	"NBT_BOOL",
	"NBT_CHAR",
	"NBT_UCHAR",
	"NBT_SHORT",
	"NBT_USHORT",
	"NBT_INT",
	"NBT_UINT",
	"NBT_LONG",
	"NBT_ULONG",
	"NBT_LLONG",
	"NBT_ULLONG",
	"NBT_FLOAT",
	"NBT_DOUBLE",
	"NBT_STRING",
	"NBT_POINTER",
	"NBT_INT_ARRAY",
	"NBT_DOUBLE_ARRAY",
};

NBT_Base::NBT_Base(string nbtName) {
	name = nbtName;
}

NBT_Base::~NBT_Base() {
	//
}

string NBT_Base::getName() const {
	return name;
}

NBT_Compound::NBT_Compound(string nbtName)
	: NBT_Base(nbtName) {
	//
}

NBT_Compound::~NBT_Compound() {
	for (auto iter = tags.begin(); iter != tags.end(); iter++) {
		delete iter->second;
	}
}

NBT_Compound::NBT_Type NBT_Compound::getType() {
	return NBT_COMPOUND;
}

void NBT_Compound::writeData(SDL_RWops* file) {
	NBT_Type compoundTag = NBT_COMPOUND;
	NBT_Type compundEndTag = NBT_COMPOUND_END;
	int nameSize = name.size();

	SDL_RWwrite(file, &compoundTag, sizeof(compoundTag), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);

	for (auto iter = tags.begin(); iter != tags.end(); iter++) {
		iter->second->writeData(file);
	}

	SDL_RWwrite(file, &compundEndTag, sizeof(compundEndTag), 1);
}

void NBT_Compound::setTag(NBT_Base* nbt) {
	tags[nbt->getName()] = nbt;
	dirty = true;
}

NBT_Base* NBT_Compound::getTag(string tagName) {
	if (tags.find(tagName) != tags.end()) {
		return tags[tagName];
	}

	return NULL;
}

NBT_Base* NBT_Compound::getTag(uint index) {
	if (index < getTagCount()) {
		auto iter = tags.begin();

		std::advance(iter, index);

		return iter->second;
	}

	return NULL;
}

string NBT_Compound::toString() {
	if (dirty) {
		builtStr = toString(0);
		dirty = false;
	}

	return builtStr;
}

string NBT_Compound::toString(uchar tabLevel) {
	stringstream str;

	str << tabs(tabLevel) << "NBT_Compound(" << name << "){\n";

	tabLevel += 1;

	for (auto iter = tags.begin(); iter != tags.end(); iter++) {
		if (iter->second->getType() == NBT_COMPOUND) {
			str << ((NBT_Compound*)iter->second)->toString(tabLevel) << endl;
		}
		else {
			str << tabs(tabLevel) << iter->second->toString() << endl;
		}
	}

	tabLevel -= 1;

	str << tabs(tabLevel) << "}";

	return str.str();
}