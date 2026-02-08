// IWDoubleList.h: interface for the CIWDoubleList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWDOUBLELIST_H__FF1643F1_98F5_4614_AD43_80DDF50F457C__INCLUDED_)
#define AFX_IWDOUBLELIST_H__FF1643F1_98F5_4614_AD43_80DDF50F457C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>			// for CList
#include "../_IWUnknown.h"
#include "_IDoubleList.h"
#include "_IChartOCX.h"			// for IChartOCX

class CIWDoubleList : public CIWUnknown, public IDoubleList
{
typedef CList<double, double> CDoubleList;
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IDoubleList, CIWDoubleList, CDoubleList, m_plDoubleList);

protected:
// Attributes (head and tail)
	// count of elements
	virtual int		GetCount() const;
	virtual BOOL	IsEmpty() const;

	// peek at head or tail
	virtual BOOL	GetHead( double &p_dData) const;
	virtual BOOL	GetTail( double &p_dData) const;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	virtual BOOL	RemoveHead( double &p_dData);
	virtual BOOL	RemoveTail( double &p_dData);

	// add before head or after tail
	virtual POSITION	AddHead( double newElement);
	virtual POSITION	AddTail( double newElement);

	// remove all elements
	virtual void	RemoveAll();

	// iteration
	virtual POSITION	GetHeadPosition() const;
	virtual POSITION	GetTailPosition() const;
	virtual BOOL	GetNext( POSITION& rPosition, double &p_dData) const; // return *Position++
	virtual BOOL	GetPrev( POSITION& rPosition, double &p_dData) const; // return *Position--

	// getting/modifying an element at a given position
	virtual BOOL	GetAt( POSITION position, double &p_dData) const;
	virtual void	SetAt( POSITION position, double newElement);
	virtual void	RemoveAt( POSITION position);

	// inserting before or after a given position
	virtual POSITION	InsertBefore( POSITION position, double newElement);
	virtual POSITION	InsertAfter( POSITION position, double newElement);

	// helper functions (note: O(n) speed)
	virtual POSITION	Find( double searchValue, POSITION startAfter = NULL) const;
		// defaults to starting at the HEAD, return NULL if not found
	virtual POSITION	FindIndex( int nIndex) const;
		// get the 'nIndex'th element (may return NULL)
};

#endif // !defined(AFX_IWDOUBLELIST_H__FF1643F1_98F5_4614_AD43_80DDF50F457C__INCLUDED_)
