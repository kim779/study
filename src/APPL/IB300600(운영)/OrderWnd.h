#pragma once
// OrderWnd.h : header file
//

#include "AccountCtrl.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "NumSpinCtrl.h"
#include "OrderGrid.h"

/////////////////////////////////////////////////////////////////////////////
// COrderWnd window

class CMapWnd; 

class COrderWnd : public CWnd
{
// Construction
public:
	COrderWnd(CWnd* pWizard);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPallete();
	void SetInputFieldFocus();
	void ParsingFOrder(CString strData, int nLen, CString strMsg);
	inline BOOL IsBlocking() { return m_bBlock; };
	void QueryFOCHoga(BOOL bHoga, CString strCode = _T(""));
	CString Parser(CString &srcstr, CString substr);
	void ParsingFOCHoga(CString strData, BOOL bHoga);
	void ClearOrderGrid();
	void ParsingFRemain(CString strData, int nLen, CString strMsg);
	inline CWnd* GetMapWnd() { return GetParent(); };
	inline CWnd* GetAccountCtrl() { return (CWnd*)m_pAccount; };
	void SetHogaData(CString strData);
	CString GetOrderState();
	CString GetOrderPriceType();
	CString GetQuantity();
	CString GetOrderPrice();
	CString GetOrderType();
	void SetHistory(CString strData);
	CWnd* GetWizard();
	CString GetHistory(CString strSymbol);
	void Init();
	void OnAlert(CString altmsg);
	virtual ~COrderWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(COrderWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg void OnClear();
	afx_msg void OnRemain();
	afx_msg void OnOrderSend();
	afx_msg void OnBuy();
	afx_msg void OnSell();
	afx_msg void OnSelchangeOrderType();
	afx_msg void OnSelchangeOrderPriceType();
	afx_msg void OnQuantity();
	afx_msg void OnOrderPrice();
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContract();
	afx_msg LRESULT OnMyPassword(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	bool IsNumber(CString str);
	CWnd* m_pWizard;
	CMapWnd* m_pParent;
	CAccountCtrl* m_pAccount;
	CfxStatic* m_pAccName;
	CfxStatic* m_pPassStatic;
	CfxStatic* m_pOrderTypeStatic;
	CfxStatic* m_pOrderPriceStatic;
	CfxStatic* m_pQuantityStatic;

	CfxEdit* m_pPassword;
	CfxEdit* m_pOrderPrice;
	CfxEdit* m_pQuantity;

	CfxImgButton* m_pClear;
	CfxImgButton* m_pRemain;
	CfxImgButton* m_pOrderSend;
	CfxImgButton* m_pContract;

	CButton* m_pBuy;
	CButton* m_pSell;

	CComboBox* m_pOrderType;
	CComboBox* m_pOrderPriceType;

	CNumSpinCtrl* m_pOrderSpin;
	CNumSpinCtrl* m_pQuantitySpin;

	COrderGrid*	m_pOrderGrid;

	CString m_strCurPrice;
	CStringArray m_arrBuyFive;
	CStringArray m_arrSellFive;

	BOOL m_bBlock;
};

