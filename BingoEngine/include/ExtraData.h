//Dustin Gehm

#pragma once
#ifndef _EXTRA_DATA_H
#define _EXTRA_DATA_H

#include "NBT.h"

using Bingo::NBT_Compound;
using Bingo::NBT_Tag;

class ExtraData {
public:
	ExtraData();
	~ExtraData();

	template<class T>
	void addData(string name, T data);

	template<class T>
	T getData(string name);

private:
	NBT_Compound* nbt = NULL;
};

#endif