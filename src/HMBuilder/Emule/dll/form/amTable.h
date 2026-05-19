#if !defined(AFX_AMTABLE_H__27352169_8659_4526_BBFE_A8C9C9A6F3FD__INCLUDED_)
#define AFX_AMTABLE_H__27352169_8659_4526_BBFE_A8C9C9A6F3FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmTable.h : header file
//

#include <afxtempl.h>
#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamTable command target

class Cpair
{
public:
	Cpair()			{}
	virtual ~Cpair()	{}

public :
	CRect		m_rect;
	WORD		m_widthx;
	float		m_width;
	CString		m_head;

	COLORREF	m_htRGB;	// head text RGB
	COLORREF	m_hpRGB;	// head paint RGB

	COLORREF	m_pRGB;
	COLORREF	m_tRGB;
	int			m_itRGB;

	CString		m_edit;
	int		m_rts;
};


class AFX_EXT_CLASS CamTable : public CamBase
{
	DECLARE_DYNCREATE(CamTable)

public:
	CamTable();	           // protected constructor used by dynamic creation
	CamTable(CAmForm* axform, struct _formR* formR, /*struct _cellR* cellR*/vector <struct _cellR*>* = NULL);
	virtual ~CamTable();

// Attributes
protected:
	//struct	_cellR*	m_cellR;
	vector <struct _cellR*>* m_cellR;
	int	m_index;

	int	m_showRows;
	int	m_totRows;

	CArray	< Cpair*, Cpair* >	m_pairs;
	CMap	< DWORD, DWORD, CString, CString& > m_names;

	enum	{ ckNone=0, ckHead, ckItem }	m_click;
	enum	{ shBoth=0, shHead, shItem }	m_show;
	double	m_cellH;
	float	m_cy;			// cell height

	CStringArray	m_data;
	CStringArray	m_edit;
	int	m_slit;
	bool	m_calc;

	CPoint	m_point;
	enum	MSTATE	{ mvNone=0, mvMove, mvDrag, mvSel } m_move;
	CRect	m_moveRc;
	int	m_delta;

	CRect	m_cell;
	Cpos	m_curpos;
	Cpos	m_cellpos;
	Cpos	m_lastpos;
	bool	m_select;

	bool	m_hasfocus;

	CString m_strResult;

// Operations
public:
	CString	GetSymbolName(int col = -1);
	int		GetName(int col = -1);
	void	Adjust(CPoint pt, float hRatio, float vRatio);
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Reload();
	void	Draw(CDC* dc);

	void	OnLButton(bool down, CPoint pt, int& result);
	void	OnRButton(bool down, CPoint pt, int& result);
	void	OnDblClick(CPoint pt, int& result);
	int		OnDrag(CPoint pt, int& result);

	void	SetFgColor(int rgb, int col = -1, int row = -1);
	void	SetBkColor(int rgb, int col = -1, int row = -1);
	int		GetFgColor(int col = -1, int row = -1);
	int		GetBkColor(int col = -1, int row = -1);
	void	SetDomino(bool redraw = true);

	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	void	SetFocus(bool focus);
	void	SetCellRow(int row);
	int		GetCellRow();
	int		GetValidRows();

	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();

	int	GetSize();
	
	
	LPCTSTR	GetText();
	LPCTSTR	GetData();
	void	SetData(LPCTSTR lpszNewValue);
	LPCTSTR	GetCell();
	void	SetCell(LPCTSTR lpszNewValue);
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	bool	GetVisible();
	void	SetVisible(bool bNewValue);
	bool	GetEnable();
	void	SetEnable(bool bNewValue);
	long	GetRow();
	void	SetRow(long nNewValue);
	LPCTSTR	GetHead();
	void	SetHead(LPCTSTR lpszNewValue);
	long	GetHitPos();
	
	void	_Refresh();
	LPCTSTR	_GetCell(long row);
	void	_SetCell(long row, LPCTSTR cells);
	LPCTSTR	_GetText(long row);	
	long	_GetPRgb(long row);
	void	_SetPRgb(long row, long rgb);
	long	_GetTRgb(long row);
	void	_SetTRgb(long row, long rgb);
	LPCTSTR	_GetHead(long row);
	void	_SetHead(long row, LPCTSTR head);
	void	_Push(long row, LPCTSTR name);
	void	_Clear();
	void	_Excel(bool run, bool head, LPCTSTR path);
	void	_SetSkip(LPCTSTR cname, bool skip);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamTable)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString	editData(CString str, int index);
	void	makeSignData(CString& data, int row = -1, int col = -1);
	void	doDataEditCORGB(int index, CString& data);

	int		keyArrowLeft(int& result);
	void	keyArrowRight(int& result);
	void	keyArrowUp(int& result);
	void	keyArrowDown(int& result);
	void	keyEscape(int& result);

	bool	isReverseCell(int row, int col);
	void	refreshSel(Cpos pos, int key);
	bool	findCell(CPoint pt, int& row, int& col, CRect& hRc, CRect& vRc);
	int		findCell(CString name);

	// Generated message map functions
	//{{AFX_MSG(CamTable)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamTable)
	
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMTABLE_H__27352169_8659_4526_BBFE_A8C9C9A6F3FD__INCLUDED_)
