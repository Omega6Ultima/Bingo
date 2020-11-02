//Dustin Gehm

#include "Exception.h"

using Bingo::Exception;

Exception::Exception(string desc) {
	this->desc = desc;
}

Exception::~Exception() throw() {
	//
}

const char* Exception::what() const {
	return desc.c_str();
}