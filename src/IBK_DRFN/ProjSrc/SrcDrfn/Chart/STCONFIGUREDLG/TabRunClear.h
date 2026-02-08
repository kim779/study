#if !defined(AFX_TABRUNCLEAR_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
#define AFX_TABRUNCLEAR_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabRunClear.h : header file
//
#include "Resource.h"
#include "RscDlg.h"
//#include "Control/ColorButton.h"
//#include "Control/BitmapItemComboBox.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExDrawButton.h"
#include "../../inc/ExStatic.h"
#include "../../inc/ExEdit.h"
#include "../../inc/ExComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTabRunClear dialog

class CTabRunClear : public CRscDlg
{
// Construction
public:
	CTabRunClear(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}

// Dialog Data
	//{{AFX_DATA(CTabRunClear)
	enum { IDD = IDD_DLG_RUNCLEAR };
	CExComboBox	m_cbAllow;
	CExComboBox	m_cbRepeat;
	CExComboBox	m_cbTax;
	CExComboBox	m_cbSlipigy;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabRunClear)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
	virtual void LoadInfo();
	virtual BOOL SaveInfo();
	virtual void RemoveInfo();
	virtual void SetDefault();

protected:
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CWnd*	m_pDlgSTOption;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabRunClear)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRUNCLEAR_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
