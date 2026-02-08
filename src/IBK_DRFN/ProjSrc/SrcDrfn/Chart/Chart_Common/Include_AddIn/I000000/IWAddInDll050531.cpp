// IWAddInDll050531.cpp: implementation of the CIWAddInDll050531 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 OCX에서 이용되는 AddIn측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//			ChartPatternAddIn			GZone			정서정
//			ChartGuideLine				GZone			남상윤
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWAddInDll050531.h"

#include "IChartAddIn050531.h"	// for IChartAddIn050531

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// [04/10/07, 배승원] Addin DLL의 Module Name과 Handle을 관리하여 DLL Interface를 호출할 수 있도록 한다.
CIWAddInDll050531::CIWAddInDll050531( const char *p_szAddInModule, HMODULE p_hAddInDll)
{
	m_hAddInDll		= p_hAddInDll;
	m_strModuleName	= p_szAddInModule;
	if( !m_hAddInDll)
	{
		AfxMessageBox( "CIWAddInDll050531 Null Wrapping Error!");
		return;
	}

	// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOcx Interface Gateway)를 전달하여
	//		DLL의 AddIn Interface를 확보한다.
	m_pIChartAddIn = NULL;
}
CIWAddInDll050531::~CIWAddInDll050531()
{
	// (2004.10.08, 배승원) AddIn Object가 성공적으로 등록된 경우, AddIn Object를 Release한다.
	if( m_pIChartAddIn)
	{
		m_pIChartAddIn->Release();
		m_pIChartAddIn = NULL;
	}

	// (2004.10.07, 배승원) 등록된 AddIn DLL을 Free시킨다.
	FreeLibrary( m_hAddInDll);
}

// (2004.10.07, 배승원) AddIn Interface Wrapper Selector Routine을 제공한다.
//		AddIn Manager의 LoadAddIn()에서 Loading된 DLL의 Addin Version에 맞는 Wrapper를 생성하는 Routine이다.
//		특히 Addin Manager는 항상 최종 Version의 Selector Routine을 호출하여 적절한 Wrapper Object를 구할 수 있도록 한다.
IAddInDll *CIWAddInDll050531::CreateAddInDLLIWrapper( const char *p_szAddInModule, HMODULE p_hAddInDll, int p_nAddInIVersion)
{
	// 0. 먼저 Addin DLL의 Handle을 확인한다.
	if( !p_hAddInDll) return NULL;

	IAddInDll *pIAddInDll = NULL;
	switch( p_nAddInIVersion)
	{
		case CIWAddInDll050531::IVERSION:	pIAddInDll = new CIWAddInDll050531( p_szAddInModule, p_hAddInDll);
											break;
	}

	if( pIAddInDll) pIAddInDll->AddRef();

	return pIAddInDll;
}


//[04/10/07] AddIn DLL을 초기화시키는 Interface를 제공한다.
// (2004.10.08, 배승원) 초기화시에 IAddInManager(ChartOcx Interface Gateway)를 전달하여
//		DLL의 AddIn Interface를 확보한다.
typedef IChartAddIn050531 *( *FN_InitAddInDll)( IAddInManager *p_pIAddInManager);	
BOOL CIWAddInDll050531::InitAddInDll( IAddInManager *p_pIAddInManager)
{
	// 0. InitAddInDll Interface의 Process Address를 확인한다.
	FN_InitAddInDll fnInitAddInDll;
	fnInitAddInDll = ( FN_InitAddInDll)GetProcAddress( m_hAddInDll, "InitAddInDll");
	if( !fnInitAddInDll) return FALSE;

	m_pIChartAddIn = ( *fnInitAddInDll)( p_pIAddInManager);
	return ( m_pIChartAddIn != NULL);
}

// [04/10/11] AddIn Object의 Interface를 제공하는 Interface를 제공한다.
IUnknown *CIWAddInDll050531::GetIChartAddIn( void)
{
	m_pIChartAddIn->AddRef();
	return m_pIChartAddIn;
}
