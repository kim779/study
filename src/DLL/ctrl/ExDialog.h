#if !defined(AFX_EXDIALOG_H__8DAA7987_1D70_11D5_9141_00001CD7EE63__INCLUDED_)
#define AFX_EXDIALOG_H__8DAA7987_1D70_11D5_9141_00001CD7EE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "bitmap.h"

class AFX_EXT_CLASS CExDialog : public CDialog
{
	DECLARE_DYNAMIC(CExDialog)

public:
	CExDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CExDialog();

	//{{AFX_VIRTUAL(CExDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void	ChangeHangulMode(HWND hWnd);		// 한글 모드로
	void	ChangeEnglishMode(HWND hWnd);		// 영문 모드로

	void	SetBackground(COLORREF rgb);
	BOOL	SetBackground(CString path);
	BOOL	SetBackground(HBITMAP& hBitmap);

	void	GetBmpSize(int& width, int& height);

	void	SetWinPos(CRect wRect);
	void	SetWinPos(CPoint pos, CRect wRect);
	void	SetWinPos(int wx, int wy, CRect wRect);

	void	ReSizeWnd(UINT *idcX, int cnt, double hRatio, double vRatio);

protected:
	COLORREF	m_crBack;
	CBrush		m_bkBrush;
	bool		m_bColor;

	CBrush		m_HollowBrush;
	CExBitmap	m_Background;
	bool		m_bFile;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_EXDIALOG_H__8DAA7987_1D70_11D5_9141_00001CD7EE63__INCLUDED_)
