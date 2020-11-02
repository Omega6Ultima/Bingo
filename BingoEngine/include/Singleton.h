//Dustin Gehm
/*a class used to manage only instantiating
one of a class and accessing that class
where ever it is needed
Simply derive from this class to make
a new class a singleton*/

#pragma once
#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <iostream>

using std::cerr;

#include "Utils.h"

namespace Bingo {

	template<class T>
	class Singleton {
	public:
		Singleton<T>() {
#if _DEBUG
			if (mSingleton) {
				cerr << "Singleton was defined twice!!\n";
				BAIL(1);
			}
#endif
			mSingleton = (T*)this;
		}
		~Singleton<T>() {
#if _DEBUG
			if (mSingleton == NULL) {
				cerr << "That't not even... just what happened!?!\n";
				BAIL(1);
			}
#endif
		}
		/*returns a reference to the singleton instance*/
		static T& getSingleton() {
#ifdef _DEBUG
			if (!mSingleton) {
				cerr << "Singleton not initialized!!\n";
				BAIL(1);
			}
#endif
			return *mSingleton;
		}
		/*returns a pointer to the singleton instance*/
		static T* getSingletonPtr() {
#ifdef _DEBUG
			if (!mSingleton) {
				cerr << "Singleton not initialized!!\n";
				BAIL(1);
			}
#endif
			return mSingleton;
		}
	protected:
		static T* mSingleton;
	private:
		Singleton(const Singleton<T>& sing);
		Singleton& operator =(const Singleton<T>& sing);
	};

	template<class T>
	T* Singleton<T>::mSingleton = NULL;

}

#endif