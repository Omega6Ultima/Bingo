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
#include "VecN.h"

using std::endl;
using std::hex;
using std::map;
using std::ostream;
using std::string;
using std::stringstream;

#define NBT_MAX_NAME 80

namespace Bingo {

	//Base class for NBT
	class NBT_Base {
	public:
		enum NBT_Type {
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
			NBT_STRING,
			NBT_POINTER,
			NBT_INT_ARRAY,
			NBT_DOUBLE_ARRAY,
		};

		static const string NBT_TypeStr[];

		NBT_Base(string nbtName);
		~NBT_Base();

		virtual NBT_Type getType() = 0;
		string getName() const;
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

	class NBT_Compound : public NBT_Base {
	public:
		NBT_Compound(string nbtName);
		~NBT_Compound();

		NBT_Type getType() override;
		void writeData(SDL_RWops* file) override;

		//add a tag to this compound
		//this must be a pointer to a dynamically allocated NBT_Tag or NBT_Compound
		void setTag(NBT_Base* nbt);
		NBT_Base* getTag(string tagName);
		NBT_Base* getTag(uint index);

		inline uint getTagCount() const {
			return tags.size();
		}

		string toString() override;

	private:
		string toString(uchar tabLevel);

	private:
		map<string, NBT_Base*> tags;

		friend inline ostream& operator <<(ostream& os, NBT_Compound* nbt) {
			return os << nbt->toString();
		}
	};

	template<class T>
	class NBT_Tag : public NBT_Base {
	public:
		NBT_Tag(string nbtName, T dat)
			: NBT_Base(nbtName) {
			type = getType();
			data = dat;
		}

		~NBT_Tag() {
			//
		}

		void writeData(SDL_RWops* file) override {
			int nameSize = name.size();

			SDL_RWwrite(file, &type, sizeof(type), 1);
			SDL_RWwrite(file, &nameSize, SZ_INT, 1);
			SDL_RWwrite(file, name.data(), SZ_CHAR, nameSize);
			SDL_RWwrite(file, &data, sizeof(data), 1);
		}

		NBT_Type getType() override {
			throw Exception("Using NBT_Tag for a type not supported by NBT_Tag");
		}

		void setData(T dat) {
			data = dat;
			dirty = true;
		}

		T getData() const {
			return data;
		}

		string toString() override {
			//here to print out vectors
			using Utils::operator<<;

			if (dirty) {
				stringstream str;

				str << "NBT_Tag{" << name << ":" << data << "}";

				builtStr = str.str();
				dirty = false;
			}

			return builtStr;
		}

		friend ostream& operator <<(ostream& os, NBT_Tag<T>* nbt) {
			return os << nbt->toString();
		}

	private:
		T data;
	};

	template<>
	NBT_Base::NBT_Type NBT_Tag<bool>::getType() {
		return NBT_Base::NBT_BOOL;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<char>::getType() {
		return NBT_Base::NBT_CHAR;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<uchar>::getType() {
		return NBT_Base::NBT_UCHAR;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<short>::getType() {
		return NBT_Base::NBT_SHORT;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<ushort>::getType() {
		return NBT_Base::NBT_USHORT;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<int>::getType() {
		return NBT_Base::NBT_INT;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<uint>::getType() {
		return NBT_Base::NBT_UINT;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<long>::getType() {
		return NBT_Base::NBT_LONG;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<ulong>::getType() {
		return NBT_Base::NBT_ULONG;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<llong>::getType() {
		return NBT_Base::NBT_LLONG;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<ullong>::getType() {
		return NBT_Base::NBT_ULLONG;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<float>::getType() {
		return NBT_Base::NBT_FLOAT;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<double>::getType() {
		return NBT_Base::NBT_DOUBLE;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<string>::getType() {
		return NBT_Base::NBT_STRING;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<void*>::getType() {
		return NBT_Base::NBT_POINTER;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<Bingo::Math::DynVecN<int>>::getType() {
		return NBT_Base::NBT_INT_ARRAY;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<vector<int>>::getType() {
		return NBT_Base::NBT_INT_ARRAY;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<Bingo::Math::DynVecN<double>>::getType() {
		return NBT_Base::NBT_DOUBLE_ARRAY;
	}

	template<>
	NBT_Base::NBT_Type NBT_Tag<vector<double>>::getType() {
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
	void NBT_Tag<vector<int>>::writeData(SDL_RWops* file) {
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
	void NBT_Tag<vector<double>>::writeData(SDL_RWops* file) {
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

}

#endif