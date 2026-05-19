#if !defined(AFX_FMEDIT_H__64BB6953_E221_4639_8AC3_5DDC15192C51__INCLUDED_)
#define AFX_FMEDIT_H__64BB6953_E221_4639_8AC3_5DDC15192C51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fmEdit.h : header file
//

#include "ambase.h"
#include "image.h"

const	int	SO = 0x0e;
const	int	SI = 0x0f;

/////////////////////////////////////////////////////////////////////////////
// CamEdit command target

class AFX_EXT_CLASS CamEdit : public CamBase
{
	DECLARE_DYNCREATE(CamEdit)

public:
	CamEdit();	           // protected constructor used by dynamic creation
	CamEdit(CAmForm* axform, struct _formR* formR, bool drawOnly);
	virtual ~CamEdit();

// Attributes
public:
	bool	m_changed;		// check change

protected:
	bool	m_drawOnly;
	bool	m_caret;
	int		m_caretPos;		// display pointer
	int		m_updPos;		// update pointer
	enum	eState { esNone=0, esReverse, esClick, esNormal } m_state;
	enum	{ ckNone=0, ckSpinUp, ckSpinDown, ckCode, ckCodeUp, ckCodeDown } m_click;
	enum	eMove  { mvNone=0, mvIn, mvCode, mvSpinUp, mvSpinDown, mvCodeUp, mvCodeDown } m_move;
	CString	m_data;			// display string
	CPoint	m_point;
	CRect	m_tRc;
	CRect	m_iRc;			// invalidate rect
	bool	m_focusing;
	int	m_skip;
	int	m_stPos;
	CString	m_edit;

	int	m_csRGB;		// Combo Scroll Color
	int	m_cbRGB;		// Combo Back Color

	Cimage* m_pic;
	/*
	class	Cimage*	m_cpic;
	class	Cimage*	m_spic1;
	class	Cimage*	m_spic2;
	*/

	int	m_cbW;
	int	m_spinW;

private:
	CRect	m_spinRc;
	CRect	m_codeRc;
	double	m_delta;
	bool	m_search;
	int	m_float;
	double	m_lower;
	double	m_upper;
	class	Cmonth*	m_cal;

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
	virtual	int		OnDrag(CPoint pt, int& result);

	virtual	void	SetFocus(bool focus);
	virtual	void	WriteData(CString data, bool redraw = true, int col = -1, int row = -1);
	virtual	void	UpdateData(int key, bool moving, int& result);
	virtual	void	InsertData(int key, bool moving, int& result);
	virtual	void	ReadData(CString& data, bool edit = false, int col = -1, int row = -1);
	virtual	void	ClearData(bool selectOnly = false);
	virtual	int	GetLength();
	
	bool	IsChanged(bool reset = true);
	void	Refresh();

	CString	Remove(char ch);

	void	GetEnum(CString& text, int& value, int func = -1);
	void	SetEnum(CString text, int value, int func = -1);
	virtual	void	DoListBox(bool apply, bool focus = false);


	LPCTSTR		GetText();
	void		SetText(LPCTSTR lpszNewValue);
	LPCTSTR		GetData();
	void		SetData(LPCTSTR lpszNewValue);
	long		GetPRgb();
	void		SetPRgb(long nNewValue);
	long		GetTRgb();
	void		SetTRgb(long nNewValue);
	bool		GetVisible();
	void		SetVisible(bool bNewValue);
	bool		GetEnable();
	void		SetEnable(bool bNewValue);
	bool		GetSkip();
	void		SetSkip(bool bNewValue);

	bool		_GetFocus();
	void		_SetFocus();
	void		_Clear();
	void		_SetDelta(double delta);
	CString		_Remove(short ch);
	void		_Refresh();
	void		_Blink(bool set, long color);
	void		_Trigger(LPCTSTR name, bool all);
	void		_Push(LPCTSTR name);
	void		_Pop(LPCTSTR name);
	void		_SetType(long type);
	void		_SetRange(double lower, double upper);
	void		_SetSend(bool send, long target);



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CamEdit)
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

	void	editData();
	int	calcCaretPos();
	void	getStartPos();
	double	getSpinPos(bool up = true);
	void	getSpinData(bool up = true);
	void	checkMode();
	bool	getCode(bool up = true);
	bool	findCode(bool up = true);
	bool	findCode(int kind, int index, bool up = true);
	BOOL	makeCalendar();

	

	// Generated message map functions
	//{{AFX_MSG(CamEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CamEdit)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FMEDIT_H__64BB6953_E221_4639_8AC3_5DDC15192C51__INCLUDED_)
