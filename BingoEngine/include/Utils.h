//Dustin Gehm
//Miscellaneous utilities

#pragma once
#ifndef _UTILS_H
#define _UTILS_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <SDL_atomic.h>
#include <SDL_rect.h>

using std::cin;
using std::iter_swap;
using std::ostream;
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

namespace Bingo {

	namespace Utils {

		//atomic-type lock
		typedef SDL_SpinLock AtomicLock;

		//round a float (f) to a number of decimal places (p)
		float round(float f, int p);

		//convert degrees to radians
		inline double degToRadians(double deg) {
			return (deg / 180.0) * PI;
		}

		//conver radians to degrees
		inline double radToDegrees(double rad) {
			return (rad * PI) / 180.0;
		}

		//returns a certain number (num) of tab characters
		string tabs(uint num);

		enum class TokenizeType {
			RetVec,
		};

		//insert separator character between different token types
		void tokenize(string& str, char separator = ' ');
		//return a vector of indices where different token types begin
		vector<uint> tokenize(const string& str, const TokenizeType dummy);
		//remove whitespace from left side of the string
		string& lstrip(string& s);
		//remove whitespace from right side of the string
		string& rstrip(string& s);
		//remove whitespace from both sides of the string
		string& strip(string& s);

		void Error(string msg);
		void Error(string msg, string error);

		void Warn(string msg);
		void Warn(string msg, string errror);

		bool operator==(SDL_Point& p1, SDL_Point& p2);

		string operator+(const string& str, int num);
		string operator+(const string& str, uint num);

		//TODO consider putting these operators into their own namespaces to label them
		template<class T>
		ostream& operator<<(ostream& os, vector<T>& vec) {
			os << "v{";

			if (vec.size() > 0) {
				for (auto iter = vec.begin(); iter != vec.end(); iter++) {
					os << *iter << ", ";
				}

				//print 2 backspace characters to erase the last comma
				os << "\b\b}";
			}
			else {
				os << '}';
			}

			return os;
		}

	}

	namespace Sort {
		template<class T>
		void selectionSort(vector<T>& vec) {
			uint sortedPos = 0;
			//vector<T>::iterator minIter;
			auto minIter = vec.begin();

			while (true) {
				minIter = vec.begin() + sortedPos;

				if (minIter == vec.end()) {
					break;
				}

				for (auto iter = vec.begin() + sortedPos + 1; iter != vec.end(); iter++) {
					if ((*iter) < (*minIter)) {
						minIter = iter;
					}
				}

				iter_swap(vec.begin() + sortedPos, minIter);
				sortedPos++;
			}
		}

		template<class T>
		void insertionSort(vector<T>& vec) {
			for (uint c = 1; c < vec.size(); c++) {
				if (vec[c] < vec[c - 1]) {
					uint index = c - 1;

					while (index > 0 && (vec[c] < vec[index - 1])) {
						index--;
					}

					vec.insert(vec.begin() + index, vec[c]);
					vec.erase(vec.begin() + c + 1);
				}
			}
		}

		template<class T>
		void mergeSort(vector<T>& vec) {
			//TODO implement this
		}

		namespace InternalUse {
			template<class T>
			int qs_partition(vector<T>& vec, int low, int high) {
				T pivot = vec[high];
				int minIndex = low - 1;

				for (int c = low; c < high; c++) {
					if (vec[c] < pivot) {
						minIndex++;

						iter_swap(vec.begin() + minIndex, vec.begin() + c);
					}
				}

				iter_swap(vec.begin() + (minIndex + 1), vec.begin() + high);

				return minIndex + 1;
			}
		}

		template<class T>
		void quickSort(vector<T>& vec, int low, int high) {
			if (low < high) {
				int pivotIndex = InternalUse::qs_partition(vec, low, high);

				quickSort(vec, low, pivotIndex - 1);
				quickSort(vec, pivotIndex + 1, high);
			}
		}

		template<class T>
		void quickSort(vector<T>& vec) {
			quickSort(vec, 0, vec.size() - 1);
		}

		//TODO https://www.geeksforgeeks.org/sorting-algorithms/
	}

}

#endif