#if !defined(AFX_TRMCHARTVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
#define AFX_TRMCHARTVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrMChartView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrMChartView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "resource.h"

#include "../../inc/IAUGwanMng.h"
#include "../../inc/IWinixPlatform.h"

class CWinixGwanManager : public IWinixGwanManager
{
public:
	CWinixGwanManager() {m_pGwanMng=NULL; m_pGwandwRet=0;}
	void DoInit()
		{	m_pGwanMng = (IAUGwanManager*)AfxGetPctrInterface(UUID_IAUGwanManager);	}

public:
	DECLARE_IWinixGwanManager_MAP()
};

class CTrMChartView : public CFormView
{
protected:
	CTrMChartView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTrMChartView)

	List_stWINIXComm m_CommList;
	BOOL _DoToCommList(BOOL bAdvise, stWINIXComm* pItem);
	void _DoClearCommList();

public:
	stWINIXComm* GetSendComm(int nKey);

// 	HWND			m_hFormMng;
	int				m_nInitFlag;

//	IButtonCtl		m_btnChartDisp;
//	IComboCtl		m_cmbQueryTpDisp;
// 	IJMComboCtl		m_cmbJCodeDisp;
// 	ILabelCtl		m_lbJCodeNameDisp;
// 	ILabelCtl		m_lbScreenType;
// 	ILabelCtl		m_lbMarketType;

	long			m_lBkColor;
	COLORREF		m_crBackColor;			// 화면 백그라운드

	CMapStringToOb	*m_MapControlName;		// 화면에 생성된 Control들을 관리하는 Map

//	class CRqMsg	m_rgMsg;				// FID 전문작성용 클래스
	
	CWnd* GetFrameWnd(CWnd*	pWnd);
// Form Data
public:
	//{{AFX_DATA(CTrMChartView)
	enum { IDD = IDD_MCHARTVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	void SetChangeCtrlID();
	void ControlMove();

	afx_msg long OnGetData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnGetBrod(WPARAM wParam, LPARAM lParam);
	afx_msg long OnLayOut(WPARAM wParam, LPARAM lParam);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrMChartView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTrMChartView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTrMChartView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg	long OnRmsgGetInterface(WPARAM wp, LPARAM lp);
	afx_msg	long OnRmsgWinixPlatform(WPARAM wp, LPARAM lp);


// afx_msg	HRESULT OnGetScreenType(WPARAM wParam, LPARAM lParam);
// afx_msg HRESULT OnGetMarketType(WPARAM wParam, LPARAM lParam);
// afx_msg HRESULT OnGetLinkInfoData(WPARAM wParam, LPARAM lParam);
// afx_msg HRESULT OnOpenScreen(WPARAM wParam, LPARAM lParam);
// afx_msg HRESULT OnOrderHoga(WPARAM wParam, LPARAM lParam);
// afx_msg void OncmbJCodeSelChanged_EditFull();
// afx_msg void OncmbJCodeSelChanged_EditEnter();
// afx_msg void OncmbJCodeSelChanged_SelChanged();
// 
// LRESULT DoRmgWrapGetInfo(WPARAM wParam, LPARAM lParam);
// afx_msg long OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam);
// afx_msg void OnFormInitAfter();
// afx_msg void OnFormGetPageSave(LPCTSTR strFileName);
// afx_msg void OnFormSetPageSave(LPCTSTR strFileName);
// afx_msg void OnColorChange();

	// 컨트롤의 HWND 리턴..
	HWND GetControlHwnd(LPCSTR szCtrlName);
	
public:
	BOOL CreateDll();
	HINSTANCE				m_hInstance;
 	CWnd					*m_pChartScreen;
	CString					m_strFormCode;
	CString					m_strMapFileName;
	BOOL					m_IsLoading;
	//int						m_nCurMarketType;
// 	IDispatch*		m_ScrFormDisp;
// 	IRunFormCtl		m_Form;
//	void	OpenScreen(CString strScreen, CStringList *p_strData);
// 	void	SetFrameMsgText(CString strMsg);
 	HWND	m_hChildFrame;
// 	void	SetCodeDragAndDrop(CString strCode);
// 	CWnd*			m_pFormWnd;

	// Screen Type & Market Type
//	enum { ST_NONE = -1, ST_MULTICHART = 0, ST_STOCKCHART, ST_UPJONGCHART, ST_FUTOPTCHART, ST_STOCKFUTOPTCHART, ST_FOREIGNCHART , ST_MARKETFUTURE, ST_FXCHART};
	enum { MT_NONE = -1, MT_STOCK = 0, MT_UPJONG, MT_FUTOPT, MT_STOCKFUTOPT, MT_FOREIGN };
	enum { MARKET_UNKNOWN = -1, MARKET_KOSPI_STOCK, MARKET_KOSDAQ_STOCK, MARKET_ELW, MARKET_KOSPI_UPJONG, MARKET_KOSDAQ_UPJONG, MARKET_KFUTURE, MARKET_KOPTION, MARKET_FUTURE_SPREAD, MARKET_STOCKFUT, MARKET_STOCKOPT, MARKET_US_ITEM};
	int m_nEnumScreenType;

	CWinixGwanManager m_wxGwnMng;
	CString	m_szFileKey;


// 	void ChangeCodeInputStyle(CString strCode);
// 	void GetLastData(LPCTSTR strPageScrNo, LPCTSTR strPageIndex, CString& strData);
// 	void SetLastData(LPCTSTR strPageScrNo, LPCTSTR strPageIndex, LPCTSTR strData);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRMCHARTVIEW_H__ABD2E8FC_53A2_4A21_8BFE_49023C0D6025__INCLUDED_)
