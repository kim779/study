#if !defined(AFX_FMSHEET_H__69BEF9E0_8F70_4CAD_AD9B_A1B5187A9238__INCLUDED_)
#define AFX_FMSHEET_H__69BEF9E0_8F70_4CAD_AD9B_A1B5187A9238__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmSheet.h : header file
//

#include "fmbase.h"

/////////////////////////////////////////////////////////////////////////////
// CfmSheet command target

class AFX_EXT_CLASS CfmSheet : public CfmBase
{
	DECLARE_DYNCREATE(CfmSheet)

public:
	CfmSheet();	           // protected constructor used by dynamic creation
	CfmSheet(CAxisForm* axform, struct _formR* formR, int iform);
	virtual ~CfmSheet();

// Attributes
protected:
	int	m_item;
	int	m_drop;
	int	m_start;

	int	m_items;	// visible item count
	int	m_total;	// total count
	CRect	m_scr;
	CRect	m_cell;
	int	m_cx;

	bool	m_changed;
	bool	m_prev;
	bool	m_next;

	CStringArray m_desc;
	CStringArray m_data;
	enum	{ ckNone=0, ckItem, ckPrev, ckNext } m_click;

// Operations
public:
	void	Adjust(CPoint pt, float hR, float vR);
	void	Resize(float hR, float vR, bool font = true, bool redraw = false);
	void	Reload();
	void	Draw(CDC* dc);

	void	OnLButton(bool down, CPoint pt, int& result);
	bool	OnRepeat(CPoint pt, int& result);
	int	OnDrag(CPoint pt, int& result);
	bool	OnDrop(CPoint pt, CString data, CString& key);
	
	void	SetFocus(bool focus);
	
	void	ReadAll(CString& data, bool edit = false, bool head = false);
	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);

	void	WriteAll(CString str, bool redraw = true);
	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	void	ClearData(bool selectOnly = false);

	void	GetEnum(CString& text, int& value, int func = -1);

	int	GetLength();
	int	GetCurSel()		{ return m_item; }
	void	SetCurSel(int index, bool redraw = true);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmSheet)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	findStartItem();
	int	findItem(CString data);
	void	enable();
	int	getItem(CPoint pt);
	bool	getRect(int index, CRect& rect);
	void	clearSheet();

	// Generated message map functions
	//{{AFX_MSG(CfmSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmSheet)
	afx_msg BSTR _getData();
	afx_msg void _setData(LPCTSTR lpszNewValue);
	afx_msg long _getPRgb();
	afx_msg void _setPRgb(long nNewValue);
	afx_msg long _getTRgb();
	afx_msg void _setTRgb(long nNewValue);
	afx_msg BOOL _getVisible();
	afx_msg void _setVisible(BOOL bNewValue);
	afx_msg long _getIndex();
	afx_msg void _setIndex(long nNewValue);
	afx_msg BSTR _getText();
	afx_msg void _Refresh();
	afx_msg BSTR _GetText(long index);
	afx_msg BSTR _GetData(long index);
	afx_msg void _SetItems(LPCTSTR items);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMSHEET_H__69BEF9E0_8F70_4CAD_AD9B_A1B5187A9238__INCLUDED_)
