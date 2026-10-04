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

bool NBT_Base::operator==(const NBT_Base& other) const {
#define CAST_N_COMPARE(TYPE) \
		auto & castedOther = static_cast<const TYPE&>(other); \
		\
		return static_cast<const TYPE&>(*this) == castedOther;

	switch (getType()) {
	case NBT_COMPOUND: {
		CAST_N_COMPARE(NBT_Compound)
	}
	case NBT_BOOL: {
		CAST_N_COMPARE(NBT_Tag<bool>)
	}
	case NBT_CHAR: {
		CAST_N_COMPARE(NBT_Tag<char>)
	}
	case NBT_UCHAR: {
		CAST_N_COMPARE(NBT_Tag<uchar>)
	}
	case NBT_SHORT: {
		CAST_N_COMPARE(NBT_Tag<short>)
	}
	case NBT_USHORT: {
		CAST_N_COMPARE(NBT_Tag<ushort>)
	}
	case NBT_INT: {
		CAST_N_COMPARE(NBT_Tag<int>)
	}
	case NBT_UINT: {
		CAST_N_COMPARE(NBT_Tag<uint>)
	}
	case NBT_LONG: {
		CAST_N_COMPARE(NBT_Tag<long>)
	}
	case NBT_ULONG: {
		CAST_N_COMPARE(NBT_Tag<ulong>)
	}
	case NBT_LLONG: {
		CAST_N_COMPARE(NBT_Tag<llong>)
	}
	case NBT_ULLONG: {
		CAST_N_COMPARE(NBT_Tag<ullong>)
	}
	case NBT_FLOAT: {
		CAST_N_COMPARE(NBT_Tag<float>)
	}
	case NBT_DOUBLE: {
		CAST_N_COMPARE(NBT_Tag<double>)
	}
	case NBT_STRING: {
		CAST_N_COMPARE(NBT_Tag<string>)
	}
	case NBT_POINTER: {
		CAST_N_COMPARE(NBT_Tag<void*>)
	}
	case NBT_INT_ARRAY: {
		CAST_N_COMPARE(NBT_Tag<vector<int>>)
	}
	case NBT_DOUBLE_ARRAY: {
		CAST_N_COMPARE(NBT_Tag<vector<double>>)
	}
	default:
		throw Bingo::Exception("Comparing undefined NBT_Type");
	}

	// this is here to prevent compile errors
#undef CAST_N_COMPARE
	return false;
}

NBT_Compound::NBT_Compound(string nbtName)
	: NBT_Base(nbtName) {
	type = NBT_Base::NBT_COMPOUND;
}

NBT_Compound::~NBT_Compound() {
	for (auto iter = tags.begin(); iter != tags.end(); iter++) {
		delete iter->second;
	}
}

NBT_Compound::NBT_Type NBT_Compound::getType() const {
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

bool NBT_Compound::operator==(const NBT_Compound& other) const {
	bool result = false;

	if (type == other.type) {
		if (name == other.name) {
			result = true;

			for (auto iter = tags.begin(); iter != tags.end(); iter++) {
				auto tagCount = other.tags.count(iter->first);

				if (tagCount > 0) {
					const NBT_Base& mine = *(iter->second);
					const NBT_Base& theirs = *(other.tags.at(iter->first));

					if (mine != theirs) {
						result = false;
						break;
					}
				}
				else {
					result = false;
				}
			}
		}
	}

	return result;
}

template<>
NBT_Base::NBT_Type NBT_Tag<bool>::getType() const {
	return NBT_Base::NBT_BOOL;
}

template<>
NBT_Base::NBT_Type NBT_Tag<char>::getType() const {
	return NBT_Base::NBT_CHAR;
}

template<>
NBT_Base::NBT_Type NBT_Tag<uchar>::getType() const {
	return NBT_Base::NBT_UCHAR;
}

template<>
NBT_Base::NBT_Type NBT_Tag<short>::getType() const {
	return NBT_Base::NBT_SHORT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<ushort>::getType() const {
	return NBT_Base::NBT_USHORT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<int>::getType() const {
	return NBT_Base::NBT_INT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<uint>::getType() const {
	return NBT_Base::NBT_UINT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<long>::getType() const {
	return NBT_Base::NBT_LONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<ulong>::getType() const {
	return NBT_Base::NBT_ULONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<llong>::getType() const {
	return NBT_Base::NBT_LLONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<ullong>::getType() const {
	return NBT_Base::NBT_ULLONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<float>::getType() const {
	return NBT_Base::NBT_FLOAT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<double>::getType() const {
	return NBT_Base::NBT_DOUBLE;
}

template<>
NBT_Base::NBT_Type NBT_Tag<string>::getType() const {
	return NBT_Base::NBT_STRING;
}

template<>
NBT_Base::NBT_Type NBT_Tag<void*>::getType() const {
	return NBT_Base::NBT_POINTER;
}

template<>
NBT_Base::NBT_Type NBT_Tag<std::vector<int>>::getType() const {
	return NBT_Base::NBT_INT_ARRAY;
}

template<>
NBT_Base::NBT_Type NBT_Tag<std::vector<double>>::getType() const {
	return NBT_Base::NBT_DOUBLE_ARRAY;
}

template<>
void NBT_Tag<string>::writeData(SDL_RWops* file) {
	int nameSize = name.size();
	int tagSize = data.size();

	SDL_RWwrite(file, &type, sizeof(type), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
	SDL_RWwrite(file, &tagSize, SZ_INT, 1);
	SDL_RWwrite(file, data.data(), SZ_CHAR, tagSize);
}

template<>
void NBT_Tag<std::vector<int>>::writeData(SDL_RWops* file) {
	int nameSize = name.size();
	int tagSize = data.size();

	SDL_RWwrite(file, &type, sizeof(type), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
	SDL_RWwrite(file, &tagSize, SZ_INT, 1);
	SDL_RWwrite(file, data.data(), SZ_INT, tagSize);
}

template<>
void NBT_Tag<std::vector<double>>::writeData(SDL_RWops* file) {
	int nameSize = name.size();
	int tagSize = data.size();

	SDL_RWwrite(file, &type, sizeof(type), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
	SDL_RWwrite(file, &tagSize, SZ_INT, 1);
	SDL_RWwrite(file, data.data(), SZ_DOUBLE, tagSize);
}

template<>
string NBT_Tag<void*>::toString() {
	if (dirty) {
		stringstream str;

		str << "NBT_Tag{" << name << ":0x" << hex << data << "}";

		builtStr = str.str();
		dirty = false;
	}

	return builtStr;
}