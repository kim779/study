#if !defined(AFX_FMEDIT_H__64BB6953_E221_4639_8AC3_5DDC15192C51__INCLUDED_)
#define AFX_FMEDIT_H__64BB6953_E221_4639_8AC3_5DDC15192C51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmEdit.h : header file
//

#include "fmbase.h"

const	int	SO = 0x0e;
const	int	SI = 0x0f;

/////////////////////////////////////////////////////////////////////////////
// CfmEdit command target

class AFX_EXT_CLASS CfmEdit : public CfmBase
{
	DECLARE_DYNCREATE(CfmEdit)

public:
	CfmEdit();	           // protected constructor used by dynamic creation
	CfmEdit(CAxisForm* axform, struct _formR* formR, int iform, bool drawOnly);
	virtual ~CfmEdit();

// Attributes
public:
	bool	m_changed;		// check changed
	CString m_slog;
protected:
	bool	m_drawOnly;
	bool	m_caret;
	int	m_caretPos;		// display pointer
	int	m_updPos;		// update pointer
	enum	eState { esNone=0, esReverse, esClick, esNormal } m_state;
	enum	{ ckNone=0, ckSpinUp, ckSpinDown, ckCode } m_click;
	enum	{ mvNone=0, mvIn, mvCode, mvSpinUp, mvSpinDown } m_move;
	CString	m_data;			// display string
	CPoint	m_point;
	CRect	m_tRc;
	CRect	m_iRc;			// invalidate rect
	bool	m_focusing;
	int	m_skip;
	int	m_stPos;
	CString	m_edit;

	class	Cimage*	m_cpic;
	class	Cimage*	m_spic1;
	class	Cimage*	m_spic2;

private:
	CRect	m_spinRc;
	CRect	m_codeRc;
	double	m_delta;
	bool	m_search;
	int	m_float;
	double	m_lower;
	double	m_upper;
	class	CmonthWnd*	 m_cal;

	CString	m_category;
	CString	m_code;
	CStringArray	m_codes;

// Operations
public:
	bool	IsValid(int& guide);
	void	Adjust(CPoint pt, float hRatio, float vRatio);
	void	Resize(float hRatio, float vRatio, bool font = true, bool redraw = false);
	void	Reload();

	virtual	void	Draw(CDC* dc);
	virtual	void	OnLButton(bool down, CPoint pt, int& result);
	virtual	void	OnDblClick(CPoint pt, int& result);
	virtual	bool	OnRepeat(CPoint pt, int& result);
	virtual	int	OnDrag(CPoint pt, int& result);

	virtual	void	SetFocus(bool focus);
	virtual	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	virtual	void	UpdateData(int key, bool moving, int& result);
	virtual	void	InsertData(int key, bool moving, int& result);
	virtual	void	ReadData(CString& data, bool edit = false, int size = -1, int col = -1, int row = -1);
	virtual	void	ClearData(bool selectOnly = false);
	virtual	int	GetLength();
	
	bool	IsChanged(bool reset = true);
	void	Refresh();

	CString	Remove(char ch);

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func);
	virtual	void	DoListBox(bool apply, bool focus = false);
	bool	IsAutoThousand();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfmEdit)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual	void	keyDelete(int& result);
	virtual	void	keyBackspace(int& result);
	virtual	int	keyArrowLeft(int& result);
	virtual	void	keyArrowRight(int& result);
	void	keyArrowUp(int& result);
	void	keyArrowDown(int& result);
	void	keyPrior(int& result);
	void	keyNext(int& result);
	void	keyReturn(int& result);

	void	editData();
	int	calcCaretPos();
	void	getStartPos();
	double	getSpinPos(bool up = true);
	void	getSpinData(bool up = true);
	void	checkMode();
	bool	getCode(bool up = true);
	bool	findCode(bool up = true);
	bool	findCode(int kind, int index, bool up = true);
	bool	getHistory(bool up = true);
	BOOL	makeCalendar();
	void	drawEdit(CDC* dc, CRect rect);

	// Generated message map functions
	//{{AFX_MSG(CfmEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CfmEdit)
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
	afx_msg BOOL _getSkip();
	afx_msg void _setSkip(BOOL bNewValue);
	afx_msg BOOL _getMust();
	afx_msg void _setMust(BOOL bNewValue);
	afx_msg BOOL _getFocus();
	afx_msg BOOL _getTabStop();
	afx_msg void _setTabStop(BOOL bNewValue);
	afx_msg void _SetFocus();
	afx_msg void _Clear();
	afx_msg void _SetDelta(double delta);
	afx_msg BSTR _Remove(short ch);
	afx_msg void _Refresh();
	afx_msg void _Trigger(LPCTSTR name, BOOL all);
	afx_msg void _Push(LPCTSTR name);
	afx_msg void _Pop(LPCTSTR name);
	afx_msg void _SetType(long type);
	afx_msg void _SetRange(double lower, double upper);
	afx_msg void _SetSend(BOOL send, long target);
	afx_msg void _SetFormat(LPCTSTR formats);
	afx_msg void _SetFloatDigit(long digit);
	afx_msg long _GetDataKind();
	afx_msg long _GetMode();
	afx_msg long _GetLength();
	afx_msg BSTR _GetFormat();
	afx_msg void _SetText(LPCTSTR text);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMEDIT_H__64BB6953_E221_4639_8AC3_5DDC15192C51__INCLUDED_)
