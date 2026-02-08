#pragma once

#include "exGrid.h"
#include "../../h/interst.h"

class CCatchWnd : public CWnd
{
public:
	CCatchWnd(CWnd *pParent, CWnd *pView);
	virtual ~CCatchWnd();

	//{{AFX_VIRTUAL(CCatchWnd)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CCatchWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	static void RegisterWindowClass();

	BOOL	CreatePopup();
	void	Show(CPoint pt);
	void	Hide();
	void	SetFont(CFont *pFont, LOGFONT lf) { m_pFont = pFont; m_lf = lf; }
	void	SetBFont(CFont *pBFont, LOGFONT blf) { m_pBFont = pBFont; m_blf = blf; }
	void	SetData(_codeinfo *codeinfo);
private:
	void	initConstruct();
	void	destroy();
	BOOL	detach();
	
	void	initControl();
	void	initGrid(CExGrid* pGrid, int idc);
	int	insertGrid(CExGrid* pGrid, UINT idc, int row);
	void	setGridItem(int col, _GVITEM *pGvitem, CString text);
	
private:
	CWnd*	m_pParent;
	CWnd*	m_pView;
	std::unique_ptr<class	CExGrid>	m_pAlarm;
	std::unique_ptr<class	CExGrid>	m_pEtc;
	std::unique_ptr<class	CAxStatic>	m_pName;
	CFont* m_pFont{};	LOGFONT	m_lf{};
	CFont* m_pBFont{};	LOGFONT	m_blf{};
	_codeinfo* m_codeinfo{};
	COLORREF	m_clrBk{};
	int	m_width{};
};

