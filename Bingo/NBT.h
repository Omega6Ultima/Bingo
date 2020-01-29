//Dustin Gehm

#pragma once
#ifndef _NBT_H
#define _NBT_H

#include <map>
#include <ostream>
#include <string>
#include <sstream>

#include <SDL_atomic.h>
#include <SDL_rwops.h>

#include "Exception.h"
#include "Utils.h"

using std::endl;
using std::map;
using std::ostream;
using std::string;
using std::stringstream;

#define NBT_MAX_NAME 80

class NBT_Base{
public:
	NBT_Base(string nbtName){
		name = nbtName;
	}

	~NBT_Base(){
		//
	}

	enum NBT_Type{
		NBT_COMPOUND,
		NBT_COMPOUND_END,
		NBT_BOOL,
		NBT_CHAR,
		NBT_UCHAR,
		NBT_SHORT,
		NBT_USHORT,
		NBT_INT,
		NBT_UINT,
		NBT_LONG,
		NBT_ULONG,
		NBT_LLONG,
		NBT_ULLONG,
		NBT_FLOAT,
		NBT_DOUBLE,
		NBT_STRING
	};

	virtual NBT_Type getType() = 0;

	string getName() const{
		return name;
	}

	virtual void writeData(SDL_RWops* file) = 0;

	virtual string toString() = 0;

protected:
	NBT_Type type;
	string name = "";
	string builtStr = "";
	bool dirty = true;

private:
	//
};

class NBT_Compound : public NBT_Base{
public:
	NBT_Compound(string nbtName)
	: NBT_Base(nbtName){
		//
	}

	~NBT_Compound(){
		for (auto iter = tags.begin(); iter != tags.end(); iter++){
			delete iter->second;
		}
	}

	NBT_Type getType() override{
		return NBT_COMPOUND;
	}

	void writeData(SDL_RWops* file) override{
		NBT_Type compoundTag = NBT_COMPOUND;
		NBT_Type compundEndTag = NBT_COMPOUND_END;
		int nameSize = name.size();

		SDL_RWwrite(file, &compoundTag, sizeof(compoundTag), 1);
		SDL_RWwrite(file, &nameSize, SZ_INT, 1);
		SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);

		for (auto iter = tags.begin(); iter != tags.end(); iter++){
			iter->second->writeData(file);
		}

		SDL_RWwrite(file, &compundEndTag, sizeof(compundEndTag), 1);
	}

	void setTag(NBT_Base* nbt){
		tags[nbt->getName()] = nbt;
		dirty = true;
	}

	NBT_Base* getTag(string tagName){
		if (tags.find(tagName) != tags.end()){
			return tags[tagName];
		}

		return NULL;
	}

	NBT_Base* getTag(uint index){
		if (index < getTagCount()){
			auto iter = tags.begin();

			std::advance(iter, index);

			return iter->second;
		}

		return NULL;
	}

	inline uint getTagCount() const{
		return tags.size();
	}

	string toString() override{
		if (dirty){
			builtStr = toString(0);
			dirty = false;
		}

		return builtStr;
	}

	friend ostream& operator <<(ostream& os, NBT_Compound* nbt){
		return os << nbt->toString();
	}

private:
	string toString(uchar tabLevel){
		stringstream str;

		str << tabs(tabLevel) << "NBT_Compound(" << name << "){\n";

		tabLevel += 1;

		for (auto iter = tags.begin(); iter != tags.end(); iter++){
			if (iter->second->getType() == NBT_COMPOUND){
				str << ((NBT_Compound*)iter->second)->toString(tabLevel) << endl;
			}
			else{
				str << tabs(tabLevel) << iter->second->toString() << endl;
			}
		}

		tabLevel -= 1;

		str << tabs(tabLevel) << "}";

		return str.str();
	}

private:
	map<string, NBT_Base*> tags;
};

template<class T>
class NBT_Tag : public NBT_Base{
public:
	NBT_Tag(string nbtName, T dat)
	: NBT_Base(nbtName){
		type = getType();
		data = dat;
	}

	~NBT_Tag(){
		//
	}

	void writeData(SDL_RWops* file) override{
		int nameSize = name.size();

		SDL_RWwrite(file, &type, sizeof(type), 1);
		SDL_RWwrite(file, &nameSize, SZ_INT, 1);
		SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
		SDL_RWwrite(file, &data, sizeof(data), 1);
	}

	NBT_Type getType() override{
		throw Exception("Using NBT_Tag for a type not supported by NBT_Tag");
	}

	void setData(T dat){
		data = dat;
		dirty = true;
	}

	T getData() const{
		return data;
	}

	string toString() override{
		if (dirty){
			stringstream str;

			str << "NBT_Tag{" << name << ":" << data << "}";

			builtStr = str.str();
			dirty = false;
		}

		return builtStr;
	}

	friend ostream& operator <<(ostream& os, NBT_Tag<T>* nbt){
		return os << nbt->toString();
	}

private:
	T data;
};

template<>
NBT_Base::NBT_Type NBT_Tag<bool>::getType(){
	return NBT_Base::NBT_BOOL;
}

template<>
NBT_Base::NBT_Type NBT_Tag<char>::getType(){
	return NBT_Base::NBT_CHAR;
}

template<>
NBT_Base::NBT_Type NBT_Tag<uchar>::getType(){
	return NBT_Base::NBT_UCHAR;
}

template<>
NBT_Base::NBT_Type NBT_Tag<short>::getType(){
	return NBT_Base::NBT_SHORT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<ushort>::getType(){
	return NBT_Base::NBT_USHORT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<int>::getType(){
	return NBT_Base::NBT_INT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<uint>::getType(){
	return NBT_Base::NBT_UINT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<long>::getType(){
	return NBT_Base::NBT_LONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<ulong>::getType(){
	return NBT_Base::NBT_ULONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<llong>::getType(){
	return NBT_Base::NBT_LLONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<ullong>::getType(){
	return NBT_Base::NBT_ULLONG;
}

template<>
NBT_Base::NBT_Type NBT_Tag<float>::getType(){
	return NBT_Base::NBT_FLOAT;
}

template<>
NBT_Base::NBT_Type NBT_Tag<double>::getType(){
	return NBT_Base::NBT_DOUBLE;
}

template<>
NBT_Base::NBT_Type NBT_Tag<string>::getType(){
	return NBT_Base::NBT_STRING;
}

template<>
void NBT_Tag<string>::writeData(SDL_RWops* file){
	int nameSize = name.size();
	int tagSize = data.size();

	SDL_RWwrite(file, &type, sizeof(type), 1);
	SDL_RWwrite(file, &nameSize, SZ_INT, 1);
	SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
	SDL_RWwrite(file, &tagSize, SZ_INT, 1);
	SDL_RWwrite(file, data.data(), SZ_CHAR, tagSize);
}

#endif