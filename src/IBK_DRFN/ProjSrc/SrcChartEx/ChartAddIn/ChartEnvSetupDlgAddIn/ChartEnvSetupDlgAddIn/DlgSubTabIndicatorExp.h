#if !defined(AFX_DLGSUBTABINDICATOREXP_H__754FEB0D_BD62_4D1F_B2DF_7642E8C21492__INCLUDED_)
#define AFX_DLGSUBTABINDICATOREXP_H__754FEB0D_BD62_4D1F_B2DF_7642E8C21492__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSubTabIndicatorExp.h : header file
//

#include "Include_AddIn.h"		// for IGraphInfo
#include "TabSubIndicatorSetUpCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgSubTabIndicatorExp dialog
class CMyDocument : public CDocument
{
public:
	CMyDocument() {}
	virtual ~CMyDocument() {}
};
class CIndicatorExpHtmlView;
class CDlgSubTabIndicatorExp : public CDialog
{
// Construction
public:
	CDlgSubTabIndicatorExp( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL );		// standard constructor
	~CDlgSubTabIndicatorExp();							// standard destructor

// Dialog Data
	//{{AFX_DATA(CDlgSubTabIndicatorExp)
	enum { IDD = IDD_SUBTAB_PAGE_EXP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSubTabIndicatorExp)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void ReadIndicatorExpFile( CString strIndicatorName = _T("") );

private:
	CDialog *m_pDlgMainTabIndicatorSetUp;

	CIndicatorExpHtmlView *m_pIndicatorExpHtmlView;

	CString m_strBaseFilePath;
	IChartOCX *		m_pIChartOCX;

// (2008/1/16 - Seung-Won, Bae) for MultiLanguage
protected:
	HWND	m_hOcxWnd;
	
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSubTabIndicatorExp)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUBTABINDICATOREXP_H__754FEB0D_BD62_4D1F_B2DF_7642E8C21492__INCLUDED_)
