// LocalShareInfoList.cpp: implementation of the CLocalShareInfoList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KTBChart.h"
#include "LocalShareInfoList.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLocalShareInfoList::CLocalShareInfoList()
{

}

CLocalShareInfoList::~CLocalShareInfoList()
{
	// (2004.11.29, 배승원) 등록된 모든 Local Share Info를 제거한다.
	RemoveAllLocalShareInfo();
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) Local Share Info를 구성하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. Local Share Info을 추가하는 Interface를 제공한다.
//		특히, 등록 직후 Local Share Setting Flag을 확인할 수 있도록 Object Pointer로 Return한다.
// (2004.12.01, 배승원) 초기 등록시 공유값도 지정할 수 있도록 한다.
CLocalShareInfo *CLocalShareInfoList::AddLocalShareInfo( const char *p_szLocalShareName, int p_nLocalShareSettingCode, const char *p_szLocalSharedData)
{
	// 0. 이미 등록된 Local Share Info인지 확인한다.
	if( FindLocalShareInfo( p_szLocalShareName)) return NULL;

	// 1. 새로운 Local Share Info를 생성하여 등록한다.
	CLocalShareInfo *pLocalShareInfo = new CLocalShareInfo( p_szLocalShareName, p_nLocalShareSettingCode, p_szLocalSharedData);
	AddTail( pLocalShareInfo);
	return pLocalShareInfo;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 Local Share Info를 제거하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. 일괄 제거하는 Interface를 제공한다.
void CLocalShareInfoList::RemoveAllLocalShareInfo( void)
{
	while( !IsEmpty()) delete RemoveHead();
}

// 2. 지정되는 Local Share Info를 제거하는 Interface를 제공한다.
BOOL CLocalShareInfoList::RemoveLocalShareInfo( const char *p_szLocalShareName)
{
	POSITION posLocalShareInfo = FindLocalShareInfo( p_szLocalShareName);
	if( !posLocalShareInfo) return FALSE;
	delete GetAt( posLocalShareInfo);
	RemoveAt( posLocalShareInfo);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 구성된 Local Share Info를 검색하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. Local Share Info를 검색하는 Inteface를 제공한다.
POSITION CLocalShareInfoList::FindLocalShareInfo( const char *p_szLocalShareName)
{
	POSITION posPrevLSI;
	POSITION posLSI = GetHeadPosition();
	while( posLSI)
	{
		posPrevLSI = posLSI;
		CLocalShareInfo *pLSI = GetNext( posLSI);
		if( pLSI->m_strLocalShareName == p_szLocalShareName) return posPrevLSI;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) Local Shared Data를 설정 및 조회하는 Interface를 제공한다.
//////////////////////////////////////////////////////////////////////

// 1. Local Shared Data를 설정하는 Interface를 제공한다.
BOOL CLocalShareInfoList::SetLocalSharedData( const char *p_szLocalShareName, const char *p_szLocalSharedData)
{
	// 0. 등록되지 않은 Local Share Info인지 확인한다.
	POSITION posLSI = FindLocalShareInfo( p_szLocalShareName);
	if( !posLSI) return FALSE;

	// 1. Local Shared Data를 설정한다.
	GetAt( posLSI)->m_strLocalSharedData = p_szLocalSharedData;
	return TRUE;
}

// 2. Local Shared Data를 조회하는 Interface를 제공한다.
const char *CLocalShareInfoList::GetLocalSharedData( const char *p_szLocalShareName)
{
	// 0. 등록되지 않은 Local Share Info인지 확인한다.
	POSITION posLSI = FindLocalShareInfo( p_szLocalShareName);
	if( !posLSI) return NULL;

	// 1. Local Shared Data를 조회한다.
	CLocalShareInfo *pLocalShareInfo = GetAt( posLSI);
	return pLocalShareInfo->m_strLocalSharedData;
}
