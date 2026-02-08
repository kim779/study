// IPropertyMap.h: interface for the IPropertyMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPROPERTYMAP_H__58869B5C_1FE9_4CC8_8A43_39F589E2505D__INCLUDED_)
#define AFX_IPROPERTYMAP_H__58869B5C_1FE9_4CC8_8A43_39F589E2505D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//-----------------------------------------------------------------------------
// Author		: Seung-Won, Bae	Date :2006/6/16
// Comments		: Interface for Property Map Object in OCX Wrapper
// Using way	: 
// See			: 
//-----------------------------------------------------------------------------

interface IPropertyMap  
{
protected:

public:
	virtual ~IPropertyMap()	{}
// Attributes
	// number of elements
	virtual int GetCount() const																		= 0;
	virtual BOOL IsEmpty() const																		= 0;

	// Lookup
	virtual BOOL Lookup(LPCTSTR key, const char *&rValue) const											= 0;
	virtual BOOL LookupKey(LPCTSTR key, LPCTSTR& rKey) const											= 0;

// Operations
	// add a new (key, value) pair
	virtual void SetAt(LPCTSTR key, LPCTSTR newValue)													= 0;

	// removing existing (key, ?) pair
	virtual BOOL RemoveKey(LPCTSTR key)																	= 0;
	virtual void RemoveAll()																			= 0;

	// iterating all (key, value) pairs
	virtual POSITION GetStartPosition() const															= 0;
	virtual void GetNextAssoc(POSITION& rNextPosition, const char *&rKey, const char *&rValue) const	= 0;
	// advanced features for derived classes
	virtual UINT GetHashTableSize() const																= 0;
	virtual void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE)									= 0;
};

#endif // !defined(AFX_IPROPERTYMAP_H__58869B5C_1FE9_4CC8_8A43_39F589E2505D__INCLUDED_)
