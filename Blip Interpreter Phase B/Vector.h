// Vector.h
// Written by Dr. Chase and edited by Gopika Ajaykumar (edits commented)
// Contains the class Vector (similar to the C++ Vector) and its functions
// Last updated 04/28/2016

#pragma once
#ifndef _VECTOR_h
#define _VECTOR_h

// Included Files
#include <stdint.h>
#include "StatementTree.h"	// Contains the StatementTree class and its function

constexpr uint32_t minimum_size = 8;
template <typename T>		// Edit: This code is a template. For this solution, we use the StatementTree::Node* type.

struct Vector
{
private:
	T* data;
	uint32_t len;
	uint32_t cap;
public:
	uint32_t size(void) { // implicit param named "this"
		return this->len;
	}

	Vector(void) {
		len = 0;
		cap = 8;
		data = new T[cap];
	}

	Vector(Vector& orig) { copy(orig); } // copy constructor (hence, I called "copy")

	Vector& operator=(Vector& rhs) {
		if (this != &rhs) {
			destroy();
			copy(rhs);
		}
		return *this;
	}

	~Vector(void) { destroy(); }

	Vector(uint32_t sz) {
		(*this).len = sz;
		(*this).cap = sz;
		if ((*this).cap < minimum_size) {
			(*this).cap = minimum_size;
		}

		(*this).data = new int[(*this).cap];

		for (int32_t k = 0; k < (*this).len; k += 1) {
			(*this).data[k] = 0;
		}

	}

	void destroy(void) {
		delete[] data;
	}

	void copy(Vector& orig) {
		this->len = orig.len;
		this->cap = this->len;
		if (this->len < minimum_size) {
			this->cap = minimum_size;
		}
		this->data = new T[this->cap];
		for (uint32_t k = 0; k < len; k += 1) {
			this->data[k] = orig.data[k];
		}
	}

	void check_range(uint32_t k) {
		if (k >= this->len) {
			printf("out of range index %d in vector with length %d\n", k, this->len);
			abort();
		}
	}

	T& Vector::operator[](uint32_t k) {
		this->check_range(k);
		return this->data[k];
	}

	void push_back(T val) {
		if ((*this).len == (*this).cap) {
			(*this).cap *= 2;

			T* new_data = new T[cap];

			for (uint32_t k = 0; k < (*this).len; k += 1) {
				new_data[k] = (*this).data[k];
			}

			delete[] data;

			(*this).data = new_data;
			(*this).data[(*this).len] = val;
		}
		(*this).data[(*this).len] = val;
		(*this).len += 1;
	}

};



#endif 