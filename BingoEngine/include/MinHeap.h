#pragma once
#ifndef _MINHEAP_H
#define _MINHEAP_H

#include <vector>

#include "Exception.h"
#include "Utils.h"

using std::iter_swap;
using std::vector;

using Bingo::Exception;

namespace Bingo {

	template<class T>
	class MinHeap {
	public:
		//a function that compares val1 and val2, return -1 for less than, 0 for equal, and 1 for greater than
		typedef int (*ComparatorFunc)(T val1, T val2);

		MinHeap(T dummyValue) {
			values.push_back(T(dummyValue));
		}

		~MinHeap() {
			//
		}

		void setComparatorFunc(ComparatorFunc func) {
			compFunc = func;
		}

		void push(T val) {
			values.push_back(val);

			uint index = values.size() - 1;
			bool done = false;

			while (!done) {
				uint parentIndex = getParent(index);

				if (parentIndex == 0) {
					done = true;
				}
				else {
					if ((compFunc && (compFunc(values[index], values[parentIndex]) < 0)) ||
						(values[index] < values[parentIndex])) {
						iter_swap(values.begin() + index, values.begin() + parentIndex);
					}
				}

				index = parentIndex;
			}
		}

		T pop() {
			if (values.size() == 1) {
				throw Exception("Popping from an empty MinHeap");
			}

			iter_swap(values.begin() + 1, values.begin() + (values.size() - 1));

			T result = values.back();
			values.pop_back();

			uint index = 1;
			bool done = false;

			while (!done) {
				T* leftChild = NULL;
				T* rightChild = NULL;

				uint leftIndex = getLeftChild(index);
				uint rightIndex = getRightChild(index);

				if (leftIndex < values.size() &&
					((compFunc && (compFunc(values[index], values[leftIndex]) >= 0)) || (values[index] >= values[leftIndex]))) {
					leftChild = &values[leftIndex];
				}

				if (rightIndex < values.size() &&
					((compFunc && (compFunc(values[index], values[rightIndex]) >= 0)) || (values[index] >= values[rightIndex]))) {
					rightChild = &values[rightIndex];
				}

				if (leftChild && rightChild) {
					if (*leftChild < *rightChild) {
						iter_swap(values.begin() + index, values.begin() + leftIndex);
						index = leftIndex;
					}
					else {
						iter_swap(values.begin() + index, values.begin() + rightIndex);
						index = rightIndex;
					}
				}
				else if (leftChild && !rightChild) {
					iter_swap(values.begin() + index, values.begin() + leftIndex);
					index = leftIndex;
				}
				else if (!leftChild && rightChild) {
					iter_swap(values.begin() + index, values.begin() + rightIndex);
					index = rightIndex;
				}
				else if (!leftChild && !rightChild) {
					done = true;
				}
			}

			return result;
		}

		inline bool empty() const {
			return values.size() == 1;
		}

		bool exist(const T& val) const {
			for (auto iter = values.begin(); iter != values.end(); iter++) {
				if (compFunc && (compFunc(*iter, val) == 0) ||
					(*iter == val)) {
					return true;
				}
			}

			return false;
		}

	private:
		inline uint getParent(uint ind) const {
			return ind >> 1;
		}

		inline uint getLeftChild(uint ind) const {
			return ind << 1;
		}

		inline uint getRightChild(uint ind) const {
			return (ind << 1) + 1;
		}

	private:
		vector<T> values;
		ComparatorFunc compFunc = NULL;
	};

}

#endif