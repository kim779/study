#if !defined(AFX_DLGTOOLBAR_H__F80CD3CD_73BD_46D1_B3DD_35C2D8037915__INCLUDED_)
#define AFX_DLGTOOLBAR_H__F80CD3CD_73BD_46D1_B3DD_35C2D8037915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgToolbar.h : header file
//

#include "../chart_common/BlockBaseEnum.h"
#include "RscDlg.h"
#include "../chart_common/ILButton.h"


#define NUM_BUTTONS 80
/////////////////////////////////////////////////////////////////////////////
// CDlgToolbar dialog
class CChartToolBtn;
class CFileBitmap;
class CLineThicknessSelDlg;
class CDlgToolbar : public CRscDlg
{
// Construction
public:
	CDlgToolbar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgToolbar();

	CChartToolBtn* m_pToolBtn[NUM_BUTTONS];
	virtual void ResizeControl(int cx, int cy);
	void SelectToolSelection();
	void SetChartMng(CWnd* pWnd) {m_pChartMng = pWnd;};
	void ResetAllButton();
	//2007.01.17 add by LYH  툴바버튼 동기화
	void SetToggleButton(int nSelToolNo, BOOL bSelect);
	BOOL GetIndexArray(int nSelTool, CArray<int, int>& arIndx, CArray<int, int>& arCheck);
	BOOL GetToolNameMap(CMap<int, int, CString, CString>& mapToolName);
	CToolOptionInfo::TOOLOPTION GetToolOption(UINT nId);
	void SetToggleBtnFromOutside(UINT nID);

	IGateManager*	m_pGateManager;

protected:
	BOOL CreateToolButton();
	BOOL CreateImageList();
	CString GetChartToolDescription(CString strToolName);

	// Gdi objects
	HBRUSH			m_hWhiteBrush;		// brush for painting editbox

	CFont			m_BtnFont;
	CToolTipCtrl	m_ToolTip;
	CWnd*			m_pCurToggleBtn;
	BOOL			m_bCheckShowData;
	CWnd*			m_pMultiChartWnd;
	CWnd*			m_pChartMng;
	int				m_nCursorIndex;
	//2006.01.07 by LYH
	BOOL			m_bShowScale;
	int				m_nYUnitType;
	BOOL			m_bShowInnerToolbar;

	CImageList*   m_pIL_W28H22; //폭 28에 높이 22인 버튼 이미지리스트

	//CStringArray		m_arrayToolName;		//분석도구리스트 이름
	CArray<int, int>	m_arrayTotalIdx;		//전체리스트 인덱스
	CArray<int, int>	m_arrayAnalIdx;			//분석도구리스트 인덱스
	CArray<int, int>	m_arrayAdditionIdx;		//부가기능리스트 인덱스
	CArray<int, int>	m_arrayUserIdx;			//사용자설정리스트 인덱스
	CMap<int, int, CString, CString>	m_mapToolName;			//분석도구리스트 이름
	void ReadIniFile(BOOL bReload = FALSE);
	void GetFieldData(CString strFieldData, int &nFieldKey, CString &strFieldName);
	int m_nSelTool;

	//20070104 by 정유진
	//윈도우 크기 변경시 툴바의 마지막 버튼이 화면보다 길게 배치된경우 상/하위 메뉴보기 버튼으로 변경 
	//상위메뉴보기 버튼 클릭시 위메뉴보기
	//하위메뉴보기 버튼 클릭시 아래 메뉴보기
	void ResizeToolbar(int nY);
	BOOL DrawHighMenu(int nHighMenuCount, int nSelTool);
	BOOL DrawLowMenu(int nLowMenuCount, int nHighMenuCount, int nSelTool);
	void GetDrawInfo(int nY, int nSelTool);
	typedef struct _DRWAINFO
	{
		int nPreWindowPos;					//윈도우의 이전 rect.bottom
		int nCurrWindowPos;					//윈도우의 현재 rect.bottom
		int nRealMenuCount;					//실제 툴바 버튼 갯수
		int nCalcMenuCount;					//변경된 toolbar에 배치 가능한 버튼 갯수
		int nHighMenuCount;					//윈도우 rect.bottom < toolbar rect.bottom 경우 위에 배치 가능한 버튼 갯수
		int nLowMenuCount;					//윈도우 rect.bottom < toolbar rect.bottom 경우 아래에 배치 가능한 버튼 갯수
		int nLastBtnPos;					//가장 마지막 버튼의 rect.bottom
		int nLowBojo;						//아래 버튼 클릭시 뒤에 표시할 버튼이 남아있는 경우 갯수
		int nLowLastIndex;					//가장 마지막에 표시된 버튼의 index
		int nHighBojo;												
		int nHighLastIndex;
		int nTotBojo;
	} DRWAINFO;

	DRWAINFO m_drawinfo;
	int m_nResizeFlag;						//ResizeToolbar func 호출 유무 

	CILButton* m_pBtnUp;
	CILButton* m_pBtnDown;
	// 이미지
	CImageList		m_ImgList;
	BOOL m_bInit;
	//by 정유진 end
	
public:
// Dialog Data
	//{{AFX_DATA(CDlgToolbar)
	enum { IDD = IDD_DLG_TOOLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgToolbar)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgToolbar)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTOOLBAR_H__F80CD3CD_73BD_46D1_B3DD_35C2D8037915__INCLUDED_)
