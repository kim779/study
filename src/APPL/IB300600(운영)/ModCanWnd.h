#pragma once
// ModCanWnd.h : header file
//

#include "AccountCtrl.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "ModCanGrid.h"

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
	void Init();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModCanWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPallete();
	void SetInputFieldFocus();
	void ParsingFOrder(CString strData, int nLen, CString strMsg);
	void SendOrder(BOOL bModify);
	void ParsingFOCHoga(CString strData, BOOL bHoga);
	inline BOOL IsBlocking() { return m_bBlock; };
	inline CWnd* GetMapWnd() { return (CWnd*)m_pParent; };
	void QueryFOCHoga(BOOL bHoga, CString strCode = _T(""), CString strState = _T(""));
	CString Parser(CString &srcstr, CString substr);
	void ParsingNonCont(CString strData, int nLen, CString strMsg);
	inline CWnd* GetAccountCtrl() { return (CWnd*)m_pAccount; };
	virtual ~CModCanWnd();
	void ClearModCanGrid();
	void OnAlert(CString altmsg);

	// Generated message map functions
protected:
	//{{AFX_MSG(CModCanWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg void OnNonCont();
	afx_msg void OnModify();
	afx_msg void OnCancelMode();
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSelchangeModCanPriceType();
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
	CfxStatic* m_pModCanPriceStatic;
	CfxEdit* m_pPassword;
	CComboBox* m_pModCanPriceType;
	CfxImgButton* m_pNonCont;
	CfxImgButton* m_pModify;
	CfxImgButton* m_pCancel;
	CfxImgButton* m_pExpand;
	CfxImgButton* m_pContract;

	CModCanGrid*	m_pModCanGrid;

	BOOL m_bBlock;
};

/////////////////////////////////////////////////////////////////////////////

