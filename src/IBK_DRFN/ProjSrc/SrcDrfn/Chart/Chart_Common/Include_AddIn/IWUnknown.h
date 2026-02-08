// IWUnknown.h: interface for the CIWUnknown class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_)
#define AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	전 AddIn과 OCX에서 이용되는 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 Version별 AddIn 관련 Module이 함께 수정되어 내려가는 것이 전제되어야 합니다.
//////////////////////////////////////////////////////////////////////

// (2004.10.07, 배승원) Interface Wrapper Class에 기본적으로 IUnknown Interface의 Routine이
//		지원될 수 있도록 제공하는 Base Wrapper Class이다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// (2004.10.07, 배승원) Interface Wrapper의 Version이 수치로 8자리로 기입이 가능하도록
//		8진수를 10진수로 자동 변환하는 Macro를 정의한다.
//		(기본적으로 050116이면 8진수로 인정되는데, 이를 10진수 수치로 변환하는 Macro를 정의한다.)
#define _IWRAPPER_DECIMAL( OCTAL)	( 1OCTAL - 1000000)

class CIWUnknown : public CObject  
{
public:
	CIWUnknown();
protected:
	virtual ~CIWUnknown();

// (2004.10.07, 배승원) AddRef()와 Release()를 제공한다. (QueryInterface()는 NULL로 등록해 놓는다.)
protected:
	ULONG	m_ulReference;
protected:
	virtual ULONG	STDMETHODCALLTYPE AddRef(	void);
	virtual ULONG	STDMETHODCALLTYPE Release(	void);
	virtual HRESULT	STDMETHODCALLTYPE QueryInterface( REFIID iid, void ** ppvObject)	{	return E_NOINTERFACE;	}
};

// (2004.10.07, 배승원) Interface Wrapper에서 IUnknown의 Interface를 손쉽게 지원할 수 있도록 Macro를 정의한다.
#define _IWRAPPER_IUNKNOWN_INTERFACE()															\
	virtual ULONG	STDMETHODCALLTYPE AddRef(	void)	{	return CIWUnknown::AddRef();	}	\
	virtual ULONG	STDMETHODCALLTYPE Release(	void)	{	return CIWUnknown::Release();	}	\
	virtual HRESULT	STDMETHODCALLTYPE QueryInterface( REFIID iid, void ** ppvObject)	{	return E_NOINTERFACE;	}

#endif // !defined(AFX_IWUNKNOWN_H__5D3DDC49_7908_4003_B8CD_BEE238AE3B1A__INCLUDED_)
