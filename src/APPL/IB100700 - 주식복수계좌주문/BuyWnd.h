#pragma once
// BuyWnd.h : header file
//

#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxEdit.h"
#include "BuyGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CBuyWnd window


class CMapWnd;

class CBuyWnd : public CWnd
{
// Construction
public:
	CBuyWnd(CWnd* pWizard);

// Attributes
public:

// Operations
public:
	void Init();
	void InitSymbol(CRect rcCtrl);
	void QuerySymbol(CString strCode);
	void QueryAbleQty();
	void ParsingSymbol(WPARAM wParam, LPARAM lParam);
	void ClearBuyGrid();
	void GroupListComplete();
	void ParsingOrder(CString strData, int nLen, CString strMsg);
	void SendOrder(BOOL bStart = TRUE);
	void ChangeSize(double* lRate, int ipoint, LPCTSTR lszname);
	void SetPallete(CMapWnd* pMapWnd);
	void SetCost(CString strCost);
	void ParsingAbleQty(LPCSTR dat, int len);
	CString GetStockCode();
	CWnd* m_pSymbol{};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBuyWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetGroupAccount(CStringArray* pArr);
	void ChangeSiseData();
	CUGCtrl* GetGridWnd();
	void SetCode(CString strCode);
	CComboBox* GetGroupCombo();
	CComboBox* GetGroupIDCombo();
	virtual ~CBuyWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBuyWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnAddAccount();
	afx_msg void OnCodeSearch();
	afx_msg void OnClear();
	afx_msg void OnOrder();
	afx_msg void OnContract();
	afx_msg void OnCodeSelEndOk();
	afx_msg void OnConditionSelEndOk();
	afx_msg void OnCost();
	afx_msg void OnCostTypeSelEndOk();
	afx_msg void OnGroupSelEndOk();
	afx_msg void OnQuantity();
	afx_msg void OnTypeSelEndOk();
	afx_msg void OnPassSet();
	afx_msg void OnAbleQty();
	DECLARE_MESSAGE_MAP()

private:
	int GetCheckOrder(int index);
	bool IsNumber(CString str);
	CWnd* m_pWizard{};
	CMapWnd* m_pParent{};

	std::unique_ptr<CfxStatic> m_pGroupStatic{};
	std::unique_ptr <CComboBox> m_pGroup{};
	std::unique_ptr <CComboBox> m_pGroupID{};
	
	std::unique_ptr <CfxImgButton> m_pAddAccount{};
	std::unique_ptr <CfxImgButton> m_pPassSet{};

	std::unique_ptr <CfxImgButton> m_pAbleQty{};
	std::unique_ptr <CfxImgButton> m_pClear{};
	std::unique_ptr <CfxImgButton> m_pOrder{};
	std::unique_ptr <CfxImgButton> m_pContract{};

	std::unique_ptr <CfxCodeCtrl> m_pCodeCtrl{};

	std::unique_ptr <CfxImgButton> m_pCodeSearch{};
	HINSTANCE	m_hiSymbol{};

	std::unique_ptr <CfxStatic> m_pTypeStatic{};
	std::unique_ptr <CComboBox> m_pType{};
	std::unique_ptr <CComboBox> m_pCondition{};

	std::unique_ptr <CfxStatic> m_pQuantityStatic{};
	std::unique_ptr <CfxEdit> m_pQuantity{};
//	CfxImgButton* m_pMax;

	std::unique_ptr <CfxStatic> m_pCostStatic{};
	std::unique_ptr <CComboBox> m_pCostType{};
	std::unique_ptr <CfxEdit> m_pCost{};

	std::unique_ptr <CBuyGrid> m_pBuyGrid{};
	CStringArray m_arrSendData{};
	int m_nCurOrder{};
};

