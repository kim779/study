// Stack.h: interface for the CStack class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACK_H__E803AD2B_9953_4F3A_A8F4_7FEBF563F684__INCLUDED_)
#define AFX_STACK_H__E803AD2B_9953_4F3A_A8F4_7FEBF563F684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <exception> // exception class
#include<new>
#include<iostream>
using namespace std;

#include "Vector.h"
template <class VALUE_TYPE>
class CStack : public CVector<VALUE_TYPE>
{
// Constructors
public:
	CStack(size_t size = 128) : CVector<VALUE_TYPE>(size) {
	}
	CStack(const CStack<VALUE_TYPE>& src) : CVector<VALUE_TYPE>(src) {
	}
	CStack<VALUE_TYPE>& operator=(const CStack<VALUE_TYPE>& src) {
		CVector<VALUE_TYPE>::operator=(src); return *this;
	}

// Attributes
public:

// Operations
public:
	VALUE_TYPE pop() {
		return CVector<VALUE_TYPE>::pop_back();
	}
	void push(VALUE_TYPE newElement) {
		CVector<VALUE_TYPE>::push_back(newElement);
	}

	VALUE_TYPE top() const;

// Implementation
public:
	virtual ~CStack() {
	}
};

template <class VALUE_TYPE>
VALUE_TYPE CStack<VALUE_TYPE>::top() const
{
	if (_n == 0)
		raise_exception(__LINE__, "Out of bounds - topIndex %d", _n);

	return _data[_n - 1];
}


#endif // !defined(AFX_STACK_H__E803AD2B_9953_4F3A_A8F4_7FEBF563F684__INCLUDED_)
