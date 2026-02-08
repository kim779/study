// PropertyMap.h: interface for the CPropertyMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTYMAP_H__7E457502_BA22_4B6E_B55F_6862963A8578__INCLUDED_)
#define AFX_PROPERTYMAP_H__7E457502_BA22_4B6E_B55F_6862963A8578__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/16
// Comments		: Property Map Object in OCX Wrapper
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

#include "IPropertyMap.h"		// for IPropertyMap

class CPropertyMap : public CMapStringToString, public IPropertyMap
{
// Attributes
	// number of elements
	virtual int GetCount() const										{	return CMapStringToString::GetCount();	}
	virtual BOOL IsEmpty() const										{	return CMapStringToString::IsEmpty();	}

	// Lookup
	virtual BOOL Lookup(LPCTSTR key, const char *&rValue) const
	{
		static CString strValue;
		BOOL bResult = CMapStringToString::Lookup( key, strValue);

		if( bResult == FALSE )
		{
			strValue = "";
		}

		rValue = strValue;
		return bResult;
	}
	virtual BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const			{	return CMapStringToString::LookupKey( key, rKey);	}

// Operations
	// add a new (key, value) pair
	virtual void SetAt(LPCTSTR key, LPCTSTR newValue)					{	CMapStringToString::SetAt( key, newValue);	}

	// removing existing (key, ?) pair
	virtual BOOL RemoveKey(LPCTSTR key)									{	return CMapStringToString::RemoveKey( key);	}
	virtual void RemoveAll()											{	CMapStringToString::RemoveAll();	}

	// iterating all (key, value) pairs
	virtual POSITION GetStartPosition() const							{	return CMapStringToString::GetStartPosition();	}
	virtual void GetNextAssoc(POSITION& rNextPosition, const char *&rKey, const char *&rValue) const
	{
		static CString strKey, strValue;
		CMapStringToString::GetNextAssoc( rNextPosition, strKey, strValue);
		rKey	= strKey;
		rValue	= strValue;
	}
	// advanced features for derived classes
	virtual UINT GetHashTableSize() const								{	return CMapStringToString::GetHashTableSize();	}
	virtual void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE)	{	CMapStringToString::InitHashTable( hashSize, bAllocNow);	}
};

#endif // !defined(AFX_PROPERTYMAP_H__7E457502_BA22_4B6E_B55F_6862963A8578__INCLUDED_)
