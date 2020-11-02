//Dustin Gehm

#include "Utils.h"

#include <iostream>
#include <math.h>

#include "ThreadManager.h"

using std::cerr;
using std::distance;
using std::floor;
using std::endl;
using std::pow;
using std::sqrt;

using Bingo::ThreadManager;
using namespace Bingo;

float Bingo::Utils::round(float f, int p) {
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

string Bingo::Utils::tabs(uint num) {
	return string(chars_beg, chars_beg + num);
}

void Bingo::Utils::tokenize(string& str, char separator) {
	enum TokenType { NONE, ALPHA, NUM, OTHER } curToken = NONE;
	TokenType nextToken = NONE;

	if (isalpha(str[0])) {
		curToken = ALPHA;
	}
	else if (isdigit(str[0])) {
		curToken = NUM;
	}
	else {
		curToken = OTHER;
	}

	for (auto iter = str.begin() + 1; iter != str.end(); iter++) {
		if (isalpha(*iter)) {
			nextToken = ALPHA;
		}
		else if (isdigit(*iter)) {
			nextToken = NUM;
		}
		else {
			nextToken = OTHER;
		}

		if (curToken != nextToken) {
			iter = str.insert(iter, separator);
			iter++;

			curToken = nextToken;
		}
	}
}

vector<uint> Bingo::Utils::tokenize(const string& str, const TokenizeType) {
	vector<uint> result;
	enum TokenType { NONE, ALPHA, NUM, OTHER } curToken = NONE;
	TokenType nextToken = NONE;

	result.push_back(0);

	if (isalpha(str[0])) {
		curToken = ALPHA;
	}
	else if (isdigit(str[0])) {
		curToken = NUM;
	}
	else {
		curToken = OTHER;
	}

	for (auto iter = str.begin() + 1; iter != str.end(); iter++) {
		if (isalpha(*iter)) {
			nextToken = ALPHA;
		}
		else if (isdigit(*iter)) {
			nextToken = NUM;
		}
		else {
			nextToken = OTHER;
		}

		if (curToken != nextToken) {
			result.push_back(distance(str.begin(), iter));
		}
	}

	return result;
}

string& Bingo::Utils::lstrip(string& s) {
	auto iter = s.begin();

	while (iter != s.end() && (isspace(*iter) || *iter == 0)) {
		iter = s.erase(iter);
	}

	return s;
}

string& Bingo::Utils::rstrip(string& s) {
	auto iter = s.rbegin();

	while (iter != s.rend() && (isspace(*iter) || *iter == 0)) {
		iter++;
		s.pop_back();
	}

	return s;
}

string& Bingo::Utils::strip(string& s) {
	lstrip(s);
	rstrip(s);

	return s;
}

void Bingo::Utils::Error(string msg) {
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg;
	BAIL(1);

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

void Bingo::Utils::Error(string msg, string error) {
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg << endl << error;
	BAIL(1);

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

void Bingo::Utils::Warn(string msg) {
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg;

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

void Bingo::Utils::Warn(string msg, string error) {
	ATOMIC_LOCK(ThreadManager::coutLock);

	cerr << msg << endl << error;

	ATOMIC_UNLOCK(ThreadManager::coutLock);
}

bool Bingo::Utils::operator==(SDL_Point& p1, SDL_Point& p2) {
	if (p1.x == p2.x) {
		if (p1.y == p2.y) {
			return true;
		}
	}

	return false;
}

string Bingo::Utils::operator+(const string& str, int num) {
	stringstream ss;

	ss << str;
	ss << num;

	return ss.str();
}

string Bingo::Utils::operator+(const string& str, uint num) {
	stringstream ss;

	ss << str;
	ss << num;

	return ss.str();
}