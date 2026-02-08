// DequeList.h: interface for the CDequeList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEQUELIST_H__C7CF366B_E58E_421C_AAC6_E146E3AC9155__INCLUDED_)
#define AFX_DEQUELIST_H__C7CF366B_E58E_421C_AAC6_E146E3AC9155__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <cassert>
#include <deque>
#include <algorithm>
#include <functional>

using namespace std;

template <class T> class CDequeList
{
public:
	CDequeList& operator=(const CDequeList& data)
	{
		if(this == &data)
			return *this;

		SetDeque(data.GetDeque());
		return *this;
	};

public:
	CDequeList() {};
	CDequeList(const deque<T>& dequeData) { m_deque = dequeData; };
	~CDequeList() { m_deque.clear(); };


	const deque<T>& GetDeque() const
	{
		return m_deque;
	};

	bool IsExisting(const int nIndex) const
	{
		return (nIndex >= 0 && nIndex < m_deque.size());
	};

	int GetTailPosition() const { return m_deque.end() -1; };
	
	int GetCount() const { return m_deque.size(); };

	T& GetHead()
	{
		assert(!m_deque.empty());
		return m_deque.front();
	};
	T GetHead() const
	{
		assert(!m_deque.empty());
		return m_deque.front();
	};


	T& GetTail()
	{
		assert(!m_deque.empty());
		return m_deque.back();
	};
	T GetTail() const
	{
		assert(!m_deque.empty());
		return m_deque.back();
	};

	T& GetAt(const int nIndex)
	{
		assert(!m_deque.empty());
		assert(IsExisting(nIndex));
		return m_deque[nIndex];
	};
	T GetAt(const int nIndex) const
	{
		assert(!m_deque.empty());
		assert(IsExisting(nIndex));
		return m_deque[nIndex];
	};

	//test 안한것임
	typename deque<T>::iterator GetIterator(const T& item) const
	{
		 return lower_bound(m_deque.begin(), m_deque.end(), item);
	};
	typename deque<T>::iterator GetIterator(const int nIndex) const
	{
		typename deque<T>::iterator i = m_deque.begin();
		advance(i, nIndex);
		return i;
	};
	//


	int GetIndex(const T& item) const
	{
		for(int nIndex = 0; nIndex < m_deque.size(); nIndex++){
			if(item == m_deque[nIndex])
				return nIndex;
		}
		return -1;
	};


	void SetAt(const int nIndex, const T& item)
	{
		if(!IsExisting(nIndex))
			return;
		m_deque[nIndex] = item;
	};

	void AddHead(const T& item){ m_deque.push_front(item); };
	void AddTail(const T& item){ m_deque.push_back(item); };
	void InsertItem(const int nIndex, const T& item)
	{
		assert(nIndex >= 0 && nIndex <= m_deque.size());
		deque<T>::iterator i = m_deque.begin();
		advance(i, nIndex);
		m_deque.insert(i, x);
	};

	void RemoveAll(){ m_deque.clear(); };
	void RemoveTail()
	{
		m_deque.pop_back();
	};
	void RemoveAt(const int nIndex)
	{
		assert(IsExisting(nIndex));
		m_deque.erase(m_deque.begin() + nIndex);
	};

	void MoveItem(const int sourceIndex, const int targetIndex)
	{
		assert(sourceIndex >= 0 && sourceIndex < m_deque.size());
		assert(targetIndex >= 0 && targetIndex <= m_deque.size());

		T t = GetAt(sourceIndex);
		InsertItem(targetIndex, t);
		if(sourceIndex <= targetIndex)
			RemoveAt(sourceIndex);
		else
			RemoveAt(sourceIndex +1);
	};

private:
	deque<T> m_deque;

	bool SetDeque(const deque<T>& dequeData)
	{
		if(dequeData.size() <= 0)
			return false;

		for(int nIndex = 0; nIndex < dequeData.size(); nIndex++){
			m_deque.push_back(dequeData[nIndex]);
		}
		return true;
	};
};


template <class T> class CDequePointerList
{
public:
	~CDequePointerList() { m_deque.clear(); };

	bool IsExisting(const int nIndex) const
	{
		return (nIndex >= 0 && nIndex < m_deque.size());
	};

	int GetTailPosition() const { return m_deque.end() -1; };
	
	int GetCount() const { return m_deque.size(); };

	T* GetHead() const
	{
		if(m_deque.empty())
			return NULL;
		return m_deque.front();
	};

	T* GetTail() const
	{
		if(m_deque.empty())
			return NULL;
		return m_deque.back();
	};

	T* GetAt(const int nIndex)
	{
		if(m_deque.empty())
			return NULL;
		assert(IsExisting(nIndex));
		return m_deque[nIndex];
	};
	T* GetAt(const int nIndex) const
	{
		if(m_deque.empty())
			return NULL;
		assert(IsExisting(nIndex));
		return m_deque[nIndex];
	};

	int GetIndex(const T* pItem) const
	{
		assert(pItem != NULL);
		for(int nIndex = 0; nIndex < m_deque.size(); nIndex++){
			if(pItem == m_deque[nIndex])
				return nIndex;
		}
		return -1;
	};

	void AddHead(T* pItem)
	{
		assert(pItem != NULL);
		m_deque.push_front(pItem);
	};
	void AddTail(T* pItem)
	{
		assert(pItem != NULL);
		m_deque.push_back(pItem);
	};

	void InsertItem(const int nIndex, T* pItem)
	{
		assert(pItem != NULL);
		assert(nIndex >= 0 && nIndex <= m_deque.size());

		deque<T*>::iterator i = m_deque.begin();
		advance(i, nIndex);
		m_deque.insert(i, pItem);
	};

	void RemoveAll(){ m_deque.clear(); };
	void RemoveTail()
	{
		m_deque.pop_back();
	};
	void RemoveAt(T* pItem)
	{
		assert(pItem != NULL);
		deque<T*>::iterator i = find(m_deque.begin(), m_deque.end(), pItem);
		if(i == m_deque.end())
			return;

		m_deque.erase(i);
	};
	void RemoveAt(const int nIndex)
	{
		assert(IsExisting(nIndex));
		m_deque.erase(m_deque.begin() + nIndex);
	};

	void MoveItem(const int sourceIndex, const int targetIndex)
	{
		if(sourceIndex == targetIndex)
			return;

		assert(sourceIndex >= 0 && sourceIndex < m_deque.size());
		assert(targetIndex >= 0 && targetIndex <= m_deque.size());

		T* pT = GetAt(sourceIndex);
		assert(pT != NULL);
		RemoveAt(sourceIndex);
		InsertItem(targetIndex, pT);
	};

private:
	deque<T*> m_deque;

};

#endif // !defined(AFX_DEQUELIST_H__C7CF366B_E58E_421C_AAC6_E146E3AC9155__INCLUDED_)
