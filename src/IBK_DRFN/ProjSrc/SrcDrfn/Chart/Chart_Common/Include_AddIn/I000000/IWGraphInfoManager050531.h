// IWGraphInfoManager050531.h: interface for the CIWGraphInfoManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IWGRAPHINFOMANAGER050531_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
#define AFX_IWGRAPHINFOMANAGER050531_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 AddIn에서 이용되는 OCX측 Interface의 Wrapper로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//		ChartAssistIndexAddIn		한화			오근문
//////////////////////////////////////////////////////////////////////

// (2004.10.13, 배승원) IndicatorList의 Interface Wrapper Class를 정의한다.

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../IWUnknown.h"
#include "IGraphInfoManager050531.h"

class CIndicatorList;
class CIWGraphInfoManager050531 : public CIWUnknown, public IGraphInfoManager050531
{
public:
	CIWGraphInfoManager050531( CIndicatorList *p_pIndicatorList);
	virtual ~CIWGraphInfoManager050531();

// (2004.10.07, 배승원) Interface Wrapper의 기본 Interface (IUnknown용)를 제공한다.
protected:
	_IWRAPPER_IUNKNOWN_INTERFACE()

// (2004.10.13, 배승원) IndicatorList Object의 Pointer를 관리한다.
protected:
	CIndicatorList *	m_pIndicatorList;

// [04/10/14] Graph Info를 조회하는 Interface를 제공한다.
public:
	// [04/10/14] Graph의 6색상을 조회하는 Interface를 제공한다.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]);
	
	// 종찬(05/07/14) 지표 설정값을 제공한다.
	virtual CList<double, double>* GetCommonConditionList(const char* pcszGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const char* pcszSubGraphName);
	virtual CList<double, double>* GetConditionList(const char* pcszGraphName, const int nSubGraphIndex);
	
// [05/07/15] Getting Graph Info Interface
public:
	virtual IGraphInfo050531 *		GetGraphInfo( const char *p_szGraphName);
};

#endif // !defined(AFX_IWGRAPHINFOMANAGER050531_H__660913E1_B441_447C_8EFD_4D60FF54619C__INCLUDED_)
