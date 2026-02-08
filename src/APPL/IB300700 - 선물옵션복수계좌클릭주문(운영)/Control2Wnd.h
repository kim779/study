#pragma once

#include "Common.h"
#include "dataio.h"
#include "AxisExt.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxImgButton.h"
#include "grid/GridCtrl.h"

#define IDC_JVOL_MADO		(1001)
#define IDC_JVOL_MASU		(1002)
#define	alLEFT		1

class CControl2Wnd : public CWnd, public CAxisExt
{
public:
	CControl2Wnd(CWnd *pParent, AccountMap *pAccount, SiseData *pSise);
	void SetJvolPrice(int jprc);
	void ChangeTheme();
	void Clear();

protected:
	AccountMap *m_pAccount;
	SiseData *m_pSiseData;
protected:
	CBitmap m_hBk;

protected:
	CBitmap *m_hBtn2[3];
	CfxStatic *m_pStJvol[3], *m_pStJngo, *m_pStMicheg, *m_pStRet;
	CGridCtrl *m_pJvolGrid, *m_pJngoGrid, *m_pMichGrid, *m_pRetGrid;
	CfxImgButton *m_pJvolBtn[2];

protected:
	void MakeBackground( int width=-1, int height=-1 );
	void Draw(CDC *pdc);
	void CalcSize(int width=-1, int height=-1);
	void SetJvolGrid();
	void SetJngoGrid();
	void SetMichGrid();
	void SetRetGrid();
	CfxImgButton* CreateImgButton(LPCSTR caption, int nID, CBitmap **bmp);
	void res_SONBQ101(LPCSTR data, int dlen);
	void proc_jngo(Jango *pj);
	void proc_jngo_all();
	void proc_micheg_all();
	void proc_micheg(int sig, Order *pOdr);
	void proc_order(OrderResult* pOr);
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControl2Wnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControl2Wnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControl2Wnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnJvolBtnClick(UINT nID);
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
};

