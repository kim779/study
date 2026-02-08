// IWHelpMessageMap.h: interface for the CIWHelpMessageMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWHELPMESSAGEMAP_H__CD21DF08_10B1_4994_A9D3_AE27CFD77167__INCLUDED_)
#define AFX_IWHELPMESSAGEMAP_H__CD21DF08_10B1_4994_A9D3_AE27CFD77167__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"
#include "_IHelpMessageMap.h"
#include "_IChartOCX.h"										// for IChartOCX

class CIWHelpMessageMap : public CIWUnknown, public IHelpMessageMap
{
// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER( IHelpMessageMap, CIWHelpMessageMap, const CMapStringToString, m_pHelpMessageMap)

public:
// Attributes
	// number of elements
	virtual int GetCount() const;
	virtual BOOL IsEmpty() const;

	// Lookup
	virtual BOOL Lookup(	LPCTSTR key, ILPCSTR &p_pszValue)	const;
	virtual BOOL LookupKey(	LPCTSTR key, ILPCSTR &p_pszKey)		const;

// Operations
	// iterating all (key, value) pairs
	virtual POSITION GetStartPosition() const;
	virtual void GetNextAssoc(POSITION& rNextPosition, ILPCSTR &p_pszKey, ILPCSTR &p_pszValue) const;
	// advanced features for derived classes
	virtual UINT GetHashTableSize() const;
};

#endif // !defined(AFX_IWHELPMESSAGEMAP_H__CD21DF08_10B1_4994_A9D3_AE27CFD77167__INCLUDED_)
