#if !defined(AFX_TOOLBARDLG_H__C5096DEC_1C31_4AF0_AC80_7DCE5DA597B3__INCLUDED_)
#define AFX_TOOLBARDLG_H__C5096DEC_1C31_4AF0_AC80_7DCE5DA597B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarDlg.h : header file
//

#include "Resource.h"
#include "BlockBaseEnum.h"
#include "BlockBaseData.h"
#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////////
// CToolBarDlg dialog

class CToolOption;
class CScrollBarEx;
class CSliderCtrlEx;
class CGraphStyleCombo;
class CGraphLineThicknessCombo;
class CToolBarManagerImplementation;
class CRadarControl;
class CPacketList;
class CPacketListManager;
class CXScaleManager;
class CButtonEx;
class CButtonProperty;
interface IChartCtrl;

typedef CList<CWnd*, CWnd*> CWndList;	// 툴바에 등록된 컨트롤들을 담는 리스트타입 정의.
typedef CList<CButtonEx*, CButtonEx*> CButtonList;	// 툴바에 등록된 버튼들을 담는 리스트 타입 정의.

class CToolBarDlg : public CDialog
{
// Construction
public:
	CToolBarDlg(CWnd* pParent, CToolBarManagerImplementation* pToolBarImpl, IChartCtrl *p_pIChartCtrl);   // standard constructor
	virtual ~CToolBarDlg();
// Dialog Data
	//{{AFX_DATA(CToolBarDlg)
	enum { IDD = IDD_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CToolBarDlg)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Function
private:
	bool MakeBitMap(const CString& strFileName);
	void ToggleBT(const CToolOptionInfo::TOOLOPTION eToolOption, const bool bState);

public:
	bool CreateCtrl(CToolOption* pToolOption, const CFont* pFont, const CString& strImageFile, const int nScrollWidth, const bool bRadarChart, const bool bStatusBar);
	void DeleteCtrl();
	void MoveToolBarDlg( void);

	// OCX에 값을 세팅하는 함수들.
	void SetStartEndIndexToOcx(const int nStartIndex, const int nEndIndex);
	void SetToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton = NULL, long nParam = 0);
	void ReleaseToolFunction(CToolOptionInfo::TOOLOPTION enumToolOption, CButtonEx* pButton = NULL, long nParam = 0);
	void SetStateWholeView(const int nState);	

	// 각종 툴기능 수행
	void ExecuteZoomInZoomOut(CButtonEx* pButton);	// Zoom In/Out 실행
	void ExecuteRadar();							// Radar Chart 실행
	void ExecuteGraphColorSetting();				// Graph Color설정창 실행


	// OCX에서 값을 가져오는 함수들.
	int GetCountMinPerPage();
	bool IsWholeView();

	// ToolBar에 값을 세팅하는 함수들.
	void SetControlData(CPacketList* pPacketList, CPacketListManager* pPacketListManager, CXScaleManager* pxScaleManager);
	void SetScrollData_TR(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage, CPacketDataInfo::DATAINSERTTYPE dataType, const int nGlanceBalanceMargin);
	void SetScrollData_Real(const int nTotalDataCount, const int nPageDataCount, const bool bFullPage);
	void SetScrollData_RemovePacket(const int nTotalDataCount, const bool bFullPage, const int nRemoveDataCount);
	void SetSliderPos(int nPageDataCount);
	void SetSliderMin(const int nSliderMin);
	void SetGraphType(CGraphBaseData::GRAPHTYPE graphType, int nIndex);
	void SetLineThickness(int nThickness);
	void SetGraphColor(COLORREF clrGraphColor);
	void SetOneChart( bool bOneChart, bool p_bIsNoZoomChart);

	// ToolBar에서 값을 뽑아내는 함수들.
	int GetDataCount();
	bool GetStartEndIndex(int& nStartIndex, int& nEndIndex);
	CPacketList* GetPacketList();
	CPacketListManager* GetPacketListManager();
	CXScaleManager* GetXScaleManager();
	CFont* GetResourceFont();
	bool GetOneChart();
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	bool	IsNoneZoomChart( void)		{	return m_bIsNoZoomChart;	}
	int GetMaxRange();
	CButtonEx* GetButtonEx(CToolOptionInfo::TOOLOPTION enumToolOption);

	void ReDraw();
	bool IsScrollBar();
	void ReleaseOtherButtonUI(CToolOptionInfo::TOOLOPTION enumToolOption, const CString& strParam = _T(""));

	int GetPageCountNextStepToOcx(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin = -1, int nMax = -1);
	BOOL GetReverseToOcx();

	// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
	void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount, BOOL p_bWholeView);

// Field
public:
	static CBitmap m_bitmapButton;	// Button클래스에서 Draw할때 필요로 하는 넘이니 직접 엑세스하도록 Public권한을 줌.
	CPacketListManager* m_pPacketListManager;
	CXScaleManager* m_pxScaleManager;
	
private:
	CSliderCtrlEx* m_pSliderCtrl;				// Slider Bar 컨트롤
	CScrollBarEx* m_pScrollBar;					// Scroll Bar 컨트롤
	CGraphStyleCombo* m_pGraphStyleCombo;		// 그래프스타일 콤보박스 컨트롤
	CGraphLineThicknessCombo* m_pGraphLineCombo;// 선굵기 콤보박스 컨트롤
	CRadarControl* m_pRadarControl;				// 레이다차트 컨트롤
	COLORREF m_clrSelection;					// 색 설정값.
	int m_nSliderMin;							// Min Range
	int m_nSliderMax;							// Max Range
	int m_nRemainCount;							// 여백을 빼고도 남은 데이타카운트(확대/축소시)
	int m_nGlanceBalanceMargin;					// 일목균형표가 쓸 여백.
	int m_nScrollWidth;							// Scroll Bar의 너비.
	int m_nScrollingDataCount;					// 자동스크롤할 데이타 갯수.
	bool m_bCheckGlanceBalance;					// 일목균형표 사용 유무
	bool m_bRadarChart;
	bool m_bStatusBar;							// '수치조회데이타보기'의 Status Bar 사용 유무
	bool m_bOneChart;							// 단일차트 사용 유무
	bool m_bIsNoZoomChart;						// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	BOOL m_bUseRadarChart;						// Radar Chart 사용 유무
	CToolBarManagerImplementation* m_pToolBarManagerImple;
	CWndList m_listToolBarCtrl;					// 툴바Dlg에 등록된 컨트롤을 담아둘 List.
	CButtonList m_listToolButton;				// Dlg에 등록된 툴버튼 리스트.
	CButtonProperty* m_pButtonProperty;			// 툴버튼 속성에 따라 State 상태를 설정하는 오브젝트
	CPacketList* m_pPacketList;
	// (2006/11/18 - Seung-Won, Bae) Support Chart OCX Ctrl Interface
	IChartCtrl *	m_pIChartCtrl;

	// (2007/2/8 - Seung-Won, Bae) Disable WholeView Btn in Special Chart.
	CButtonEx *	m_pWholeViewBtn;
	// (2007/2/8 - Seung-Won, Bae) Disable Zoom Btn in Special Chart.
	CButtonEx *	m_pZoomInBtn;
	CButtonEx *	m_pZoomOutBtn;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARDLG_H__C5096DEC_1C31_4AF0_AC80_7DCE5DA597B3__INCLUDED_)
