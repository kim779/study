#if !defined(AFX_FMCOMBO_H__63D2B440_2203_40AF_BEE9_9D212926B251__INCLUDED_)
#define AFX_FMCOMBO_H__63D2B440_2203_40AF_BEE9_9D212926B251__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmCombo.h : header file
//

#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmCombo command target

class AFX_EXT_CLASS CfmCombo : public CfmBase
{
	DECLARE_DYNCREATE(CfmCombo)

public:
	CfmCombo();	           // protected constructor used by dynamic creation
	CfmCombo(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly);
	virtual ~CfmCombo();

// Attributes
public:
	class	CcbList* m_list;

protected:
	int	m_index;
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

	ULONGLONG	m_elapse;
	bool	m_cc;		// combo change

	class	Cimage*	m_pic;

// Operations
public:
	void	Draw(CDC* dc);
	void	OnLButton(bool down, CPoint pt, int& result);
	void	OnDblClick(CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);
	int	OnKey(int key, int& result);

	void	Adjust(CPoint pt, float hRatio, float vRatio);
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Reload();
	
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	void	ReadAll(CString& str, bool edit = false, bool head = false);

	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	UpdateData(int key, bool moving, int& result);
	void	InsertData(int key, bool moving, int& result);

	int	GetLength();
	bool	IsChanged(bool reset = true);
	void	ClearData(bool selectOnly = false);
	
	void	SetFocus(bool focus);
	int	IsPointInRect(CPoint pt);

	CRect	GetRect(bool original = false);
	void 	SetEnable(bool enable);
	void	SetCurSel(int index, bool redraw = true);
	int	GetCurSel();

	void	SetEnum(CString text, int value, int func = -1);

	void	Refresh();
	void	DoListBox(bool apply, bool focus = false);
	bool	IsFocus(HWND hWnd);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmCombo)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	draw(CDC* dc);
	void	loadCombo();
	void	lbutton(bool down, CPoint pt, int& result);
	void	updateData(int key, bool moving, int& result);

	bool	makeListBox();
	CRect	setRect(CDC* dc, int height);
	void	clearInputData();
	void	editData();
	int	calcCaretPos();

	void	keyDelete(int& result);
	void	keyBackspace(int& result);
	void	keyReturn(int& result);
	void	keyEscape(int& result);
	int	keyArrowLeft(int& result);
	void	keyArrowRight(int& result);
	void	keyArrowDown(int& result);

	// Generated message map functions
	//{{AFX_MSG(CfmCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmCombo)
	afx_msg BSTR _getText();
	afx_msg BSTR _getData();
	afx_msg void _setData(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg BOOL _getEnable();
	afx_msg void _setEnable(BOOL bNewValue);
	afx_msg long _getIndex();
	afx_msg void _setIndex(long nNewValue);
	afx_msg BOOL _getSkip();
	afx_msg void _setSkip(BOOL bNewValue);
	afx_msg long _getSize();
	afx_msg void _setSize(long nNewValue);
	afx_msg BOOL _getFocus();
	afx_msg BOOL _getTabStop();
	afx_msg void _setTabStop(BOOL bNewValue);
	afx_msg void _SetFocus();
	afx_msg void _Clear();
	afx_msg void _Refresh();
	afx_msg void _Push(LPCTSTR name);
	afx_msg void _Pop(LPCTSTR name);
	afx_msg void _SetItems(LPCTSTR items);
	afx_msg void _ShowDropDown(BOOL showIt);
	afx_msg long _GetItemCount();
	afx_msg long _GetDataKind();
	afx_msg long _GetLength();
	afx_msg BSTR _GetItems();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMCOMBO_H__63D2B440_2203_40AF_BEE9_9D212926B251__INCLUDED_)
