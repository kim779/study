#if !defined(AFX_DLGRESULT_H__02C6E278_551B_4454_BA71_2449F1426B96__INCLUDED_)
#define AFX_DLGRESULT_H__02C6E278_551B_4454_BA71_2449F1426B96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgResult.h : header file
//
#include "resource.h"
#include "DlgOptimizer.h"
#include "GridCtrl_src/GridCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDlgResult dialog
#define     MAXDATANUM          36 
class CDlgOptimizer;
class CDlgResult : public CEscDialog
{
// Construction
private:
	CDlgOptimizer* m_pOptimizer;
public:
	CDlgResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgResult)
	enum { IDD = IDD_DLG_TAP_RESULT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgResult)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//Web
	CString m_strFilePath;
	CString m_strDir;
	CString m_strHtml;
	void InitHtml();
	void MakeHtmlFile();
	void SetOptimizer(CDlgOptimizer* Opt);
	void SetDataDir(CString DataDir);
	CString ParsingstrHtml(CString lParam);
	void OpenWebView(CString);
	CString GetComma(LPCSTR pData);
	CString GetNumber( LPCSTR pData );

	// Grid
	CBrush		m_brBackground;
	CGridCtrl m_GridResult;
	void InitGrid();
	void SetDataSpreadGrid();

	CFont*		m_pFont;
	void		SetGridFont(CFont* pFont) { m_pFont = pFont; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESULT_H__02C6E278_551B_4454_BA71_2449F1426B96__INCLUDED_)
