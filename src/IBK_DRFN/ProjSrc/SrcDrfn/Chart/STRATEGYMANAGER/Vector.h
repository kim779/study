// Vector.h: interface for the CVector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOR_H__379D1F05_6AFB_4B0F_89E3_6A932EA8BDCB__INCLUDED_)
#define AFX_VECTOR_H__379D1F05_6AFB_4B0F_89E3_6A932EA8BDCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <exception> // exception class
#include<new>
#include<iostream>
using namespace std;

extern void raise_exception(int _L, const char* _F, ...);

template <class VALUE_TYPE>
class CVector
{
public:
	class iterator
	{
		friend class CVector<VALUE_TYPE>;
	// Constructors
	public:
		iterator() : _cur(0), _data(0) {
		}

		iterator(const iterator& src) : _cur(src._cur), _data(src._data) {
		}

	// Attributes
	public:

	// Operations
	public:
		iterator& operator++() {
			++_cur; return *this;
		}
		iterator operator++(int) {
			iterator tmp(*this); _cur++; return tmp;
		}
		iterator& operator--() {
			--_cur; return *this;
		}
		iterator operator--(int) {
			iterator tmp(*this); _cur--; return tmp;
		}
		iterator& operator+=(const int n) {
			_cur += n; return *this;
		}
		iterator& operator-=(const int n) {
			_cur -= n; return *this;
		}
		iterator operator+(const int n) {
			return iterator(_cur + n, _data);
		}
		iterator operator-(const int n) {
			return iterator(_cur - n, _data);
		}

		bool operator==(const iterator& src) const {
			return (_cur == src._cur);
		}
		bool operator!=(const iterator& src) const {
			return (_cur != src._cur);
		}
		bool operator<(const iterator& src) const {
			return (_cur < src._cur);
		}
		bool operator>(const iterator& src) const {
			return (_cur > src._cur);
		}
		bool operator<=(const iterator& src) const {
			return (_cur <= src._cur);
		}
		bool operator>=(const iterator& src) const {
			return (_cur >= src._cur);
		}

		VALUE_TYPE& operator*() {
			return _data->at(_cur);
		}
		VALUE_TYPE& operator[](const int i) {
			return _data->at(_cur + i);
		}

		iterator& operator=(const iterator& src) {
			_cur = src._cur; _data = src._data; return *this;
		}

	// Implementation
	protected:
		explicit iterator(const int _c, CVector<VALUE_TYPE>* _d)
			: _cur(_c), _data(_d) {
		}

		size_t _cur;
		CVector<VALUE_TYPE>* _data;
	};

// Constructors
public:
	CVector(size_t size = 128);
	CVector(const CVector<VALUE_TYPE>& src);
	CVector<VALUE_TYPE>& operator=(const CVector<VALUE_TYPE>& src);
	void resize(size_t size);

// Attributes
public:
	size_t size() {
		return _n;
	}
	size_t capacity() {
		return _max;
	}
	bool empty() {
		return (_n == 0) ? true : false;
	}

// Operations
public:
    void push_back(VALUE_TYPE newElement) {
		insert(_n, newElement);
	}
    VALUE_TYPE pop_back();
    void insert(int index, VALUE_TYPE newElement);
    VALUE_TYPE remove(int index);
    void clear() {
		_n = 0;
	}

	VALUE_TYPE& at(int index) const;
    VALUE_TYPE& operator[](int index);
    const VALUE_TYPE& operator[](int index) const;

	iterator begin() {
		return iterator(0, this);
	}
	iterator end() {
		return iterator(_n, this);
	}

// Implementation
public:
	virtual ~CVector();

protected:
	size_t _max;
	size_t _by;
	size_t _n;
	VALUE_TYPE* _data;
};

/////////////////////////////////////////////////////////////////////////////
// CVector

template <class VALUE_TYPE>
CVector<VALUE_TYPE>::CVector(size_t size)
	:
	_max(0),
	_by(128),
	_n(0),
	_data(NULL)
{
	resize(size);
}

template <class VALUE_TYPE>
CVector<VALUE_TYPE>::CVector(const CVector<VALUE_TYPE>& src)
	:
	_max(0),
	_n(0),
	_data(NULL)
{
	_by = src.by;

	if (src._n == 0)
		return;

	resize(src._n);

	for (int i = 0; i < src._n; i++)
		_data[i] = src._data[i];

	_n = src._n;
}

template <class VALUE_TYPE>
CVector<VALUE_TYPE>::~CVector()
{
	_max = 0;
	_n = 0;
	if (_data != NULL)
		delete[] _data;
	_data = NULL;
}

template <class VALUE_TYPE>
CVector<VALUE_TYPE>&
CVector<VALUE_TYPE>::operator=(const CVector<VALUE_TYPE>& src)
{
	if (&src == this) // check for self-assignment
		return;

	_n = 0; // no backup

	resize(src._n);

	for (int i = 0; i < src._n; i++)
		data[i] = src._data[i];

	_n = src._n;

	return *this;
}

template <class VALUE_TYPE>
void CVector<VALUE_TYPE>::resize(size_t size)
{
	size_t new_max = (((_max + size) / _by) + 1) * _by;

	VALUE_TYPE* new_data;
	try
	{
		new_data = new VALUE_TYPE[new_max];
	}
	catch (std::bad_alloc&)
	{
		raise_exception(__LINE__, "Out of memory - size %d", new_max);
	}

	_max = new_max;

	if (_data == NULL)
	{
		_data = new_data;
		return;
	}

	for (int i = 0; i < _n; i++)
		new_data[i] = _data[i];
	delete[] _data;

	_data = new_data;
}

/////////////////////////////////////////////////////////////////////////////
// Attributes

/////////////////////////////////////////////////////////////////////////////
// Operations

template <class VALUE_TYPE>
VALUE_TYPE CVector<VALUE_TYPE>::pop_back()
{
	VALUE_TYPE element;

	if (_n == 0)
		raise_exception(__LINE__, "Out of bounds - size %d", _n);

	element = _data[--_n];

	return element;
}

template <class VALUE_TYPE>
void CVector<VALUE_TYPE>::insert(int index, VALUE_TYPE newElement)
{
	int new_n;

	if (index > _n)
		new_n = index + 1;
	else
		new_n = _n + 1;

	if (new_n > _max)
		raise_exception(__LINE__, "Out of bounds - capacity %d size %d", _max, new_n);

	_n = new_n;

	for (int i = index; i < (_n - 1); i++)
		_data[i + 1] = _data[i];

	_data[index] = newElement;
}

template <class VALUE_TYPE>
VALUE_TYPE CVector<VALUE_TYPE>::remove(int index)
{
	VALUE_TYPE element;

	if (_n == 0)
		raise_exception(__LINE__, "Out of bounds - size %d", _n);

	--_n;

	element = _data[index];

	for (int i = index; i < _n; i++)
		_data[i] = _data[i + 1];

	return element;
}

template <class VALUE_TYPE>
VALUE_TYPE& CVector<VALUE_TYPE>::operator[](int index)
{
	if (index >= _max)
		raise_exception(__LINE__, "Out of bounds - capacity %d, index %d", _max, index);

	if (index >= _n)
		_n = index + 1;

	return _data[index];
}

template <class VALUE_TYPE>
const VALUE_TYPE& CVector<VALUE_TYPE>::operator[](int index) const
{
	if (index >= _max)
		raise_exception(__LINE__, "Out of bounds - capacity %d, index %d", _max, index);

	if (index >= _n)
		_n = index + 1;

	return _data[index];
}

template <class VALUE_TYPE>
VALUE_TYPE& CVector<VALUE_TYPE>::at(int index) const
{
	if (index >= _n)
		raise_exception(__LINE__, "Out of bounds - capacity %d, index %d", _max, index);

	return _data[index];
}

#endif // !defined(AFX_VECTOR_H__379D1F05_6AFB_4B0F_89E3_6A932EA8BDCB__INCLUDED_)
