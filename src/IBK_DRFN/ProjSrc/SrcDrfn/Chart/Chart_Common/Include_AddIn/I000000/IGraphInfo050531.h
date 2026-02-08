// IGraphInfo050531.h: interface for the IGraphInfo050531 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IGRAPHINFO050531_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
#define AFX_IGRAPHINFO050531_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_

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
#if IGraphInfoVer < 050531
	#define	IGraphInfo		IGraphInfo050531
	#undef	IGraphInfoVer
	#define	IGraphInfoVer	050531
#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <AFXTEMPL.H>

interface IGraphInfo050531 : public IUnknown
{
protected:
	virtual ~IGraphInfo050531() {}

		
public:
	// 종찬(05/07/14) 지표 계산에 필요한 조건값을 제공한다.
	virtual CList<double, double>* GetCommonCalculateVarList() = 0;
	virtual CList<double, double>* GetCalculateVarList(const char* pcszSubGraphName) = 0;
	virtual CList<double, double>* GetCalculateVarList(const int nSubGraphIndex) = 0;

	// 종찬(05/07/15) 계산에 쓰이는 패킷명을 제공한다.
	virtual LPCTSTR GetCommonCalculatePacketNames() = 0;
	virtual LPCTSTR GetCalculatePacketNames(const int nSubGraphIndex) = 0;

	// 서브그래프의 인덱스를 리턴한다.
	virtual int GetSubGraphIndex(const char* pcszSubGraphName) = 0;
	
	// 서브그래프의 Show/Hide 여부를 리턴한다. (bSubGraphShow - true: show, false: hide)
	virtual bool IsShowSubGraph(const char* pcszSubGraphName, bool& bSubGraphShow) = 0;
	virtual bool IsShowSubGraph(const int nSubGraphIndex, bool& bSubGraphShow) = 0;
};

#endif // !defined(AFX_IGRAPHINFO050531_H__C3090292_6F2E_46A2_85B1_D47E41B10577__INCLUDED_)
