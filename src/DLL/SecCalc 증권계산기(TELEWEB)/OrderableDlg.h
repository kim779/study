#if !defined(AFX_ORDERABLEDLG_H__032B6422_CAFC_424C_ADFF_7D73B81BA2DA__INCLUDED_)
#define AFX_ORDERABLEDLG_H__032B6422_CAFC_424C_ADFF_7D73B81BA2DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrderableDlg.h : header file
//
#include "MiniCalculator.h"

/////////////////////////////////////////////////////////////////////////////
// COrderableDlg dialog

class COrderableDlg : public CDialog
{
// Construction
public:
	COrderableDlg(class CSecCalcDlg *pDlg, CWnd* pParent = NULL);   // standard constructor
	void OnChangeFee();

	CSecCalcApp		*m_pApp;
	class CSecCalcDlg	*m_pMainDlg;
	CMiniCalculator		m_Calc;
	int			m_CurMarket;

	CColorButton		m_pColorButton[23];
// Dialog Data
	//{{AFX_DATA(COrderableDlg)
	enum { IDD = IDD_ORDERABLE_DIALOG };
	CFreeEdit	m_CalcEdit;
	CButton	m_ctrlCheckSingle;
	CStatic	m_ctrlStaticCountUnit;
	CStatic	m_ctrlStaticPriceUnit;
	CStatic	m_ctrlStaticOrderable;
	CStatic	m_ctrlStaticMoney;
	CStatic	m_ctrlStaticFee;
	CStatic	m_ctrlStaticBalance;
	CButton	m_ctrlRadioStock;
	CButton	m_ctrlRadioFuture;
	CButton	m_ctrlRadioOption;
	CButton	m_ctrlRadioElw;
	CButton	m_ctrlFreeRate;
	CCommaEdit	m_ctrlInputPrice;
	CCommaEdit	m_ctrlInputMoney;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderableDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrderableDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioStock();
	afx_msg void OnRadioFuture();
	afx_msg void OnRadioOption();
	afx_msg void OnRadioElw();
	afx_msg void OnBtnCalc();
	afx_msg void OnBtnClear();
	afx_msg void OnBtnChange();
	afx_msg void OnStaticFstock();
	afx_msg void OnStaticFfuture();
	afx_msg void OnStaticFoption();
	afx_msg void OnStaticFelw();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg long OnChild(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCalc(UINT nID);
	DECLARE_MESSAGE_MAP()

	void SetMkRadio(int kind);
	void OnMkRadio(int kind, bool bForce = false);
	void CalcOrderable(double dMoney, double dPrice);
	int IsCalcChar(int key);
	void InitControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERABLEDLG_H__032B6422_CAFC_424C_ADFF_7D73B81BA2DA__INCLUDED_)
