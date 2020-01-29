//Dustin Gehm
/*a custom exception class that will be thrown
when the engine occurs a fatal runtime error*/

#pragma once
#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <exception>
#include <string>

using std::exception;
using std::string;

class Exception : public exception{
public:
	Exception(string desc);
	~Exception() throw();

	/*returns a c-string of the supplied description of the error*/
	virtual const char* what() const override;
private:
	string desc;
};

#endif