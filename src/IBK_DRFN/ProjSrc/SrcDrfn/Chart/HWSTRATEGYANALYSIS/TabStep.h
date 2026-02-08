#if !defined(AFX_TABSTEP_H__3E444BFE_8FFC_4FA8_90D6_3A7DFF7A0D23__INCLUDED_)
#define AFX_TABSTEP_H__3E444BFE_8FFC_4FA8_90D6_3A7DFF7A0D23__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabStep.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTabStep dialog
#include "resource.h"

#include "DlgInterface.h"
#define  STEPMAXDATANUM  165
class CTabStep : public CDlgInterface
{
// Construction
public:
	CTabStep(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTabStep)
	enum { IDD = IDD_DLG_TAP_STEP };
	CString	m_PageFrame;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabStep)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabStep)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
	//Web

	void OpenWebView(CString);
	CString m_strFilePath;
	void MakeHtmlFile();
	void InitHtml();
	CString ParsingstrHtml(CString strHtml);
	int m_nDiv;
	CString m_strHtml;
	//Grid°ü·Ã
	CGridCtrl m_StepGridCtrl;
	CFont*		m_pFont;
	void		SetGridFont(CFont* pFont) { m_pFont = pFont; }

	void InitGrid();
	void SetDataSpreadGrid();
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSTEP_H__3E444BFE_8FFC_4FA8_90D6_3A7DFF7A0D23__INCLUDED_)
