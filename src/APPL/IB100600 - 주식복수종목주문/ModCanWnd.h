#pragma once
// ModCanWnd.h : header file
//

#include "ModCanGrid.h"
#include "AccountCtrl.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxCodeCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CModCanWnd window

class CMapWnd;

class CModCanWnd : public CWnd
{
// Construction
public:
	CModCanWnd(CWnd* pWizard);

// Attributes
public:

// Operations
public:
	void SetPallete();
	void Init();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModCanWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	std::unique_ptr<CAccountCtrl> m_pAccount{};
	std::unique_ptr<CfxEdit> m_pPassword{};
	void SendOrder(BOOL bModify);
	int IsMandatoryField();
	int GetOrderType();
	void QueryNonContract(CString strCode, int nType);
	virtual ~CModCanWnd();
	void SetAccount(CString strAccount);
	void SetCode(CString strCode);
	void ParsingSymbol(WPARAM wParam, LPARAM lParam);
	void QuerySymbol(CString strCode);
	void ParsingNonCont(CString strData, int nLen, CString strMsg);
	void ParsingSCHoga(CString strData, BOOL bHoga, BOOL bStdPrc = FALSE);	//2015.06.12 KSJ 기준가 구하기 추가
	void ParsingSOrder(CString strData, int nLen, CString strMsg);
	inline BOOL IsBlocking() { return m_bBlock; };
	inline CWnd* GetAccountCtrl() { return (CWnd*)m_pAccount.get(); };
	void SetInputFieldFocus();
	void ClearModCanGrid();
	inline CWnd* GetWizard() { return m_pWizard; };

	void ChangeSize(double* lRate, int ipoint, LPCTSTR lszname);

	// Generated message map functions
protected:
	//{{AFX_MSG(CModCanWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnCodeSelEndOk();
	afx_msg void OnSelchangePriceUnit();
	afx_msg void OnNonContract();
	afx_msg void OnAll();
	afx_msg void OnBuy();
	afx_msg void OnSell();
	afx_msg void OnModify();
	afx_msg void OnCancel();
	afx_msg void OnPercent();
	afx_msg void OnCodeSearch();
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContrect();
	afx_msg void OnClear();
	afx_msg LRESULT OnMyPassword(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void InitSymbol(CRect rcCtrl);
	CString GetCodeName(CString strCode);
	CString Parser(CString&srcstr, CString substr);
	void QuerySCHoga(BOOL bHoga, CString strCode = _T(""), BOOL bStdPrc = FALSE);	//2015.06.12 KSJ 기준가 구하기 추가

	void ChkPassword();


private:
	bool IsNumber(CString str);
	CWnd* m_pWizard{};
	CMapWnd* m_pParent{};
	std::unique_ptr<CModCanGrid> m_pModCanGrid{};


	std::unique_ptr < CfxStatic > m_pAccName{};
	std::unique_ptr < CfxStatic > m_pPassStatic{};

	std::unique_ptr < CButton > m_pOrderConfirm{};

	std::unique_ptr < CfxImgButton > m_pClear{};
	std::unique_ptr < CfxImgButton > m_pNonContract{};
	std::unique_ptr < CfxImgButton > m_pModify{};
	std::unique_ptr < CfxImgButton > m_pCancel{};
	std::unique_ptr < CfxImgButton > m_pContrect{};
	std::unique_ptr < CfxImgButton > m_pExpand{};

	std::unique_ptr < CButton > m_pAll{};
	std::unique_ptr < CButton > m_pBuy{};
	std::unique_ptr < CButton > m_pSell{};

	std::unique_ptr < CfxStatic > m_pCodeCtrlStatic{};
	std::unique_ptr < CfxCodeCtrl > m_pCodeCtrl{};

	std::unique_ptr < CfxImgButton > m_pCodeSearch{};
	CWnd* m_pSymbol{};
	HINSTANCE	m_hiSymbol{};

	std::unique_ptr < CfxStatic > m_pQuantityStatic{};
	std::unique_ptr < CButton > m_pQuantityRemain{};
	std::unique_ptr < CButton > m_pQuantityPart{};

	std::unique_ptr < CfxStatic > m_pPriceUnitStatic{};
	std::unique_ptr < CComboBox > m_pPriceUnit{};

	std::unique_ptr < CfxEdit > m_pPercent{};
	std::unique_ptr < CfxStatic> m_pPercentStatic{};

	BOOL m_bBlock, m_bFirst, m_bOrderType{};
};

