#if !defined(AFX_DLGTRADE_H__4D38B933_DDC4_4DF6_A31D_ADDC009FDBC3__INCLUDED_)
#define AFX_DLGTRADE_H__4D38B933_DDC4_4DF6_A31D_ADDC009FDBC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTrade.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTrade dialog
#include "resource.h"
#include "DlgOptimizer.h"
#include "GridCtrl_src/GridCtrl.h"
class CDlgOptimizer;
class CDlgTrade : public CDlgInterface
{
// Construction
public:
	CDlgTrade(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTrade)
	enum { IDD = IDD_DLG_TAP_TRADE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTrade)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTrade)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CIStrategyItem *m_pStrategyItem;
//	CDlgOptimizer* m_pDlgOptimizer;
public:
	//Web

	CString m_strHtml;
	CString m_strDir;
	void InitHtml();
	void SetStrategyItem(CIStrategyItem* pStrat);
	void SelectRunData(CString pRunData);
	void SetOptimizer(CDlgOptimizer* Opt);
	void SetDataDir(CString DataDir);
	void OpenWebView(CString);

	void GetSTMessageData();

	void InsertMonthGubun(CString &str);
	void InsertDateGubun(CString &str);
	void InsertTimeGubun(CString &str);
	BOOL IsInsertGubun(CString str);

	//KHD : 2006.11.28
	void InsertMinuteGubun(CString &str, BOOL IsHtml);
	BOOL InsertGubun(CString &str,BOOL IsHtml = FALSE);

	//Grid ฐüทร
	CGridCtrl m_TradeGridCtrl;
	void InitGrid();
	void SetDataSpreadGrid();

	CFont*		m_pFont;
	void		SetGridFont(CFont* pFont) { m_pFont = pFont; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTRADE_H__4D38B933_DDC4_4DF6_A31D_ADDC009FDBC3__INCLUDED_)
