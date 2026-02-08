#if !defined(AFX_TABRUNSETUP_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
#define AFX_TABRUNSETUP_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabRunSetup.h : header file
//
#include "Resource.h"
#include "RscDlg.h"
#include "Control/ColorButton.h"
#include "Control/BitmapItemComboBox.h"
#include "Control/XEdit.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTabRunSetup dialog

class CTabRunSetup : public CRscDlg
{
// Construction
public:
	CTabRunSetup(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}

// Dialog Data
	//{{AFX_DATA(CTabRunSetup)
	enum { IDD = IDD_DLG_RUNSETUP };
	CExComboBox	m_cbP1;
	CExComboBox	m_cbP2;
	CExComboBox	m_cbP3;
	CExComboBox	m_cbP4;
	CExComboBox	m_cbP5;
	CExComboBox	m_cbP6;
	CExComboBox	m_cbP7;
	CExComboBox	m_cbS1;
	CExComboBox	m_cbS2;
	CExComboBox	m_cbS3;
	CExComboBox	m_cbS4;
	CExComboBox	m_cbS5;
	CColorButton	m_btnC1;
	CColorButton	m_btnC2;
	CColorButton	m_btnC3;
	CColorButton	m_btnC4;
	CColorButton	m_btnC5;
	CExComboBox	m_cbTime;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabRunSetup)
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
	//{{AFX_MSG(CTabRunSetup)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtC1();
	afx_msg void OnBnClickedBtC2();
	afx_msg void OnBnClickedBtC3();
	afx_msg void OnBnClickedBtC4();
	afx_msg void OnBnClickedBtC5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void OnBtnClr(CColorButton &rColorButton);
	CXEdit m_xEditTime;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRUNSETUP_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
