// IStringList.h: interface for the IStringList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISTRINGLIST_H__5BA95DE3_5439_41E9_8B72_6DB4FAEACFF5__INCLUDED_)
#define AFX_ISTRINGLIST_H__5BA95DE3_5439_41E9_8B72_6DB4FAEACFF5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>			// for CList
#include "../_IWUnknown.h"		// for CIWUnknown
#include "_AddInVer.h"			// for AddIn Version Define
#include "_IString.h"			// for ILPCSTR

interface IStringList : public IUnknown
{
protected:
	virtual ~IStringList()	{}

public:
// Attributes (head and tail)
	// count of elements
	virtual int		GetCount() const	= 0;
	virtual BOOL	IsEmpty() const		= 0;

	// peek at head or tail
	virtual BOOL	GetHead( ILPCSTR &p_pszData) const	= 0;
	virtual BOOL	GetTail( ILPCSTR &p_pszData) const	= 0;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	virtual BOOL	RemoveHead( ILPCSTR &p_pszData)	= 0;
	virtual BOOL	RemoveTail( ILPCSTR &p_pszData)	= 0;

	// add before head or after tail
	virtual POSITION	AddHead( const char *newElement)	= 0;
	virtual POSITION	AddTail( const char *newElement)	= 0;

	// remove all elements
	virtual void	RemoveAll()	= 0;

	// iteration
	virtual POSITION	GetHeadPosition() const				= 0;
	virtual POSITION	GetTailPosition() const				= 0;
	virtual BOOL	GetNext( POSITION& rPosition, ILPCSTR &p_pszData) const	= 0; // return *Position++
	virtual BOOL	GetPrev( POSITION& rPosition, ILPCSTR &p_pszData) const	= 0; // return *Position--

	// getting/modifying an element at a given position
	virtual BOOL	GetAt( POSITION position, ILPCSTR &p_pszData) const	= 0;
	virtual void	SetAt( POSITION pos, const char *newElement)		= 0;
	virtual void	RemoveAt( POSITION position)						= 0;

	// inserting before or after a given position
	virtual POSITION	InsertBefore( POSITION position, const char *newElement)	= 0;
	virtual POSITION	InsertAfter( POSITION position, const char *newElement)	= 0;

	// helper functions (note: O(n) speed)
	virtual POSITION	Find( const char *searchValue, POSITION startAfter = NULL) const	= 0;
		// defaults to starting at the HEAD, return NULL if not found
	virtual POSITION	FindIndex( int nIndex) const								= 0;
		// get the 'nIndex'th element (may return NULL)
};

#endif // !defined(AFX_ISTRINGLIST_H__5BA95DE3_5439_41E9_8B72_6DB4FAEACFF5__INCLUDED_)
