#if !defined(AFX_DLGANALYSIS_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_)
#define AFX_DLGANALYSIS_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAnalysis.h : header file
//

//Dlg Header // KHD 
//#include "InputLeft.h"
#include "InfoRight.h"
#include "../../inc/ExOCXDLL.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/PcTrMngHelper.h"
#include "../../inc/ISysTabExChartItem.h"
#include "./control/XListCtrl.h"

//#include "RscDlg.h
/////////////////////////////////////////////////////////////////////////////
// CDlgAnalysis dialog
#include "resource.h"

class CDlgWait;
class CDlgAnalysis : public CEscDialog
{
// Construction
public:
	CDlgAnalysis(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAnalysis)
	enum { IDD = IDD_DLG_ANALYSIS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	void	FormInit();
	void	FormLoad();
	HWND			m_hSocket;					// 소켓핸들
	HWND			m_hMainFrame;				// CMainFrame의 Handle

//	CPCTrMngHelper  m_pPcTrLib;
	CString			m_strTrCode;				// 화면번호
//@유진삭제
//	CString			m_strUserID;
//@유진삭제
	CString			m_strRootPath;
//@유진삭제
	CString			m_strUserDir;
//@유진삭제
	CString			m_strDataDir;

	int				m_nStartType;			// 0:성과분석, 1:적용실행


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAnalysis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CIStrategyItem *m_pStrategyItem;
	CStrategyLoader m_strategyLoader;

	CDlgWait*	m_pDlgWait;

	// Generated message map functions
	//{{AFX_MSG(CDlgAnalysis)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//afx_msg	LRESULT OnGetAupSize(WPARAM wParam, LPARAM lParam); //화면로딩시 화면크기를 알기위해 호출됨.
	afx_msg	LRESULT OnGetDllSize(WPARAM wParam, LPARAM lParam); //화면로딩시 화면크기를 알기위해 호출됨.

	LRESULT OnSTDataManagerMessagge( WPARAM wParam, LPARAM lParam);

	// 060721_001 JLO
	virtual void OnCancel();
	// ~060721_001 JLO

public:
	CRect GetRect(int nID);
	//CInputLeft* m_pdlgInput;
	CInfoRight* m_pdlgInfo;
    
	//KHD 추가 : 2006.07.27
	int m_nCodeType;
	CString m_stCode;
	CString m_stCodeName;
	CString m_stSTName;//전략 파일의 전체 경로
	CString m_stEndDate;
	CString m_stSTPath;
	HINSTANCE m_hRsc;
	int m_nCount;
	int m_nDivideCnt;
	int m_DivideState;	// 분할 상태, GetCode(7) 틱(0)/분(1)/일(2)/주(3)/월(4)/년(5)
	double m_dfDivideRate;
	long m_pChartInterface;
	CXListCtrl *m_pCtlList;
	CISDataItem *m_pDataItem;
	CStringArray m_stArrayPath;
	BOOL m_bGraphDivide;//분할 윈도우를 멀티챠트에 그릴지 안그릴지..(True:Draw, False:Not Draw)

	ISysTabExChartItem* m_pChartItem;
	long m_hTabChartEx;//EX포인터

	CArray<ST_DATAINDEX,ST_DATAINDEX> m_stDataIndex; //Index: 0 전체 , 1~10: 분할자
	void InitSTLoad();
	void InsertMonthGubun(CString &str);
	void InsertDateGubun(CString &str);
	void InsertTimeGubun(CString &str);
	void InsertHourGubun(CString &str);
	void InsertMinuteGubun(CString &str, BOOL IsHtml);
	BOOL InsertGubun(CString &str,BOOL IsHtml = FALSE);
	CIStrategyItem* GetStrategyItem(){return m_pStrategyItem;}
	void SetCalculateData(int nIndexnum);
	void Result();

	void WriteDebug(CString);
	CGridCtrl m_GridCtrl;
	BOOL	m_bReceiveData;

	COLORREF	m_crBk;
	CBrush		m_brBkColor;

	///END
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGANALYSIS_H__3AD78271_0EF1_40D3_A5A6_69B7558D719D__INCLUDED_)
