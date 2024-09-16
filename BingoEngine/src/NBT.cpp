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
	switch (getType()) {
	case NBT_COMPOUND: {
		auto* castedOther = static_cast<const NBT_Compound*>(&other);

		return static_cast<const NBT_Compound*>(this)->operator==(*castedOther);
	}
	case NBT_BOOL: {
		auto* castedOther = static_cast<const NBT_Tag<bool>*>(&other);

		return static_cast<const NBT_Tag<bool>*>(this)->operator==(*castedOther);
	}
	case NBT_CHAR: {
		auto* castedOther = static_cast<const NBT_Tag<char>*>(&other);

		return static_cast<const NBT_Tag<char>*>(this)->operator==(*castedOther);
	}
	case NBT_UCHAR: {
		auto* castedOther = static_cast<const NBT_Tag<uchar>*>(&other);

		return static_cast<const NBT_Tag<uchar>*>(this)->operator==(*castedOther);
	}
	case NBT_SHORT: {
		auto* castedOther = static_cast<const NBT_Tag<short>*>(&other);

		return static_cast<const NBT_Tag<short>*>(this)->operator==(*castedOther);
	}
	case NBT_USHORT: {
		auto* castedOther = static_cast<const NBT_Tag<ushort>*>(&other);

		return static_cast<const NBT_Tag<ushort>*>(this)->operator==(*castedOther);
	}
	case NBT_INT: {
		auto* castedOther = static_cast<const NBT_Tag<int>*>(&other);

		return static_cast<const NBT_Tag<int>*>(this)->operator==(*castedOther);
	}
	case NBT_UINT: {
		auto* castedOther = static_cast<const NBT_Tag<uint>*>(&other);

		return static_cast<const NBT_Tag<uint>*>(this)->operator==(*castedOther);
	}
	case NBT_LONG: {
		auto* castedOther = static_cast<const NBT_Tag<long>*>(&other);

		return static_cast<const NBT_Tag<long>*>(this)->operator==(*castedOther);
	}
	case NBT_ULONG: {
		auto* castedOther = static_cast<const NBT_Tag<ulong>*>(&other);

		return static_cast<const NBT_Tag<ulong>*>(this)->operator==(*castedOther);
	}
	case NBT_LLONG: {
		auto* castedOther = static_cast<const NBT_Tag<llong>*>(&other);

		return static_cast<const NBT_Tag<llong>*>(this)->operator==(*castedOther);
	}
	case NBT_ULLONG: {
		auto* castedOther = static_cast<const NBT_Tag<ullong>*>(&other);

		return static_cast<const NBT_Tag<ullong>*>(this)->operator==(*castedOther);
	}
	case NBT_FLOAT: {
		auto* castedOther = static_cast<const NBT_Tag<float>*>(&other);

		return static_cast<const NBT_Tag<float>*>(this)->operator==(*castedOther);
	}
	case NBT_DOUBLE: {
		auto* castedOther = static_cast<const NBT_Tag<double>*>(&other);

		return static_cast<const NBT_Tag<double>*>(this)->operator==(*castedOther);
	}
	case NBT_STRING: {
		auto* castedOther = static_cast<const NBT_Tag<std::string>*>(&other);

		return static_cast<const NBT_Tag<std::string>*>(this)->operator==(*castedOther);
	}
	case NBT_POINTER: {
		auto* castedOther = static_cast<const NBT_Tag<void*>*>(&other);

		return static_cast<const NBT_Tag<void*>*>(this)->operator==(*castedOther);
	}
	// TODO what to cast these as?
	//case NBT_INT_ARRAY: {
	//	auto* castedOther = static_cast<const NBT_Tag<bool>*>(&other);
	//
	//	return static_cast<NBT_Tag<bool>*>(this)->operator==(*castedOther);
	//}
	//case NBT_DOUBLE_ARRAY: {
	//	auto* castedOther = static_cast<const NBT_Tag<bool>*>(&other);
	//
	//	return static_cast<NBT_Tag<bool>*>(this)->operator==(*castedOther);
	//}
	default:
		throw Bingo::Exception("Comparing undefined NBT_Type");
	}

	// this is here to prevent compile errors
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
					if (!(*(iter->second) == *(other.tags.at(iter->first)))) {
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
NBT_Base::NBT_Type NBT_Tag<Bingo::Math::DynVecN<int>>::getType() const {
	return NBT_Base::NBT_INT_ARRAY;
}

template<>
NBT_Base::NBT_Type NBT_Tag<std::vector<int>>::getType() const {
	return NBT_Base::NBT_INT_ARRAY;
}

template<>
NBT_Base::NBT_Type NBT_Tag<Bingo::Math::DynVecN<double>>::getType() const {
	return NBT_Base::NBT_DOUBLE_ARRAY;
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
void NBT_Tag<Bingo::Math::DynVecN<int>>::writeData(SDL_RWops* file) {
	int nameSize = name.size();
	int tagSize = data.getSize();

	SDL_RWwrite(file, &type, sizeof(type), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
	SDL_RWwrite(file, &tagSize, SZ_INT, 1);
	SDL_RWwrite(file, data.data(), SZ_INT, tagSize);
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
void NBT_Tag<Bingo::Math::DynVecN<double>>::writeData(SDL_RWops* file) {
	int nameSize = name.size();
	int tagSize = data.getSize();

	SDL_RWwrite(file, &type, sizeof(type), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
	SDL_RWwrite(file, &tagSize, SZ_INT, 1);
	SDL_RWwrite(file, data.data(), SZ_DOUBLE, tagSize);
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