#if !defined(AFX_FMTABLE_H__27352169_8659_4526_BBFE_A8C9C9A6F3FD__INCLUDED_)
#define AFX_FMTABLE_H__27352169_8659_4526_BBFE_A8C9C9A6F3FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmTable.h : header file
//

#include <afxtempl.h>
#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmTable command target

class Cpair
{
public:
	Cpair()			{}
	virtual ~Cpair()	{}

public :
	CRect		m_rect;
	WORD		m_widthx = 0;
	float		m_width = 0;
	CString		m_head = _T("");

	COLORREF	m_htRGB = 0;	// head text RGB
	COLORREF	m_hpRGB = 0;	// head paint RGB

	COLORREF	m_pRGB = 0;
	COLORREF	m_tRGB = 0;
	int		m_itRGB = -1;

	CString		m_edit = _T("");
	int		m_rts = 0;
};


class AFX_EXT_CLASS CfmTable : public CfmBase
{
	DECLARE_DYNCREATE(CfmTable)

public:
	CfmTable();	           // protected constructor used by dynamic creation
	CfmTable(CAxisForm* axform, struct _formR* formR, int iform, struct _cellR* cellR = NULL);
	virtual ~CfmTable();

// Attributes
protected:
	struct	_cellR*	m_cellR;
	int	m_index;

	int	m_showRows;
	int	m_totRows;

	CArray	< Cpair*, Cpair* >	m_pairs;

	enum	{ ckNone=0, ckHead, ckItem }	m_click;
	enum	{ shBoth=0, shHead, shItem }	m_show;
	int	m_cellH;
	float	m_cy;			// cell height

	CStringArray	m_data;
	CStringArray	m_edit;
	int	m_slit;
	bool	m_calc;

	CPoint	m_point;
	enum	{ mvNone=0, mvMove, mvDrag, mvSel, mvHead, mvItem } m_move;
	CRect	m_moveRc;
	int	m_delta;
	Cpos	m_movepos;	// 0 base

	CRect	m_cell;
	Cpos	m_curpos;	// 1 base
	Cpos	m_cellpos;	// 1 base
	Cpos	m_lastpos;	// 1 base
	bool	m_select;

	bool	m_hasfocus;

// Operations
public:
	int	GetName(int col = -1);
	void	Adjust(CPoint pt, float hRatio, float vRatio);
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Reload();
	void	Draw(CDC* dc);

	void	OnLButton(bool down, CPoint pt, int& result);
	void	OnRButton(bool down, CPoint pt, int& result);
	void	OnDblClick(CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);

	void	SetFgColor(int rgb, int col = -1, int row = -1);
	void	SetBkColor(int rgb, int col = -1, int row = -1);
	int	GetFgColor(int col = -1, int row = -1);
	int	GetBkColor(int col = -1, int row = -1);
	void	SetDomino(bool redraw = true);

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);

	void	SetFocus(bool focus);
	void	SetCellRow(int row);
	int	GetCellRow();
	int	GetValidRows();

	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();

	int	GetSize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmTable)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString	editData(CString str, int index);
	void	makeSignData(CString& data, int row = -1, int col = -1);
	void	doDataEditCORGB(int index, CString& data);

	int	keyArrowLeft(int& result);
	void	keyArrowRight(int& result);
	void	keyArrowUp(int& result);
	void	keyArrowDown(int& result);
	void	keyEscape(int& result);

	bool	isReverseCell(int row, int col);
	void	refreshSel(Cpos pos, int key);
	bool	findCell(CPoint pt, int& row, int& col, CRect& hRc, CRect& vRc);

	// Generated message map functions
	//{{AFX_MSG(CfmTable)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmTable)
	afx_msg BSTR _getText();
	afx_msg BSTR _getData();
	afx_msg void _setData(LPCTSTR lpszNewValue);
	afx_msg BSTR _getCell();
	afx_msg void _setCell(LPCTSTR lpszNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg long _getRow();
	afx_msg void _setRow(long nNewValue);
	afx_msg BSTR _getHead();
	afx_msg void _setHead(LPCTSTR lpszNewValue);
	afx_msg long _getHitPos();
	afx_msg long _getRows();
	afx_msg long _getHeadWidth();
	afx_msg void _Refresh();
	afx_msg BSTR _GetCell(long row);
	afx_msg void _SetCell(long row, LPCTSTR cells);
	afx_msg BSTR _GetText(long row);
	afx_msg long _GetPRgb(long row, long column);
	afx_msg void _SetPRgb(long row, long column, long rgb);
	afx_msg long _GetTRgb(long row, long column);
	afx_msg void _SetTRgb(long row, long column, long rgb);
	afx_msg BSTR _GetHead(long row);
	afx_msg void _SetHead(long row, LPCTSTR head);
	afx_msg void _Push(long row, LPCTSTR name);
	afx_msg void _Clear();
	afx_msg void _Excel(BOOL run, BOOL head, LPCTSTR path);
	afx_msg void _SetSkip(LPCTSTR cname, BOOL skip);
	afx_msg long _GetDataKind(long row);
	afx_msg long _GetMode(long row);
	afx_msg long _GetLength(long row);
	afx_msg BSTR _GetFormat(long row);
	afx_msg void _SetText(long row, LPCTSTR text);
	afx_msg long _GetFloatDigit(long row);
	afx_msg void _SetFloatDigit(long col, long val);
	afx_msg BSTR _GetRowName(long row);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMTABLE_H__27352169_8659_4526_BBFE_A8C9C9A6F3FD__INCLUDED_)
