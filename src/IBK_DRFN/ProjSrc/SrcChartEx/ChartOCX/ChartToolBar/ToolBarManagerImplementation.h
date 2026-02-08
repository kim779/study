// ToolBarManagerImplementation.h: interface for the CToolBarManagerImplementation class.
// 실질적인 툴바를 제어하는 클래스로 각각의 컨트롤들이 중앙집중식으로 이 클래스에 멤버로
// 되어있다.
// 본 클래스에서는 두개의 Dialog만 관리한다. 미니툴바Dlg와 수치조회Dlg.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOOLBARMANAGERIMPLEMENTATION_H__523BA067_CD45_4CD0_B61B_7F1A6291D3DF__INCLUDED_)
#define AFX_TOOLBARMANAGERIMPLEMENTATION_H__523BA067_CD45_4CD0_B61B_7F1A6291D3DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/BlockBaseEnum.h"
#include "../Include_Chart/BlockBaseData.h"

interface IChartCtrl;
class CMainBlock;
class CPacketList;
class CPacketListManager;
class CXScaleManager;
class CToolOption;
class CToolBarDlg;
class CRowViewData;
class CIndicatorList;
class COutsideCommonInterface;
class CNumericalInquiryDlg;
class CToolBarManagerImplementation  
{
// Function
public:
	CToolBarManagerImplementation( IChartCtrl *p_pIChartCtrl);
	virtual ~CToolBarManagerImplementation();

// Field
private:	
	CToolBarDlg* m_pToolBarDlg;						// 미니툴바 다이얼로그.

	CToolOption *		m_pToolOption;
	IChartCtrl *		m_pIChartCtrl;	// OCX의 인터페이스
	CPacketList *		m_pPacketList;
	CPacketListManager* m_pPacketListManager;
	CXScaleManager*		m_pxScaleManager;
	CMainBlock *		m_pMainBlock;
	CIndicatorList *	m_pIndicatorList;
	CRect m_rectToolBarDlg;					// ToolBar DLL의 실질적 영역.
	int m_nMaxRange;						// MaxRange
	int m_nDataStartIndex;					// 시작 인덱스
	int m_nDataEndIndex;					// 끝 인덱스

private:
	bool FilterSimpleInquiryData(const CString& strPacketNames);	// 간략한 조회데이타만 추출하기 위한 함수로 불필요한 조회데이타는 제거하기 위함.

private:
	const CRect CalculateToolBarRect(int& nScrollBarWidth, bool& bRadarChart, bool& bStatusBar);	// 툴바영역 계산.(생성시)
	bool		CreateToolBarDialog();	// 툴바 Dialog 생성.
	bool		CreateToolBarControl(const CString& strImageFile, const int nScrollBarWidth, const bool bRadarChart, const bool bStatusBar);	// 툴바의 각각Ctrl들 생성.

public:
	//-----------------------------툴바.
	bool CreateToolBar(const CString& strToolBarOption, const CString& strImageFile);	// 툴바 생성.
	void ToolBarPosChanged(const CRect rectToolBarPos);	// 툴바 위치 잡기.
	
	void SetSliderMin(const int nSliderMin);	// Slider의 Min Range설정하기.
	void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager, CMainBlock* pMainBlock, CIndicatorList* pIndicatorList);	// 기본 설정데이타 세팅.
	void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);	// Graph형태 설정.
	void SetLineThickness(int nThickness);	// 선굵기 설정
	void SetGraphColor(COLORREF clrGraphColor);	// 색깔 설정.
	void SetOneChart(bool bOneChart, bool p_bIsNoZoomChart);	// 원차트 설정.
	void SetStartEndIndexToOcx(const int nStart, const int nEnd);	// Start,End Index를 OCX에 보내기.

	const CRect			GetToolBarRect();	// 툴바의 영역을 리턴.
	void				SetToolBarRect( const CRect p_rctToolBar)	{	m_rectToolBarDlg = p_rctToolBar;	}	// (2006/4/14 - Seung-Won, Bae) Reset New ToolBar Rect by CToolBarDlg
	bool				GetStartEndIndex(int& nStartIndex, int& nEndIndex);	// Data의 Start, End Index 리턴.
	int					GetMaxRange() const;	// SliderBar의 Max Range 리턴.
	HWND				GetToolBarHwnd();	// ToolBar Dialog의 윈도우핸들을 리턴.
	
	void ReDraw();				//툴바를 그린다.
	bool IsSpecialIndicator();	// 현재 특수지표를 사용중인지를 리턴.
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	bool IsRunningNoZoomChart();
	bool IsToolScrollBar();	// 스크롤바를 사용하는지 여부를 리턴.
	void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption);	// 눌려진 버튼 해제.
	BOOL UseToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	// 버튼 기능 수행.
	BOOL GetToolOption(CToolOptionInfo::TOOLOPTION enumToolOption);	// 버튼 기능 수행 해제.
	
	//--------------------------------------------------수치조회창.
public:
	void SetNumericalInquiryDlgMoveState(const short nMove);	// 이동/고정/마우스따라다니기 설정.
	void OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption);	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
	CString GetRealPacketNames(const CString& strTitle, const CString& strPacketNames) const;				// 1틱일경우 패킷명변경.

	// (2006/11/15 - Seung-Won, Bae) Tool On/Off State Changed.
	BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);

// (2006/11/18 - Seung-Won, Bae) Notify the User Zoom State to MultiChart
public:
	COutsideCommonInterface *GetMChartInterface( void)	{	return NULL;	}
	int GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin = -1, int nMax = -1);
	BOOL GetReverseToOcx();

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
public:
	void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView);
};

#endif // !defined(AFX_TOOLBARMANAGERIMPLEMENTATION_H__523BA067_CD45_4CD0_B61B_7F1A6291D3DF__INCLUDED_)
