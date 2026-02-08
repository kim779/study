#pragma once
// MapWnd.h : header file
//

#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_GridEx/UGCtrl.h"
#include "NumSpinCtrl.h"
#include "GridEx.h"

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* pWizard);

// Attributes
public:
	HBITMAP				m_hRoundBitmap[10]{};
	int					m_iHeight=0;

// Operations
public:
	void Init();
	CString Variant(int nComm, CString strData = _T(""));
	CString Parser(CString &srcstr, CString substr);
	CFont* GetAxFont(CString strName, int nPt, bool bItalic, int nBold);
	COLORREF GetIndexColor(UINT nIndex);
	inline CBitmap* GetAxBitmap(CString strPath) 
			{ return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (long)(LPCTSTR)strPath); };

private:
	void QueryGroupList();
	void SetAccountList();
	BOOL SendTR(CString strTR, CString strData, int nKey, int nStat);
	void ParsingGroupList(CString strData, int nLen, CString strMsg);
	void ParsingGroupAccount(CString strData, int nLen, CString strMsg);
	void ClearSelGrid();
	int FindGroup(CString strID);
	void ParsingGroupAMD(CString strData, int nLen, CString strMsg);
	void QueryGroupAccount();

	void LoadRoundBitmap();
	void DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);
	void DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType=0);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnRowChange(int oldRow, int newRow, CWnd* pWnd);
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLtoR();
	afx_msg void OnAddModify();
	afx_msg void OnGroupDelete();
	afx_msg void OnSelModify();
	afx_msg void OnSelDelete();
	afx_msg void OnOk();
	DECLARE_MESSAGE_MAP()

private:
	void QueryGroupAMD(int nMode);
	CWnd* m_pWizard{};

	std::unique_ptr <CfxStatic> m_pAccTitle;
	std::unique_ptr <CfxStatic> m_pGroupTitle;
	std::unique_ptr <CfxStatic> m_pSelTitle;

	std::unique_ptr <CGridEx> m_pAccGrid;
	std::unique_ptr <CGridEx> m_pGroupGrid;
	std::unique_ptr <CGridEx> m_pSelGrid;

	std::unique_ptr <CfxImgButton> m_pLtoR;

	std::unique_ptr <CfxStatic> m_pIDStatic;
	std::unique_ptr <CfxEdit> m_pID;
	std::unique_ptr <CfxStatic> m_pNameStatic;
	std::unique_ptr <CfxEdit> m_pName;
	std::unique_ptr <CfxImgButton> m_pAddModify;
	std::unique_ptr <CfxImgButton> m_pGroupDelete;
//	CfxImgButton* m_pGroupUp;
//	CfxImgButton* m_pGroupDown;

	std::unique_ptr <CfxStatic> m_pAccStatic;
	std::unique_ptr <CfxStatic> m_pAccount;
	std::unique_ptr <CfxStatic> m_pRateStatic;

	std::unique_ptr <CfxEdit> m_pRate;
	std::unique_ptr <CNumSpinCtrl> m_pRateSpin;

	std::unique_ptr <CfxImgButton> m_pSelModify;
	std::unique_ptr <CfxImgButton> m_pSelDelete;
//	CfxImgButton* m_pSelUp;
//	CfxImgButton* m_pSelDown;

	std::unique_ptr <CfxImgButton> m_pDefault;
	std::unique_ptr <CfxImgButton> m_pOk;
	std::unique_ptr <CfxImgButton> m_pCancel;
	std::unique_ptr <CfxImgButton> m_pApply;

	CString m_strRoot;
	CFont* m_pNormalFont{};
	CFont* m_pBoldFont{};
	std::unique_ptr<CPen> m_pLinePen;
};

