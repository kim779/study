// IDblList.h: interface for the IDblList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDBLLIST_H__27BD74B5_D471_4C28_89E5_208ADCDD0F55__INCLUDED_)
#define AFX_IDBLLIST_H__27BD74B5_D471_4C28_89E5_208ADCDD0F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//********************************************************************
// Caution! Do not update or delete or add in old interface
//	This header was released already to OCX Wrapper etc.
//********************************************************************

interface IDblList : public IUnknown
{
protected:
	virtual ~IDblList()	{}

public:
// Attributes (head and tail)
	// count of elements
	virtual int		GetCount() const	= 0;
	virtual BOOL	IsEmpty() const		= 0;

	// peek at head or tail
	virtual BOOL	GetHead( double &p_dData) const	= 0;
	virtual BOOL	GetTail( double &p_dData) const	= 0;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	virtual BOOL	RemoveHead( double &p_dData)	= 0;
	virtual BOOL	RemoveTail( double &p_dData)	= 0;

	// add before head or after tail
	virtual POSITION	AddHead( double newElement)	= 0;
	virtual POSITION	AddTail( double newElement)	= 0;

	// remove all elements
	virtual void	RemoveAll()	= 0;

	// iteration
	virtual POSITION	GetHeadPosition() const				= 0;
	virtual POSITION	GetTailPosition() const				= 0;
	virtual BOOL	GetNext( POSITION& rPosition, double &p_dData) const	= 0; // return *Position++
	virtual BOOL	GetPrev( POSITION& rPosition, double &p_dData) const	= 0; // return *Position--

	// getting/modifying an element at a given position
	virtual BOOL	GetAt( POSITION position, double &p_dData) const	= 0;
	virtual void	SetAt( POSITION pos, double newElement)				= 0;
	virtual void	RemoveAt( POSITION position)						= 0;

	// inserting before or after a given position
	virtual POSITION	InsertBefore( POSITION position, double newElement)	= 0;
	virtual POSITION	InsertAfter( POSITION position, double newElement)	= 0;

	// helper functions (note: O(n) speed)
	virtual POSITION	Find( double searchValue, POSITION startAfter = NULL) const	= 0;
		// defaults to starting at the HEAD, return NULL if not found
	virtual POSITION	FindIndex( int nIndex) const								= 0;
		// get the 'nIndex'th element (may return NULL)
};

#endif // !defined(AFX_IDBLLIST_H__27BD74B5_D471_4C28_89E5_208ADCDD0F55__INCLUDED_)
