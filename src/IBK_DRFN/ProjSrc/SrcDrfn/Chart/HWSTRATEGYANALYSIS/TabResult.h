#if !defined(AFX_TABRESULT_H__D74F3D73_FF66_4C49_A395_BF45B90C5C0A__INCLUDED_)
#define AFX_TABRESULT_H__D74F3D73_FF66_4C49_A395_BF45B90C5C0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabResult.h : header file
//
#define IDC_GRID	0x00112
/////////////////////////////////////////////////////////////////////////////
// CTabResult dialog
#include "resource.h"
#include "DlgInterface.h"

#define     MAXDATANUM          36 

class CTabResult : public CDlgInterface
{
// Construction
public:
	CTabResult(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabResult)
	enum { IDD = IDD_DLG_TAP_RESULT };
	CStatic	m_PageFrame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabResult)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	int m_iDateIndex;
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
	//Web
	CString m_strFilePath;
	CString m_strHtml;
	void OpenWebView(CString);
	void MakeHtmlFile();
	CString ParsingstrHtml(CString strHtml);
	void InitHtml();
	//HeaderInfo//
	CString m_strItemName;
	CString m_strStrategyName;
	CString m_strPriod;
	CString m_strCurrentPos;
	CString m_strACCumProf;
	//Pasing Index Decimal Point
	CString m_pHtmlCodeNameArray[MAXDATANUM];
	int m_nViewPointIndex[MAXDATANUM];
	void InitPasingData();
	void	InsertData2HtmlString(short nIndex, CString& strHtml, CIStrategyItem* pStrategy);
	BOOL	ParsingstrHtml2(CString& strHtml);
// Grid
	CBrush		m_brBackground;
	CGridCtrl m_GridResult;
	CFont*		m_pFont;
	void		SetGridFont(CFont* pFont) { m_pFont = pFont; }
	void InitGrid();
	void SetDataSpreadGrid();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRESULT_H__D74F3D73_FF66_4C49_A395_BF45B90C5C0A__INCLUDED_)
