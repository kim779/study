#if !defined(AFX_PROFITDLG_H__0AA14D29_BFE1_471A_9AF2_3A9AF03E7C6D__INCLUDED_)
#define AFX_PROFITDLG_H__0AA14D29_BFE1_471A_9AF2_3A9AF03E7C6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfitDlg.h : header file
//
#include "MiniCalculator.h"

/////////////////////////////////////////////////////////////////////////////
// CProfitDlg dialog

class CProfitDlg : public CDialog
{
// Construction
public:
	CProfitDlg(class CSecCalcDlg *pDlg, CWnd* pParent = NULL);   // standard constructor
	void OnChangeFee();

	CSecCalcApp		*m_pApp;
	class CSecCalcDlg	*m_pMainDlg;
	CMiniCalculator		m_Calc;
	int			m_CurMarket;

	CColorButton		m_pColorButton[23];

// Dialog Data
	//{{AFX_DATA(CProfitDlg)
	enum { IDD = IDD_PROFIT_DIALOG };
	CFreeEdit	m_CalcEdit;
	CCommaEdit	m_ctrlInputBuyPrc;
	CCommaEdit	m_ctrlInputSellPrc;
	CCommaEdit	m_ctrlInputSellCnt;
	CStatic	m_ctrlStaticUSellCnt;
	CStatic	m_ctrlStaticUSellPrc;
	CStatic	m_ctrlStaticUBuyPrc;
	CStatic	m_ctrlStaticProfitRate;
	CStatic	m_ctrlStaticProfit;
	CStatic	m_ctrlStaticTax;
	CStatic	m_ctrlStaticFee;
	CButton	m_ctrlRadioStock;
	CButton	m_ctrlRadioFuture;
	CButton	m_ctrlRadioOption;
	CButton	m_ctrlRadioElw;
	CButton	m_ctrlFreeRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfitDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfitDlg)
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
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg long OnChild(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCalc(UINT nID);
	DECLARE_MESSAGE_MAP()

	void SetMkRadio(int kind);
	void OnMkRadio(int kind, bool bForce = false);
	void CalcProfit(double dBuyPrice, double dSellPrice, double dCnt);
	int IsCalcChar(int key);
	void InitControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFITDLG_H__0AA14D29_BFE1_471A_9AF2_3A9AF03E7C6D__INCLUDED_)
