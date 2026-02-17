#if !defined(AFX_FMTAB_H__F3C2623C_680F_4E4D_A0E3_68BBC9772B15__INCLUDED_)
#define AFX_FMTAB_H__F3C2623C_680F_4E4D_A0E3_68BBC9772B15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmTab.h : header file
//

#include "fmbase.h"
#include <afxtempl.h>

#define	TAB_EXTRA	14
#define	TAB_EXTRA2	25

/////////////////////////////////////////////////////////////////////////////
// CfmTab command target

class AFX_EXT_CLASS CfmTab : public CfmBase
{
	DECLARE_DYNCREATE(CfmTab)

public:
	CfmTab();	           // protected constructor used by dynamic creation
	CfmTab(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmTab();

// Attributes
protected:
	int		m_count;
	int		m_start;
	int		m_last;
	int		m_index;
	bool		m_scroll;

	COLORREF	m_fpRGB;
	COLORREF	m_ftRGB;

	CStringArray	m_caption;

	CRect		m_scr;
	CRect		m_cell;
	enum	{ ckNone=0, ckTab, ckLeft, ckRight } m_click;
	enum	{ mvNone=0, mvTab, mvScroll, mvEdit } m_move;
	int		m_imove;

private:
	int		m_tabH;
	int		m_otabH;
	CObArray	m_pages;
	CMap		< int, int, CBitmap*, CBitmap* > m_images;
	class	Cimage*	m_scrBM;

	int		m_leftW;
	int		m_rightW;

// Operations
public:
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	int	IsPointInRect(CPoint pt);
	void	SetFont(int point, int style, CString fonts, bool resize);
	
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);
	bool	OnDrop(CPoint pt, CString Data, CString& key);

	void	SetFgColor(int rgb, int col = -1, int row = -1);
	void	SetBkColor(int rgb, int col = -1, int row = -1);

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int size = -1, int index = -1, int row = -1);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int index = -1, int row = -1);
	
	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();
	int	GetSize();

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);

	bool	IsFormVisible(int index);
	void	Refresh();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmTab)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	drawTab(CDC* dc, CRect pRc, bool scroll);
	void	drawPage(CDC* dc, CRect tabRc, CRect rect, bool left);
	void	drawScroll(CDC* dc, CRect scrRc);
	bool	isScrolled(CDC* dc, int& index, int& width, int start = -1);
	int	getSize(CDC* dc, int index, bool focus = false);
	void	scroll(bool prev = true);
	void	loadBitmap(CString theme = _T(""));
	void	loadABitmap();

	// Generated message map functions
	//{{AFX_MSG(CfmTab)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmTab)
	afx_msg BSTR _getText();
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg long _getIndex();
	afx_msg void _setIndex(long nNewValue);
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg void _Refresh();
	afx_msg void _Edit();
	afx_msg BSTR _GetText(long index);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMTAB_H__F3C2623C_680F_4E4D_A0E3_68BBC9772B15__INCLUDED_)
