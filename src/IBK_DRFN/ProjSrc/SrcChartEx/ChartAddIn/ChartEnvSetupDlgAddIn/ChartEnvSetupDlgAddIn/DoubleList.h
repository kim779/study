// DoubleList.h: interface for the CDoubleList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOUBLELIST_H__0EF56D9C_F414_4A26_8CD5_85F8E102368F__INCLUDED_)
#define AFX_DOUBLELIST_H__0EF56D9C_F414_4A26_8CD5_85F8E102368F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Include_AddIn.h"		// for IDoubleList

class CDoubleList : public IDoubleList, public CIWUnknown
{
// (2004.10.07, 배승원) Interface Wrapper에서 IUnknown의 Interface를 손쉽게 지원할 수 있도록 Macro를 정의한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()
	CList< double, double> m_lstDouble;
	
public:

// Attributes (head and tail)
	// count of elements
	virtual int		GetCount() const	{	return	m_lstDouble.GetCount();	}
	virtual BOOL	IsEmpty() const		{	return	m_lstDouble.IsEmpty();	}

	// peek at head or tail
	virtual BOOL	GetHead( double &p_dData) const	{	p_dData = m_lstDouble.GetHead();	return TRUE;	}
	virtual BOOL	GetTail( double &p_dData) const	{	p_dData = m_lstDouble.GetTail();	return TRUE;	}

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	virtual BOOL	RemoveHead( double &p_dData)	{	p_dData = m_lstDouble.RemoveHead();	return TRUE;	}
	virtual BOOL	RemoveTail( double &p_dData)	{	p_dData = m_lstDouble.RemoveTail();	return TRUE;	}

	// add before head or after tail
	virtual POSITION	AddHead( double newElement)	{	return	m_lstDouble.AddHead( newElement);	}
	virtual POSITION	AddTail( double newElement)	{	return	m_lstDouble.AddTail( newElement);	}

	// remove all elements
	virtual void	RemoveAll()	{	m_lstDouble.RemoveAll();	}

	// iteration
	virtual POSITION	GetHeadPosition() const	{	return m_lstDouble.GetHeadPosition();	}
	virtual POSITION	GetTailPosition() const	{	return m_lstDouble.GetTailPosition();	}
	virtual BOOL	GetNext( POSITION& rPosition, double &p_dData) const	{	p_dData = m_lstDouble.GetNext( rPosition);	return TRUE;	}
	virtual BOOL	GetPrev( POSITION& rPosition, double &p_dData) const	{	p_dData = m_lstDouble.GetPrev( rPosition);	return TRUE;	}

	// getting/modifying an element at a given position
	virtual BOOL	GetAt( POSITION position, double &p_dData) const	{	p_dData = m_lstDouble.GetAt( position);	return TRUE;	}
	virtual void	SetAt( POSITION pos, double newElement)				{	m_lstDouble.SetAt( pos, newElement);	}
	virtual void	RemoveAt( POSITION position)						{	m_lstDouble.RemoveAt( position);	}

	// inserting before or after a given position
	virtual POSITION	InsertBefore( POSITION position, double newElement)	{	return m_lstDouble.InsertBefore( position, newElement);	}
	virtual POSITION	InsertAfter( POSITION position, double newElement)	{	return m_lstDouble.InsertAfter( position, newElement);	}

	// helper functions (note: O(n) speed)
	virtual POSITION	Find( double searchValue, POSITION startAfter = NULL) const	{	return m_lstDouble.Find( searchValue, startAfter);	}
		// defaults to starting at the HEAD, return NULL if not found
	virtual POSITION	FindIndex( int nIndex) const								{	return m_lstDouble.FindIndex( nIndex);	}
		// get the 'nIndex'th element (may return NULL)
};

#endif // !defined(AFX_DOUBLELIST_H__0EF56D9C_F414_4A26_8CD5_85F8E102368F__INCLUDED_)
