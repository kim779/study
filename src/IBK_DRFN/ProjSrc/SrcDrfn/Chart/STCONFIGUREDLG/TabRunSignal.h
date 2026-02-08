#if !defined(AFX_TABRUNSIGNAL_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
#define AFX_TABRUNSIGNAL_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabRunSignal.h : header file
//
#include "Resource.h"
#include "RscDlg.h"
#include "Control/ColorButton.h"
#include "Control/BitmapItemComboBox.h"

#include "../../inc/ExCommonHeader.h"
#include "../../inc/ExComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTabRunSignal dialog

class CTabRunSignal : public CRscDlg
{
// Construction
public:
	CTabRunSignal(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}
	void OnBtnClr(CColorButton &rColorButton);		// 버튼의 색상을 변경할 수 있도록 한다.

// Dialog Data
	//{{AFX_DATA(CTabRunSignal)
	enum { IDD = IDD_DLG_RUNSIGNAL };
	CExComboBox		m_cbSell;
	CExComboBox		m_cbExitshort;
	CExComboBox		m_cbExitlong;
	CExComboBox		m_cbBuy;
	CColorButton	m_btnClrSell;
	CColorButton	m_btnClrExitshort;
	CColorButton	m_btnClrExitlong;
	CColorButton	m_btnClrBuy;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabRunSignal)
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

public:


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabRunSignal)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtClrBuy();
	afx_msg void OnBtClrExitlong();
	afx_msg void OnBtClrExitshort();
	afx_msg void OnBtClrSell();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABRUNSIGNAL_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
