#pragma once
// CancelWnd.h : header file
//

#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxEdit.h"
#include "CancelGrid.h"
#include "ContractGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CCancelWnd window

class CMapWnd;

class CCancelWnd : public CWnd
{
// Construction
public:
	CCancelWnd(CWnd* pWizard);

// Attributes
public:

// Operations
public:
	void Init();
	void InitSymbol(CRect rcCtrl);
	void QuerySymbol(CString strCode);
	void ParsingSymbol(WPARAM wParam, LPARAM lParam);
	void ParsingNCDetail(CString strData, int nLen, CString strMsg);
	void ParsingNonContract(CString strData, int nLen, CString strMsg);
	void ParsingOrder(CString strData, int nLen, CString strMsg);
	void ClearCancelGrid();
	void ClearContractGrid();
	void QueryNCDetail(CString strFundNo);
	void SendOrder(BOOL bStart = TRUE);
	bool IsNumber(CString str);
	void ChangeSize(double* lRate, int ipoint, LPCTSTR lszname);
	void SetPallete(CMapWnd* pMapWnd);
	CWnd* m_pSymbol{};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCancelWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void GroupListComplete();
	void ChangeSiseData();
	CUGCtrl* GetGridWnd();
	void SetCode(CString strCode);
	CComboBox* GetGroupCombo();
	CComboBox* GetGroupIDCombo();
	virtual ~CCancelWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCancelWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnAddAccount();
	afx_msg void OnCodeSearch();
	afx_msg void OnClear();
	afx_msg void OnNonContract();
	afx_msg void OnOrder();
	afx_msg void OnContract();
	afx_msg void OnSelGroup();
	afx_msg void OnCodeSelEndOk();
	afx_msg void OnCostTypeSelEndOk();
	afx_msg void OnTypeSelEndOk();
	afx_msg void OnCost();
	afx_msg long OnContractGridRowChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPassSet();
	DECLARE_MESSAGE_MAP()

private:
	
	CWnd* m_pWizard{};
	CMapWnd* m_pParent{};

	std::unique_ptr<CfxStatic> m_pGroupStatic{};
	std::unique_ptr <CComboBox> m_pGroup{};
	std::unique_ptr <CComboBox> m_pGroupID{};
	
	std::unique_ptr <CfxImgButton> m_pAddAccount{};
	std::unique_ptr <CfxImgButton> m_pPassSet{};

	std::unique_ptr <CfxImgButton> m_pClear{};
	std::unique_ptr <CfxImgButton> m_pNonContract{};
	std::unique_ptr <CfxImgButton> m_pOrder{};
	std::unique_ptr <CfxImgButton> m_pContract{};

	std::unique_ptr <CfxCodeCtrl> m_pCodeCtrl{};

	std::unique_ptr <CfxImgButton> m_pCodeSearch{};
	
	HINSTANCE	m_hiSymbol{};

	std::unique_ptr <CfxStatic> m_pTypeStatic{};
	std::unique_ptr <CComboBox> m_pType{};

	std::unique_ptr <CfxStatic> m_pQuantityStatic{};
	std::unique_ptr <CButton> m_pRemainQty{};
	std::unique_ptr <CButton> m_pPartQty{};

	std::unique_ptr <CfxStatic> m_pOrderTypeStatic{};
	std::unique_ptr <CButton> m_pNormalType{};

	std::unique_ptr <CCancelGrid> m_pCancelGrid{};
	std::unique_ptr <CContractGrid> m_pContractGrid{};

	CStringArray m_arrSendData{};
	int m_nCurOrder{};
};

