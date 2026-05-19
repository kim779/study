#if !defined(AFX_AMCOMBO_H__63D2B440_2203_40AF_BEE9_9D212926B251__INCLUDED_)
#define AFX_AMCOMBO_H__63D2B440_2203_40AF_BEE9_9D212926B251__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmCombo.h : header file
//

#include "ambase.h"

/////////////////////////////////////////////////////////////////////////////
// CamCombo command target

class AFX_EXT_CLASS CamCombo : public CamBase
{
	DECLARE_DYNCREATE(CamCombo)

public:
	CamCombo();	           // protected constructor used by dynamic creation
	CamCombo(CAmForm* axform, struct _formR* formR);
	virtual ~CamCombo();

// Attributes
public:
	class	CcbList* m_list;

protected:
	int		m_index;
	bool	m_changed;		// Changed Check
	bool	m_spreaded;
	enum	{ ckNone=0, ckOrigin, ckCombo } m_click;
	enum	{ mvNone=0, mvIn, mvCombo } m_move;

	int	m_count;		// Entry Count
	int	m_vcount;

	CRect	m_rBtn;			// Combo Scroll Button Rect.
	CRect	m_rItem;		// Combo Item List Rect.

	CStringArray	m_datas;
	CStringArray	m_displays;

	bool	m_editMode;
	bool	m_caret;
	bool	m_focusing;
	int	m_caretpos;
	enum	{ esNone=0, esReverse, esClick, esNormal } m_state;
	CString	m_data;
	CPoint	m_point;
	CString	m_edit;

	int	m_csRGB;		// Combo Scroll Color
	int	m_cbRGB;		// Combo Back Color

	class	Cimage*	m_pic;
	int	m_cbW;

// Operations
public:
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	void	OnDblClick(CPoint pt, int& result);
	int		OnDrag(CPoint pt, int& result);
	int		OnKey(int key, int& result);

	void	Adjust(CPoint pt, float hRatio, float vRatio);
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Reload();
	
	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	void	ReadAll(CString& str, bool edit = false, bool head = false);

	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	UpdateData(int key, bool moving, int& result);
	void	InsertData(int key, bool moving, int& result);

	int		GetLength();
	bool	IsChanged(bool reset = true);
	void	ClearData(bool selectOnly = false);
	
	void	SetFocus(bool focus);
	int		IsPointInRect(CPoint pt);

	CRect	GetRect(bool original = false);
	void 	_SetEnable(bool enable);
	void	SetCurSel(int index, bool redraw = true);
	int		GetCurSel();

	void	SetEnum(CString text, int value, int func = -1);

	void	Refresh();
	void	DoListBox(bool apply, bool focus = false);
	bool	IsFocus(HWND hWnd);

	LPCTSTR	GetText();
	void	SetText(LPCTSTR lpszNewValue);
	LPCTSTR	GetData();
	void	SetData(LPCTSTR lpszNewValue);
	long	GetPRgb();
	void	SetPRgb(long nNewValue);
	long	GetTRgb();
	void	SetTRgb(long nNewValue);
	bool	GetVisible();
	void	SetVisible(bool bNewValue);
	bool	GetEnable();
	void	SetEnable(bool bNewValue);
	long	GetIndex();
	void	SetIndex(long nNewValue);
	bool	GetSkip();
	void	SetSkip(bool bNewValue);
	
	void	_SetFocus();
	void	_Clear();
	void	_Refresh();
	void	_Push(LPCTSTR name);
	void	_Pop(LPCTSTR name);
	void	_SetItems(LPCTSTR items);
	void	_ShowDropDown(BOOL showIt);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamCombo)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	draw(CDC* dc);
	void	lbutton(bool down, CPoint pt, int& result);
	void	updateData(int key, bool moving, int& result);

	bool	makeListBox();
	CRect	setRect(CDC* dc);
	void	clearInputData();
	void	editData();
	int		calcCaretPos();

	void	keyDelete(int& result);
	void	keyBackspace(int& result);
	void	keyReturn(int& result);
	void	keyEscape(int& result);
	int		keyArrowLeft(int& result);
	void	keyArrowRight(int& result);
	void	keyArrowDown(int& result);

	// Generated message map functions
	//{{AFX_MSG(CamCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamCombo)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	void StrParser(int nCnt, CString strDisp, CString strData);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMCOMBO_H__63D2B440_2203_40AF_BEE9_9D212926B251__INCLUDED_)
