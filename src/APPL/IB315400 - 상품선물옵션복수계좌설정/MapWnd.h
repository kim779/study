#if !defined(AFX_MAPWND_H__6014506F_5F80_4C3E_87E8_FE092FB72753__INCLUDED_)
#define AFX_MAPWND_H__6014506F_5F80_4C3E_87E8_FE092FB72753__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapWnd.h : header file
//

#include <vector>
#include <algorithm>
#include <functional>
#include "../../control/fx_misc/fxImgButton.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxEdit.h"
#include "../../control/fx_GridEx/UGCtrl.h"
#include "NumSpinCtrl.h"
#include "GridEx.h"

typedef struct _GROUP_INFO {
	CString GroupId;
	int Accounts;
} GROUP_INFO;

typedef std::vector<GROUP_INFO> GroupVector;
typedef GroupVector::iterator GroupVectorIter;

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

class CMapWnd : public CWnd
{
	// Construction
public:
	CMapWnd(CWnd* pWizard);

	// Attributes
private:

public:

	// Operations
public:
	void Init();
	CString Variant(int nComm, CString strData = _T(""));
	CString Parser(CString& srcstr, CString substr = _T("\t"));
	CFont* GetAxFont(CString strName, int nPt, bool bItalic, int nBold);
	COLORREF GetIndexColor(UINT nIndex);
	inline CBitmap* GetAxBitmap(CString strPath) { return (CBitmap*)m_pWizard->SendMessage(WM_USER, getBITMAP, (long)(LPCTSTR)strPath); };

private:
	UINT GetGroupAccounts(CString GroupId);
	void SetGroupAccounts(CString GroupId, UINT cnt);
	void QueryGroupList();
	void SetAccountList();
	BOOL SendTR(CString strTR, CString strData, int nKey, int nStat);
	void ParsingGroupList(CString strData, int nLen, CString strMsg);
	void ParsingGroupAccount(CString strData, int nLen, CString strMsg);
	void ClearSelGrid();
	int FindGroup(CString strID);
	void QueryGroupAccount();


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CMapWnd)
		//}}AFX_VIRTUAL

	// Implementation
public:
	CString GetPalette();
	HBITMAP getBitmap(CString bmps);
	void SetBitmap();
	CString m_strPal;
	void ClearGroupGrid();
	void ParsingTrigger(CString strTrigger);
	void OnRowChange(int oldRow, int newRow, CWnd* pWnd);
	void OnDClicked(int col, int row, RECT* rect, POINT* point, CWnd* pWnd);
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
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
	CString pre_id;
	int selRow;
	void QueryGroupAMD(CString strMode, CString strAccount, CString strCode, CString strRate, CString strRate2 = "0.00");
	CWnd* m_pWizard;

	CfxStatic* m_pAccTitle;
	CfxStatic* m_pGroupTitle;
	CfxStatic* m_pSelTitle;
	CfxStatic* m_panel;
	HBITMAP m_hBmpRoundPanel[4];
	HBITMAP m_hBmpRoundContents[4];

	CGridEx* m_pAccGrid;
	CGridEx* m_pGroupGrid;
	CGridEx* m_pSelGrid;

	CfxImgButton* m_pLtoR;

	CfxStatic* m_pIDStatic;
	CfxEdit* m_pID;
	CfxStatic* m_pNameStatic;
	CfxEdit* m_pName;
	CfxImgButton* m_pAddModify;
	CfxImgButton* m_pGroupDelete;
	//	CfxImgButton* m_pGroupUp;
	//	CfxImgButton* m_pGroupDown;

	CfxStatic* m_pAccStatic;
	CfxStatic* m_pAccount;
	CfxStatic* m_pRateStatic;
	CfxEdit* m_pRate;
	CNumSpinCtrl* m_pRateSpin;

	CfxImgButton* m_pSelModify;
	CfxImgButton* m_pSelDelete;
	//	CfxImgButton* m_pSelUp;
	//	CfxImgButton* m_pSelDown;

	CfxImgButton* m_pDefault;
	CfxImgButton* m_pOk;
	CfxImgButton* m_pCancel;
	CfxImgButton* m_pApply;

	CString m_strRoot;
	CFont* m_pNormalFont;
	CFont* m_pBoldFont;
	CPen* m_pLinePen;

	GroupVector m_grpVector;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPWND_H__6014506F_5F80_4C3E_87E8_FE092FB72753__INCLUDED_)
