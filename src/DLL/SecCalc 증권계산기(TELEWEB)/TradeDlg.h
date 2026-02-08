#if !defined(AFX_TRADEDLG_H__31CB8151_8FDE_4974_9EF3_7511C68F9E8C__INCLUDED_)
#define AFX_TRADEDLG_H__31CB8151_8FDE_4974_9EF3_7511C68F9E8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TradeDlg.h : header file
//

#include "MiniCalculator.h"

/////////////////////////////////////////////////////////////////////////////
// CTradeDlg dialog

class CTradeDlg : public CDialog
{
// Construction
public:
	CTradeDlg(class CSecCalcDlg *pDlg, CWnd* pParent = NULL);   // standard constructor
	void OnChangeFee();

	CSecCalcApp		*m_pApp;
	class CSecCalcDlg	*m_pMainDlg;
	CMiniCalculator		m_Calc;
	int			m_CurMarket;

	CColorButton		m_pColorButton[23];

// Dialog Data
	//{{AFX_DATA(CTradeDlg)
	enum { IDD = IDD_TRADE_DIALOG };
	CButton	m_Calc_0;
	CFreeEdit	m_CalcEdit;
	CCommaEdit	m_ctrlInputVol;
	CCommaEdit	m_ctrlInputPrice;
	CStatic	m_ctrlStaticIUVol;
	CStatic	m_ctrlStaticIUPrice;
	CStatic	m_ctrlStaticIVol;
	CStatic	m_ctrlStaticIPrice;
	CStatic	m_ctrlStaticMoney;
	CStatic	m_ctrlStaticTax;
	CStatic	m_ctrlStaticFee;
	CButton	m_ctrlRadioSell;
	CButton	m_ctrlRadioBuy;
	CButton	m_ctrlRadioStock;
	CButton	m_ctrlRadioFuture;
	CButton	m_ctrlRadioOption;
	CButton	m_ctrlRadioElw;
	CButton	m_ctrlFreeRate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTradeDlg)
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
	afx_msg void OnRadioBuy();
	afx_msg void OnRadioSell();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg long OnChild(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCalc(UINT nID);
	DECLARE_MESSAGE_MAP()

	void SetMkRadio(int kind);
	void OnMkRadio(int kind, bool bForce = false);
	void OnOdrRadio(int kind);
	int IsCalcChar(int key);
	void InitControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADEDLG_H__31CB8151_8FDE_4974_9EF3_7511C68F9E8C__INCLUDED_)
