// IWGraphInfoManager050116.cpp: implementation of the CIWGraphInfoManager050116 class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// *. 주의!	050116 Version의 AddIn에서 이용되는 OCX측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050116 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050116 Version Info
//		AddIn Module				Site			Manager
//			ChartCustomAddIn			GZone			배승원
//			ChartPatternAddIn			GZone			정서정
//			ChartGuideLine				GZone			남상윤
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IWGraphInfoManager050116.h"

#include "IndicatorList.h"				// for CIndicatorList
#include "GraphData.h"					// for CSubGraphData
#include "IndicatorInfo.h"				// for CIndicatorInfo

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIWGraphInfoManager050116::CIWGraphInfoManager050116( CIndicatorList *p_pIndicatorList)
{
	// (2004.10.11, 배승원) MainBlock Object의 Pointer를 관리한다.
	m_pIndicatorList = p_pIndicatorList;
	if( !m_pIndicatorList)
	{
		AfxMessageBox( "CIWGraphInfoManager050116 Null Wrapping Error!");
		return;
	}
}

CIWGraphInfoManager050116::~CIWGraphInfoManager050116()
{

}

// [04/10/14] Graph의 6색상을 조회하는 Interface를 제공한다.
BOOL CIWGraphInfoManager050116::GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6])
{
	// 1. Indicator Info를 구한다.
	CIndicatorInfo *pIndicatorInfo = NULL;
	if( p_bDefaultInfo) pIndicatorInfo = m_pIndicatorList->GetDefaultIndicatorInfo( p_szGraphName);
	else pIndicatorInfo = m_pIndicatorList->GetUserIndicatorInfo( p_szGraphName);
	if( !pIndicatorInfo) return FALSE;

	// 2. SubGraph를 구한다.
	CSubGraphData *pSubGraphData = pIndicatorInfo->GetSubGraphData( p_nSubIndex);
	if( !pSubGraphData) return FALSE;

	// 3. Graph Color를 구한다.
	CGraphColor *pGraphColor = pSubGraphData->GetColor_PointerType();
	if( !pGraphColor) return FALSE;
	p_aclrGraphColor6[ 0] = pGraphColor->GetColor1();
	p_aclrGraphColor6[ 1] = pGraphColor->GetColor2();
	p_aclrGraphColor6[ 2] = pGraphColor->GetColor3();
	p_aclrGraphColor6[ 3] = pGraphColor->GetColor4();
	p_aclrGraphColor6[ 4] = pGraphColor->GetColor5();
	p_aclrGraphColor6[ 5] = pGraphColor->GetColor6();
	return TRUE;
}