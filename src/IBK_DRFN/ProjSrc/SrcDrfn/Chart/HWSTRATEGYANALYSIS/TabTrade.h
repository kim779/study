#if !defined(AFX_TABTRADE_H__A638707A_E733_4CE8_9BE5_54F121C0CA00__INCLUDED_)
#define AFX_TABTRADE_H__A638707A_E733_4CE8_9BE5_54F121C0CA00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabTrade.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabTrade dialog
#include "DlgInterface.h"
#define TRAMAXDATANUM 10
class CTabTrade : public CDlgInterface
{
// Construction
public:
	CTabTrade(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabTrade)
	enum { IDD = IDD_DLG_TAP_TRADE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabTrade)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabTrade)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//Web
	void OpenWebView(CString);
	CString m_strFilePath;
	
	void MakeHtmlFile();
	CString ParsingstrHtml(CString strHtml);
	void InitHtml();
	CString m_strHtml;
	int m_nDiv;
	//Grid ฐüทร
	CGridCtrl m_TradeGridCtrl;
	void InitGrid();
	void SetDataSpreadGrid();

	CString GetStrValue(HKEY Hkey,CString strkey, CString strsubkey,CString strRValue);
	void SetDataToExcel();

	CFont*		m_pFont;
	void		SetGridFont(CFont* pFont) { m_pFont = pFont; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABTRADE_H__A638707A_E733_4CE8_9BE5_54F121C0CA00__INCLUDED_)
