// CtrlFactory.h: interface for the CCtrlFactory class.
// ToolBar에 올릴 컨트롤들을 생성시키는 팩토리 클래스.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTRLFACTORY_H__9C549468_DCA5_40D7_81BE_70EFD148CE14__INCLUDED_)
#define AFX_CTRLFACTORY_H__9C549468_DCA5_40D7_81BE_70EFD148CE14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "ToolOption.h"

// (2006/2/8 - Seung-Won, Bae) Dynamic UI Control IDs
#define IDC_SCROLL                      17000
#define IDC_SLIDER                      17001
#define IDC_GRAPHSTYLE_COMBO            17002
#define IDC_LINETHICKNESS_COMBO         17003
#define IDC_RADAR                       17004

interface IChartCtrl;
class CScrollBarEx;
class CSliderCtrlEx;
class CButtonEx;
class CGraphStyleCombo;
class CGraphLineThicknessCombo;
class CRadarControl;
class CObjTool;

class CCtrlFactory  
{
public:
	CCtrlFactory( IChartCtrl *p_pIChartCtrl, CWnd* pParent, const CRect rectToolBar, const bool bStatusBar, const bool bRadarChart, const int nScrollWidth, const bool bBitmap);
	virtual ~CCtrlFactory();	// 주의! 각각의 컨트롤 소멸은 ToolBarDlg에서 해야함.
	void Initialize();	// 초기상태 세팅.

	CScrollBarEx* CreateScrollBar();	// 스크롤바 생성
	CSliderCtrlEx* CreateSliderCtrl();	// 슬라이더바 생성.
	CGraphStyleCombo* CreateGraphStyleCombo(const CString& strName);	// 그래프설정 콤보박스 생성
	CGraphLineThicknessCombo* CreatreGraphLineThicknessCombo(const CString& strName);	// 선굵기 설정 콤보 박스 생성
	CRadarControl* CreateRadarControl();	// 레이더차트 생성
	CButtonEx* CreateButton(CObjTool* pObjTool, const CString& strParam = _T(""));	// 버튼들을 생성.
	
private:
	CRect m_rectCtrl;		// 컨트롤 생성에 사용할 멤버.
	bool m_bStatusBar;		// 수치조회데이타보기(StatusBar) 사용여부
	bool m_bRadarChart;		// 레이다차트(펼침형) 사용여부.
	bool m_bBitmap;			// 비트맵 로딩에 성공했는지 여부. (버튼을 OWNER_DRAW로 그릴것인지 판단하기 위함)
	CWnd* m_pParent;		// 부모윈도우포인터.
	UINT	m_nCtrlID;		// 컨트롤 아이디.
	UINT	m_nToolBarWidth;// 툴바의 Width.(Status Bar, Radar Chart 용)
	int		m_nScrollWidth;	// Scroll Bar의 Width.
	HWND	m_hOcxWnd;

protected:
	IChartCtrl *	m_pIChartCtrl;
};

#endif // !defined(AFX_CTRLFACTORY_H__9C549468_DCA5_40D7_81BE_70EFD148CE14__INCLUDED_)
