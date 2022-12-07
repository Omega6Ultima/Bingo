//Dustin Gehm

#include "Utils.h"

#include <iostream>
#include <math.h>
#include <sstream>

#include "ThreadManager.h"

using std::cerr;
using std::distance;
using std::floor;
using std::endl;
using std::pow;
using std::sqrt;
using std::stringstream;

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

string Bingo::Utils::lstrip(const string& s) {
	string str(s);
	auto iter = str.begin();

	while (iter != str.end() && (isspace(*iter) || *iter == 0)) {
		iter = str.erase(iter);
	}

	return str;
}

string Bingo::Utils::rstrip(const string& s) {
	string str(s);
	auto iter = str.rbegin();

	while (iter != str.rend() && (isspace(*iter) || *iter == 0)) {
		iter++;
		str.pop_back();
	}

	return str;
}

string Bingo::Utils::strip(const string& s) {
	string str(s);

	str = lstrip(str);
	str = rstrip(str);

	return str;
}

vector<string> Bingo::Utils::split(const string& s, string separator) {
	vector<string> result;
	vector<uint> foundIndices;
	uint found = s.find(separator.c_str());

	while (found != string::npos) {
		foundIndices.push_back(found);

		found = s.find(separator.c_str(), found + 1);
	}

	foundIndices.push_back(string::npos);

	uint offset = 0;

	for (uint c = 0; c < foundIndices.size(); c++) {
		result.push_back(s.substr(offset, foundIndices[c] - offset));

		offset = foundIndices[c] + 1;
	}

	return result;
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