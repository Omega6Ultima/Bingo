//Dustin Gehm

#include "Utils.h"

#include <iostream>
#include <math.h>
#include <xtgmath.h>

#include "ThreadManager.h"

using std::cerr;
using std::distance;
using std::floor;
using std::endl;
using std::pow;
using std::sqrt;

float round(float f, int p){
	return (float)(floor(f * pow(10, p) + .5f) / pow(10, p));
}

//160 tabs
static string chars = "\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t"
"\t\t\t\t\t\t\t\t\t\t";
static string::iterator chars_beg = chars.begin();

string tabs(uint num){
	return string(chars_beg, chars_beg + num);
}

void tokenize(string& str, char separator){
	enum TokenType { NONE, ALPHA, NUM, OTHER } curToken = NONE;
	TokenType nextToken = NONE;

	if (isalpha(str[0])){
		curToken = ALPHA;
	}
	else if (isdigit(str[0])){
		curToken = NUM;
	}
	else{
		curToken = OTHER;
	}

	for (auto iter = str.begin() + 1; iter != str.end(); iter++){
		if (isalpha(*iter)){
			nextToken = ALPHA;
		}
		else if (isdigit(*iter)){
			nextToken = NUM;
		}
		else{
			nextToken = OTHER;
		}

		if (curToken != nextToken){
			iter = str.insert(iter, separator);
			iter++;

			curToken = nextToken;
		}
	}
}

vector<uint> tokenize(const string& str, uint&){
	vector<uint> result;
	enum TokenType { NONE, ALPHA, NUM, OTHER } curToken = NONE;
	TokenType nextToken = NONE;

	if (isalpha(str[0])){
		curToken = ALPHA;
	}
	else if (isdigit(str[0])){
		curToken = NUM;
	}
	else{
		curToken = OTHER;
	}

	for (auto iter = str.begin() + 1; iter != str.end(); iter++){
		if (isalpha(*iter)){
			nextToken = ALPHA;
		}
		else if (isdigit(*iter)){
			nextToken = NUM;
		}
		else{
			nextToken = OTHER;
		}

		if (curToken != nextToken){
			result.push_back(distance(str.begin(), iter));
		}
	}

	return result;
}

void Error(string msg){
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg;
	BAIL(1);

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

void Error(string msg, string error){
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg << endl << error;
	BAIL(1);

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

void Warn(string msg){
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg;

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

void Warn(string msg, string error){
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg << endl << error;

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

bool operator==(SDL_Point& p1, SDL_Point& p2){
	if (p1.x == p2.x){
		if (p1.y == p2.y){
			return true;
		}
	}

	return false;
}