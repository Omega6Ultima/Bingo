//Dustin Gehm

#include "Exception.h"

using Bingo::Exception;

Exception::Exception(string desc) {
	this->desc = desc;
}

Exception::Exception(const std::stringstream& stream) {
	this->desc = stream.str();
}

Exception::~Exception() throw() {
	//
}

const char* Exception::what() const {
	return desc.c_str();
}