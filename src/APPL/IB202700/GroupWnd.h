#pragma once
// GroupWnd.h : header file
//

#include "../../control/fx_misc/fxImgButton.h"
#include <afxmt.h>
#include "../../h/axisfire.h"
#include "AxisExt.h"
#include "GridWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CGroupWnd window

class CGroupWnd : public CWnd, public CAxisExt
{
// Construction
public:
	CGroupWnd(CWnd* pMainWnd, int nIndex);
// Attributes
public:

private:
	std::unique_ptr<CGridWnd>	m_pGridWnd;
	CWnd	*m_pMainWnd;
	int		m_nIndex;
	int		m_nGridIndex;	//몇번째 그리드인지.
	int		m_nGroupIndex;	//몇번째 그룹을 보여주고 있는지.
	CRect	m_rcTitle, m_rcRect;
	bool	m_bSelect;
	bool	m_bLBtnDown;

	CString		m_root, m_user, m_fileCFG, m_strTag, m_szTitle;
	COLORREF	m_clrFOCUS[2];	// 0 : focus 1 : normal	
	COLORREF	m_clrTEXT[2];	// 0 : focus 1 : normal

	CfxImgButton	m_btSAVE;	// 관심종목으로 저장
	CfxImgButton	m_btCLOSE;	// 화면 닫기

// Operations
public:
	void	InitGrid();
	int	GetGroupIndex(){ return m_pGridWnd->GetGroupIndex(); }
	int	GetGridIndex(){ return m_nIndex; }
	void	queryCodes(CString strGroup, CString strGroupName);
	void	SetSelect(bool bSelect){ m_bSelect = bSelect; }
	CWnd*	GetMainWnd(){ return GetParent(); }
	void	DrawTitle(BOOL bFocus = FALSE);
	void	DrawSelect(BOOL bFocus = FALSE);
	void	OperResize(int cx, int cy);
	void	FieldSetup(BOOL bSetup);
	void	RecvOper(int kind, CRecvData* rdata);
	bool    clear();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupWnd)
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGroupWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGroupWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

