#pragma once
// ModifyWnd.h : header file
//

#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxCodeCtrl.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxEdit.h"
#include "ModifyGrid.h"
#include "ContractGrid.h"

/////////////////////////////////////////////////////////////////////////////
// CModifyWnd window

class CMapWnd;

class CModifyWnd : public CWnd
{
// Construction
public:
	CModifyWnd(CWnd* pWizard);

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
	void ClearModifyGrid();
	void ClearContractGrid();
	void QueryNCDetail(CString strFundNo);
	void SendOrder(BOOL bStart = TRUE);
	void ChangeSize(double* lRate, int ipoint, LPCTSTR lszname);
	void SetPallete(CMapWnd* pMapWnd);
	void SetCost(CString strCost);
	CWnd* m_pSymbol{};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModifyWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void GroupListComplete();
	void ChangeSiseData();
	CUGCtrl* GetGridWnd();
	void SetCode(CString strCode);
	CComboBox* GetGroupCombo();
	CComboBox* GetGroupIDCombo();
	virtual ~CModifyWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CModifyWnd)
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
	afx_msg void OnCodeSelEndOk();
	afx_msg void OnSelGroup();
	afx_msg void OnCostTypeSelEndOk();
	afx_msg void OnTypeSelEndOk();
	afx_msg void OnCost();
	afx_msg long OnContractGridRowChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPassSet();



	DECLARE_MESSAGE_MAP()

private:
	bool IsNumber(CString str);
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
	std::unique_ptr <CComboBox> m_pQuantity{};

	std::unique_ptr <CfxStatic> m_pOrderTypeStatic{};
	std::unique_ptr <CComboBox> m_pOrderType{};
	std::unique_ptr <CComboBox> m_pCondition{};

	std::unique_ptr <CfxStatic> m_pCostStatic{};
	std::unique_ptr <CComboBox> m_pCostType{};
	std::unique_ptr <CfxEdit> m_pCost{};

	std::unique_ptr <CModifyGrid> m_pModifyGrid{};
	std::unique_ptr <CContractGrid> m_pContractGrid{};

	CStringArray m_arrSendData{};
	int m_nCurOrder{};


};

