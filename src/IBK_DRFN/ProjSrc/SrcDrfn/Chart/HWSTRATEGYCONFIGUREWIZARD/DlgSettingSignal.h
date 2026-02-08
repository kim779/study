#if !defined(AFX_DLGSETTINGSIGNAL_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_)
#define AFX_DLGSETTINGSIGNAL_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSettingSignal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingSignal dialog
#include "Resource.h"
#include "control/BitmapItemComboBox.h"
#include "Control/ColorButton.h"
#include "Control/xSkinButton.h"

class CDlgSettingSignal : public CDialog
{
// Construction
public:
	CDlgSettingSignal(CWnd* pParent = NULL);   // standard constructor
	void SetDlgStrategyList(CWnd *pWnd) { m_pDlgStrategyList = pWnd;}
	void UpdateInfo();
// Dialog Data
	//{{AFX_DATA(CDlgSettingSignal)
	enum { IDD = IDD_DLG_SIGNAL };
	CButton	m_chkShowQty;
	CButton	m_chkShowName;
	CxSkinButton	m_btnOk;
	CxSkinButton	m_btnCancel;
	CColorButton	m_btnClrExitshort;
	CColorButton	m_btnClrExitlong;
	CColorButton	m_btnClrBuy;
	CColorButton	m_btnClrSell;
	CBitmapItemComboBox	m_cbSell;
	CBitmapItemComboBox	m_cbExitshort;
	CBitmapItemComboBox	m_cbExitlong;
	CBitmapItemComboBox	m_cbBuy;
	//}}AFX_DATA

	BOOL	m_bUseExitshort;
	BOOL	m_bUseExitlong;
	BOOL	m_bUseBuy;
	BOOL	m_bUseSell;
	BOOL	m_bShowName;
	BOOL	m_bShowQty;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSettingSignal)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetCBSignal(CBitmapItemComboBox &rCb);
	void OnBtnClr(CColorButton &rColorButton);
	CWnd *m_pDlgStrategyList;
	COLORREF m_crBk;
	CBrush	m_brBkColor;
	// Generated message map functions
	//{{AFX_MSG(CDlgSettingSignal)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBtClrBuy();
	afx_msg void OnBtClrExitlong();
	afx_msg void OnBtClrExitshort();
	afx_msg void OnBtClrSell();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnChkShowname();
	afx_msg void OnChkShowqty();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETTINGSIGNAL_H__6029D970_3E68_4DF0_84BE_B12B9DD33C6F__INCLUDED_)
