//Dustin Gehm
//Miscellaneous utilities

#pragma once
#ifndef _UTILS_H
#define _UTILS_H

#include <iostream>
#include <string>
#include <vector>

#include <SDL_atomic.h>
#include <SDL_rect.h>

using std::cin;
using std::string;
using std::vector;

//less typing
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned long long ullong;
typedef long long llong;
typedef long double ldouble;

//also less typing
#define SZ_BOOL sizeof(bool)
#define SZ_CHAR sizeof(char)
#define SZ_UCHAR sizeof(uchar)
#define SZ_SHORT sizeof(short)
#define SZ_USHORT sizeof(ushort)
#define SZ_INT sizeof(int)
#define SZ_UINT sizeof(uint)
#define SZ_LONG sizeof(long)
#define SZ_ULONG sizeof(ulong)
#define SZ_LLONG sizeof(llong)
#define SZ_ULLONG sizeof(ullong)
#define SZ_FLOAT sizeof(float)
#define SZ_DOUBLE sizeof(double)
#define SZ_LDOUBLE sizeof(ldouble)
#define SZ_POINTER sizeof(void*)

//Pi, who doesnt like Pie
#define PI 3.14159265358979323846

//eulers constant
#define EULER 2.7182818284590452353602875

//simply pause and quit
#define BAIL(c) cin.get(); exit(c);

//returns whichever is greater, a or b
#define MAX(a, b) (a > b ? a : b)

//returns whichever is lesser, a or b
#define MIN(a, b) (a < b ? a : b)

//lock an atomic lock
#define ATOMIC_LOCK(lock) SDL_AtomicLock(&lock)

//unlock an atomic lock
#define ATOMIC_UNLOCK(lock) SDL_AtomicUnlock(&lock)

//atomic-type lock
typedef SDL_SpinLock AtomicLock;

//round a float (f) to a number of decimal places (p)
float round(float f, int p);

//convert degrees to radians
inline double degToRadians(double deg){
	return (deg / 180.0) * PI;
}

//conver radians to degrees
inline double radToDegrees(double rad){
	return (rad * PI) / 180.0;
}

//returns a certain number (num) of tab characters
string tabs(uint num);

//insert separator character between different token types
void tokenize(string& str, char separator = ' ');
//return a vector of indices where different token types begin
vector<uint> tokenize(const string& str, uint& dummy);

void Error(string msg);
void Error(string msg, string error);

void Warn(string msg);
void Warn(string msg, string errror);

bool operator==(SDL_Point& p1, SDL_Point& p2);

#endif