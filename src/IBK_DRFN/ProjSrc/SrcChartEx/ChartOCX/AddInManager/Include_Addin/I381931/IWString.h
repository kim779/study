// IWString.h: interface for the CIWString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWSTRING_H__C4A76FE6_58E1_4667_A5E1_4E331D3FB680__INCLUDED_)
#define AFX_IWSTRING_H__C4A76FE6_58E1_4667_A5E1_4E331D3FB680__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../_IWUnknown.h"
#include "_IString.h"
#include "_IChartOCX.h"				// for IChartOCX

class CIWString : public CIWUnknown, public IString, public CString
{
// (2004.10.07, 배승원) Interface Wrapper에서 IUnknown의 Interface를 손쉽게 지원할 수 있도록 Macro를 정의한다.
protected:
	_IWRAPPER_IUNKNOWN_MANAGER_REFERENCE( this)
	_IWRAPPER_IUNKNOWN_MANAGER_STRUCTOR( CIWString)
	_IWRAPPER_IUNKNOWN_MANAGER_MGR( IString, CIWString)
protected:
	BOOL	m_bAutoDelete;
	void SetTarget( const char *p_pTarget)	{	*( CString *)this = p_pTarget;	}
public:
	static CIWString *CreateObject( const char *p_pTarget)
	{
		CIWString *pIWrapper = NULL;
		if( m_lReleaseMgr.IsEmpty())	pIWrapper = new CIWString;
		else							pIWrapper = ( CIWString *)m_lReleaseMgr.RemoveHead();
		pIWrapper->AddRef();
		if( p_pTarget) pIWrapper->SetTarget( p_pTarget);
		return pIWrapper;
	}

protected:
	virtual const char *GetData( void)	{	return GetBuffer();/* m_pchData; */ }
};

#endif // !defined(AFX_IWSTRING_H__C4A76FE6_58E1_4667_A5E1_4E331D3FB680__INCLUDED_)
