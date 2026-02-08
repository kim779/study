#if !defined(AFX_TINFO3_H__6C29F900_B096_4FF0_9F66_1003411D65D8__INCLUDED_)
#define AFX_TINFO3_H__6C29F900_B096_4FF0_9F66_1003411D65D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TInfo3.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CTInfo3 window

#define DEFAULT_COLS	3

#include "usedefine.h"
class CNButton;

class AFX_EXT_CLASS CTInfo3 : public CDialogBar
{
// Construction
public:
	CTInfo3(bool (*callback)(int, WPARAM, LPARAM));

// Attributes
protected:
	int		m_height;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTInfo3)
	//}}AFX_VIRTUAL

// Implementation
public:
	void change_Palette(CString skinN);
	virtual ~CTInfo3();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	//{{AFX_MSG(CTInfo3)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	LONG OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CBitmap* m_bmpBack;
	CBitmap	 m_bitmap;
	

	void MakeBaseBitmap(CDC* pDC);
private:
	
	int		m_cols;
	int		m_col;
	int		m_buttonSize;

	int		m_blinkInterval;
	int		m_blinkStep;
	COLORREF m_blinkColor;

	CArray<CString, CString> m_datas;
	CArray<CString, CString> m_acnts;

	CNButton* m_btnClose;
	//CToolTipCtrl* m_ToolTip2;
	CNButton* CreateNButton(unsigned int id, const char* title, const char* button, BOOL eachButton);

	void	DrawData(CDC* dc, int col);
	CRect   GetCellRect(int col);
	CString Comma(CString value);
public:
	void InitToolTip();
	void SetToolTipText(CString text, BOOL bActivate = TRUE);
	int		GetCols();
	void	SetCols(int cols = DEFAULT_COLS);
	void	make_Ctrl();
	BOOL	AddData(CString data, BOOL bFail);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TINFO3_H__6C29F900_B096_4FF0_9F66_1003411D65D8__INCLUDED_)
