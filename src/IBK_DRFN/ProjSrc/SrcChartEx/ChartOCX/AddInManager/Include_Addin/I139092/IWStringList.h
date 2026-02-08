// IWStringList.h: interface for the CIWStringList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWSTRINGLIST_H__76A4E031_5A53_480F_AC61_FBEADD1CBB7B__INCLUDED_)
#define AFX_IWSTRINGLIST_H__76A4E031_5A53_480F_AC61_FBEADD1CBB7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>				// for CList
#include "../_IWUnknown.h"
#include "_IStringList.h"
#include "_IChartOCX.h"				// for IChartOCX

class CIWStringList : public CIWUnknown, public IStringList
{
public:
typedef CList<CString, CString> CStringTList;
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IStringList, CIWStringList, CStringTList, m_plStringList)

protected:
// Attributes (head and tail)
	// count of elements
	virtual int		GetCount() const;
	virtual BOOL	IsEmpty() const;

	// peek at head or tail
	virtual BOOL	GetHead( ILPCSTR &p_pszData) const;
	virtual BOOL	GetTail( ILPCSTR &p_pszData) const;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	virtual BOOL	RemoveHead( ILPCSTR &p_pszData);
	virtual BOOL	RemoveTail( ILPCSTR &p_pszData);

	// add before head or after tail
	virtual POSITION	AddHead( const char *newElement);
	virtual POSITION	AddTail( const char *newElement);

	// remove all elements
	virtual void	RemoveAll();

	// iteration
	virtual POSITION	GetHeadPosition() const;
	virtual POSITION	GetTailPosition() const;
	virtual BOOL	GetNext( POSITION& rPosition, ILPCSTR &p_pszData) const; // return *Position++
	virtual BOOL	GetPrev( POSITION& rPosition, ILPCSTR &p_pszData) const; // return *Position--

	// getting/modifying an element at a given position
	virtual BOOL	GetAt( POSITION position, ILPCSTR &p_pszData) const;
	virtual void	SetAt( POSITION pos, const char *newElement);
	virtual void	RemoveAt( POSITION position);

	// inserting before or after a given position
	virtual POSITION	InsertBefore( POSITION position, const char *newElement);
	virtual POSITION	InsertAfter( POSITION position, const char *newElement);

	// helper functions (note: O(n) speed)
	virtual POSITION	Find( const char *searchValue, POSITION startAfter = NULL) const;
		// defaults to starting at the HEAD, return NULL if not found
	virtual POSITION	FindIndex( int nIndex) const;
		// get the 'nIndex'th element (may return NULL)
};

#endif // !defined(AFX_IWSTRINGLIST_H__76A4E031_5A53_480F_AC61_FBEADD1CBB7B__INCLUDED_)
