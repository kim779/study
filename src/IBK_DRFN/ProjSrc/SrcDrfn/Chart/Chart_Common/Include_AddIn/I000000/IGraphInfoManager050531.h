// IGraphInfoManager050531.h: interface for the IGraphInfoManager050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFOMANAGER050531_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
#define AFX_IGRAPHINFOMANAGER050531_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_

//////////////////////////////////////////////////////////////////////
// *. 주의!	050531 Version의 AddIn에서 이용되는 OCX측 Interface로 기능 추가만 가능합니다.
//		기능 삭제와 Type 변경은 허용하지 않습니다.
//		Interface 순서 변경 조차 허용되지 않습니다.
//		허용되지 않는 편집은 050531 Version의 모든 AddIn Module과 관련 Site OCX가
//			함께 수정되어 내려가는 것이 전제되어야 합니다.
// *. 050531 Version Info
//		AddIn Module				Site			Manager
//		ChartCustomAddIn			한화			배승원
//		ChartAssistIndexAddIn		한화			오근문
//////////////////////////////////////////////////////////////////////

// (2004.10.13, 배승원) IndicatorList 의 Object Interface를 정의한다.
//		AddIn DLL 참조 Interface로 Macro Interface를 정의한다.
#if IGraphInfoManagerVer < 050531
	#define	IGraphInfoManager		IGraphInfoManager050531
	#undef	IGraphInfoManagerVer
	#define	IGraphInfoManagerVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IGraphInfo050531.h"
#include <AFXTEMPL.H>

interface IGraphInfoManager050531 : public IUnknown  
{
protected:
	virtual ~IGraphInfoManager050531()	{};

// [04/10/14] Graph Info를 조회하는 Interface를 제공한다.
public:
	// [04/10/14] Graph의 6색상을 조회하는 Interface를 제공한다.
	virtual BOOL	GetGraph6Color( BOOL p_bDefaultInfo, const char *p_szGraphName, int p_nSubIndex, COLORREF p_aclrGraphColor6[ 6]) = 0;
	
// [05/07/15] Getting Graph Info Interface
public:
	virtual IGraphInfo *		GetGraphInfo( const char *p_szGraphName) = 0;
};

#endif // !defined(AFX_IGRAPHINFOMANAGER050531_H__6318E91D_7061_4B34_9B11_E08E5DAEA36A__INCLUDED_)
