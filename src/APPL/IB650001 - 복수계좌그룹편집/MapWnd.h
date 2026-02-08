#if !defined(AFX_MAPWND_H__5F99CFB2_9D6C_4D76_9AB9_7E544AA79B69__INCLUDED_)
#define AFX_MAPWND_H__5F99CFB2_9D6C_4D76_9AB9_7E544AA79B69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "common.h"
#include "AxisExt.h"
#include "EditVC.h"
#include "grid/GridCtrl.h"
#include "grid/NewCellTypes/GridCellNumeric.h"
#include "grid/NewCellTypes/GridCellPassword.h"
#include "../../control/fx_misc/fxImgButton.h"

#define MAX_GROUPLIST			(50)
#define MAX_GROUPACCOUNT		(40)

#define IDC_STATIC_CONTROL		(1000)
#define IDC_GRID_GROUPLIST		(1001)
#define IDC_GRID_GROUPACCOUNT	(1002)
#define IDC_GRID_REMAINACCOUNT	(1003)
#define IDC_EDIT_GROUPNAME		(1004)
#define IDC_BTN_GROUPUPDATE		(1005)
#define IDC_BTN_GROUPADD		(1006)
#define IDC_BTN_GROUPDELETE		(1007)
#define IDC_BTN_RELOAD			(1008)
#define IDC_BTN_LIMITSTOCK		(1009)

class CMapWnd : public CWnd, CAxisExt
{
// Construction
public:
	CMapWnd(CWnd *pParent);

// Attributes
protected:
	CString m_home, m_user, m_name;
	BOOL m_bLoading, m_bNewGroup;
	grop_data m_gdata;
	accn_data m_adata;
	vector<accn_info> m_owns;
	vector<accn_info> m_updateAcnn;	//2014.04.07 KSJ 업데이트한 리스트를 가지고 있다가 비밀번호가 틀리다고 하면 저장된 비번 지우기 위해
	map<CString,CString> m_pass;

	grop_info *m_pCurGroup, m_TempGroup;
	vector<accn_info> *m_pCurAccount, m_TempAccount;
	
protected:
	void MakeBackground();
	void DrawRoundRectangle(CDC *pDC, CRect rc, COLORREF crBk);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	CfxImgButton* CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp);
	void WriteLog(LPCSTR name, int dlen, LPCSTR data);
	void WritePLog(LPCSTR fmt, ...);

protected:
	CRect m_rcGroupList[2], m_rcGroupAccount[2], m_rcRemainAccount[2];
	CBitmap *m_hRndBmp[4], *m_hGuide, m_hBackground, *m_hBtn2[3], *m_hBtn4[3];
	COLORREF m_crEven, m_crOdd, m_crHeader, m_crSelect;

	// 0:GroupList, 1:GroupAccount, 2:RemainAccount
	int m_pGridSelectIdx[3];
	CGridCtrl *m_pGrid[3];
	CStatic m_stGroupName;
	CEdit m_edGroupName;
	CfxImgButton *m_btnGroupUpdate, *m_btnGroupAdd, *m_btnGroupDelete, *m_btnReload, *m_btnLimit;
	bool m_bAsc;

public:
	void Init();
	void InitGrid();
	void InitControl();

public:
	void LoadData();
	void LoadGroupAccount(LPCSTR gpno, LPCSTR gpnm, struct _ledgerH *nledger=NULL);
	BOOL IsLoadingComplete();
	void SelectGroup(int grid_idx);
	void ClearGrid(CGridCtrl *pGrid, BOOL bRepaint=TRUE);
	void SelectGridRow(CGridCtrl *pGrid, int idx);
	void RefreshRemainAccount();
	void AddGroupAccount(accn_info *ai);
	void DelGroupAccount(accn_info *ai);
	void UpdateGroup();
	void LoadOwnAccount();
	void ClearUsedStatus();

// Operations
public:
	void OnGroupList(LPCSTR data, int dlen);
	void OnGroupAccount(LPCSTR data, int dlen);
	void OnGroupUpdate(LPCSTR data, int dlen);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBtnNewGroup();
	afx_msg void OnBtnDelGroup();
	afx_msg void OnBtnReload();
	afx_msg void OnBtnLimit();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPWND_H__5F99CFB2_9D6C_4D76_9AB9_7E544AA79B69__INCLUDED_)
