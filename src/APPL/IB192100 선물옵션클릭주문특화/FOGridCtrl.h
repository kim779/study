#pragma once

#include "Common.h"
#include "AxisExt.h"
#include "GridCtrlExt.h"
#include <map>
#include <set>

using namespace std;

#define COLUMN_SIZE		(10)
#define DEF_ROW_SIZE	(1000)

/*
typedef enum {
	IDX_ASK_RSVD = 0,
	IDX_ASK_JUMUN,
	IDX_ASK_BEFORE,
	IDX_ASK_SIZE,
	IDX_HOGA,
	IDX_RATE,
	IDX_BID_SIZE,
	IDX_BID_BEFORE,
	IDX_BID_JUMUN,
	IDX_BID_RSVD
} FOGRID_IDX;
*/

bool operator<(const CCellID &lsh, const CCellID &rsh);
bool operator<(CCellID &lsh, CCellID &rsh);

class CFOGridCtrl : public CGridCtrlExt, public CAxisExt
{
// Construction
public:
	CFOGridCtrl(CWnd *pParent);

public:
	int IDX_ASK_RSVD  ;
	int IDX_ASK_JUMUN ;
	int IDX_ASK_BEFORE;
	int IDX_ASK_SIZE  ;
	int IDX_HOGA      ;
	int IDX_RATE      ;
	int IDX_BID_SIZE  ;
	int IDX_BID_BEFORE;
	int IDX_BID_JUMUN ;
	int IDX_BID_RSVD  ;

public:
	void SetGridHeader();
	void SetGridBody();
	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE , DWORD dwExStyle = 0);
// Operations
public:
	void SetReverseMode(BOOL bReverse = FALSE);
	int GetRequireWidth();
	void SetWidths(int rsvd, int jumun, int cnt, int vol, int hoga, int rate);
	void GetWidths(int *rsvd, int *jumun, int *cnt, int *vol, int *hoga, int *rate);

	void ShowRate(bool bShowRate);
	void ShowBefore(bool bShowBefore);
	void ShowRsvd(bool bShowRsvd);

	BOOL IsShowRate() { return m_bShowRate; }
	BOOL IsShowBefore() { return m_bShowBefore; }
	BOOL IsShowRsvd() { return m_bShowRate; }

	void SetData(SiseData *psd);
	void SetRealData(RealData* rp);
	void SetRealData(DWORD* data);
	void SetCurrToCenter();
	void SetMdgaToCenter();
	void Clear();
	void SetMicheg(PriceMichegMap *pmm);
	BOOL IsMouseOut() { return m_bOutMouse; }
	void CheckExpand();
	void SetHogaFix(BOOL bFixed);

protected:
	int m_pdir;
	CCellID m_pid;

	COLORREF m_crNormalBk, m_crYellowBk;
	CCellID m_CurCell;
	bool m_bShowBefore, m_bShowRate, m_bShowRsvd, m_bExpandHoga;

	BOOL m_bOutMouse, m_bHogaFix, m_bReverseMode;
	CRect m_rcClient;

	set<CCellID> m_JumnCells;
	int m_MdgaCell[MAX_FHOGA], m_MsgaCell[MAX_FHOGA];
	map<int, int> m_mapHogaRow;
	LOGFONT m_BLogFont;

	int m_HeadWidth[COLUMN_SIZE];
	LPCSTR m_HeadText[COLUMN_SIZE];
	LPCSTR m_RHeadText[COLUMN_SIZE];
protected:
	SiseData *m_pSiseData;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFOGridCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFOGridCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFOGridCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

