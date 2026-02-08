// IHelpMessageMap.h: interface for the IHelpMessageMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IHELPMESSAGEMAP_H__BAE00709_57E3_49E9_BC4F_D55C8F50BFC4__INCLUDED_)
#define AFX_IHELPMESSAGEMAP_H__BAE00709_57E3_49E9_BC4F_D55C8F50BFC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "_AddInVer.h"				// for AddIn Version Define
#include "_IString.h"				// for ILPCSTR

interface IHelpMessageMap : public IUnknown
{
protected:
	virtual ~IHelpMessageMap()	{}

public:
// Attributes
	// number of elements
	virtual int GetCount() const																		= 0;
	virtual BOOL IsEmpty() const																		= 0;

	// Lookup
	virtual BOOL Lookup(	LPCTSTR key, ILPCSTR &p_pszValue)	const									= 0;
	virtual BOOL LookupKey(	LPCTSTR key, ILPCSTR &p_pszKey)		const									= 0;

// Operations
	// iterating all (key, value) pairs
	virtual POSITION GetStartPosition() const															= 0;
	virtual void GetNextAssoc(POSITION& rNextPosition, ILPCSTR &p_pszKey, ILPCSTR &p_pszValue) const	= 0;
	// advanced features for derived classes
	virtual UINT GetHashTableSize() const																= 0;
};

#endif // !defined(AFX_IHELPMESSAGEMAP_H__BAE00709_57E3_49E9_BC4F_D55C8F50BFC4__INCLUDED_)
