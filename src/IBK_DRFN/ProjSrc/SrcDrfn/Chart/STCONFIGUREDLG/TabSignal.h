#if !defined(AFX_TABSIGNAL_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
#define AFX_TABSIGNAL_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabSignal.h : header file
//
//#include "Resource.h"
#include "RscDlg.h"
#include "Control/ColorButton.h"
// #include "Control/BitmapItemComboBox.h"

/////////////////////////////////////////////////////////////////////////////
// CTabSignal dialog

class CTabSignal : public CRscDlg
{
// Construction
public:
	CTabSignal(CWnd* pParent = NULL);   // standard constructor
	void SetColor(COLORREF& rCrBk) { m_crBk = rCrBk;}
	void SetBrush(CBrush*	pBrBkColor) { m_pBrBkColor = pBrBkColor;}

// Dialog Data
	//{{AFX_DATA(CTabSignal)
	enum { IDD = IDD_DLG_TAB_SIGNAL };
	CButton	m_chkSaveAsDefault;
	CButton	m_chkShowQty;
	CButton	m_chkShowName;
	CBitmapItemComboBox	m_cbSell;
	CBitmapItemComboBox	m_cbExitshort;
	CBitmapItemComboBox	m_cbExitlong;
	CBitmapItemComboBox	m_cbBuy;
	CColorButton	m_btnClrSell;
	CColorButton	m_btnClrExitshort;
	CColorButton	m_btnClrExitlong;
	CColorButton	m_btnClrBuy;
	//}}AFX_DATA

	void OnBtnClr(CColorButton &rColorButton);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabSignal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
private:
	void SetCBSignal(CBitmapItemComboBox &rCb);
	void LoadInfo();
	BOOL SaveInfo();
	void RemoveInfo();
	BOOL m_bUseBuy;
	BOOL m_bUseSell;
	BOOL m_bUseExitlong;
	BOOL m_bUseExitshort;
	COLORREF m_crBk;
	CBrush*	m_pBrBkColor;
	CWnd*	m_pDlgSTOption;


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTabSignal)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtClrBuy();
	afx_msg void OnBtClrExitlong();
	afx_msg void OnBtClrExitshort();
	afx_msg void OnBtClrSell();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSIGNAL_H__6BA8C800_72A9_4B5D_8266_7D60B25A1197__INCLUDED_)
