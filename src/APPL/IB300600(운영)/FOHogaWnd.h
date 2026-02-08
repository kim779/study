#pragma once

#include <set>
#include "AxisExt.h"
#include "FOGridCtrl.h"
#include "../../control/fx_misc/fxButton.h"
#include "BottomGrid.h"


#define	bsTEXT		0x00000001
#define	bsCOLOR		0x00000002
#define	bsIMAGE		0x00000004	// 1 IMAGE, size DeflateRect(2, 2)
#define	bsGRAD		0x00000008	// gradiation

#define	bsPUSH		0x00000010	// push button
#define	bsFLATE		0x00000020	// flate button
#define	bsTOGGLE	0x00000040	// toggle button
#define	bsRECT		0x00000080	// draw rect only
#define	bsGENERAL	0x00000100	// general button

#define IDC_GRID_HOGA			(1001)
#define IDC_GRID_BOTTOM			(1002)
#define IDC_BTN_MADO_RSVD		(1003)
#define IDC_BTN_MADO_JUMN		(1004)
#define IDC_BTN_MASU_RSVD		(1005)
#define IDC_BTN_MASU_JUMN		(1006)
#define IDC_BTN_HOGA_FIX        (1007)
#define IDC_BTN_MARK_MADO		(1008)
#define IDC_BTN_MARK_MASU		(1009)

typedef enum { MM_NONE, MM_DRAG_JUMN, MM_DRAG_RSVD } MOUSE_DRAG_MODE;

class CFOHogaWnd : public CWnd, CAxisExt
{
public:
	CFOHogaWnd(CWnd *pParent, AccountMap *pAcntMap, RsvdDataArray *pRsvdData);

public:
	SiseData *m_pSiseData;

	int GetRequireWidth();
	void ShowRate(bool bShowRate);
	void ShowBefore(bool bShowBefore);
	void ShowRsvd(bool bShowRsvd);
	void SetData(SiseData *psd);
	void SetRealData(RealData *rp);
	void SetRealData(DWORD* data);
	void SetBGridMichegData(int t_dcnt, int t_scnt, int t_dwqty, int t_swqty);
	void SetBGridRsvdData(int t_dcnt, int t_scnt, int t_dwqty, int t_swqty);
	void SetHogaFix(BOOL bFixed);
	void SetWidths(int rsvd, int jumun, int cnt, int vol, int hoga, int rate);
	void GetWidths(int *rsvd, int *jumun, int *cnt, int *vol, int *hoga, int *rate);
	
	BOOL GetExtOrderColumn() { return m_bExtOdrColumn; }
	void SetExtOrderColumn(BOOL bExtend = FALSE);
	
	BOOL GetSpaceOrder() { return m_bSpaceOrder; }
	void SetSpaceOrder(BOOL bEnable = FALSE);

	BOOL GetRClickCancel() { return m_bRClickCancel; }
	void SetRClickCancel(BOOL bEnable = FALSE);

	void SetOneClickMode(BOOL bOneClickOrder);
	void ShowOrderContextMenu(int nRow, int nCol);

	void SetFocusGrid();

	void ChangeTheme();
	void SetAverage( CCellID id, int tick );
	void RefreshAvearage();

public:
	BOOL m_bSiJangDBClick;	//2017.01.16 KSJ 시장가 더블클릭
	BOOL m_bKeyUPDOWN;      
protected:
	CTitleTip m_TitleTip;
	CFOGridCtrl *m_pGrid;
//	CGridCtrl *m_pBGrid;
	BottomGrid *m_pBGrid;

	CfxButton *m_pMadoRsvd, *m_pMadoJumn, *m_pMasuRsvd, *m_pMasuJumn, *m_pHogaFix, *m_pMarkMado, *m_pMarkMasu;

protected:
	AccountMap *m_pAccounts;
	RsvdDataArray *m_pRsvdData;
	set<HWND> m_HandCursorWnd;
	
	BOOL m_bMouseOut, m_bOneClickOrder, m_bExtOdrColumn, m_bSpaceOrder, m_bRClickCancel;
	CCellID m_DragStartId;
	MOUSE_DRAG_MODE m_MouseMode;
	HCURSOR m_csArrow, m_csHand;
	//CToolTipCtrl* m_pTooltip;

protected:
	void CalcSize();
	void SetBottomGrid();
	void SetBGridData(int dcnt, int dvol, int sdch, int svol, int scnt);
	void ShowOrderInfo(int row, int col);
	BOOL IsJumnCell(CCellID id);
	BOOL IsRsvdCell(CCellID id);
	void AddArrow(CCellID id, int tick);
	void DelArrow(CCellID id, int tick);

public:
	void SetPprcData(int idata);
	
	bool m_bDragDropOrder;
	bool m_bDragDropJung;
	int  m_iDragDropJungMS;
	int  m_iRowHeight;
	void SetHogaGridRowHeight();
	void ReDrawReserveArrow();
public:
	CWnd* GetHogaGrid()  { return m_pGrid; };
	BOOL  GetOneClickOrder() { return m_bOneClickOrder; };
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFOHogaWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFOHogaWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFOHogaWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRefresh(WPARAM wParam, LPARAM lParam);
	afx_msg void OnAllMadoCancel();
	afx_msg void OnAllMasuCancel();
	afx_msg void OnAllMadoRsvdCancel();
	afx_msg void OnAllMasuRsvdCancel();
	afx_msg void OnHogaFix();
	afx_msg LRESULT OnAppSignal(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMarketMado();
	afx_msg void OnMarketMasu();
	afx_msg void OnMarketMadoDBClick();
	afx_msg void OnMarketMasuDBClick();
};

