#if !defined(AFX_CODECTRL_H__4BDE51A6_0565_46F6_8481_F6E65E5E3236__INCLUDED_)
#define AFX_CODECTRL_H__4BDE51A6_0565_46F6_8481_F6E65E5E3236__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeCtrl.h : header file
//
#include <afxtempl.h>
#include "qsort.h"

/////////////////////////////////////////////////////////////////////////////
// CListBox window

class CSubList  : public CWnd
{
public:
	CSubList(class CCodeCtrl *pCodeCtrl);
	virtual ~CSubList();

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubList)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

	class CCodeCtrl	*m_pCodeCtrl;
};

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo window

	struct _JCode
	{
		CString Code;
		CString Name;
	};


class CCodeCombo : public CComboBox
{
// Construction
public:
	CCodeCombo(class CCodeCtrl *pParent, int iTextHeight, int TextGap);	// IBK

	enum { DM_NONE, DM_HISTORY, DM_CODELIST};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeCombo)
	afx_msg void OnSelchange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);		// IBK
	//rtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);	// IBK
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SearchCode(CString code);
	CString	GetSelectCode(CString sCode);
	void	Key(int key);
	void	InitCodeList();
	void	InitHistory(CString history, bool bForeign = false);
	void	InitIndexList(int kkind);
	bool	IsHistoryMode();
	bool	IsCodeListMode();
	bool	JCodeLoad(CString tabPath);
	bool	UCodeLoad(CString tabPath);

	bool	IsListShow();
	int	GetDataHeight()	{ return m_TextHeight+m_TextGap;	}
	//void	AddString(LPCTSTR str);	// IBK
	
private:
	class CCodeCtrl		*m_pParent;
	CAxisGComApp		*m_pApp;
	int			m_DataMode;
	int			m_TextHeight;	// IBK
	int			m_TextGap;	// IBK
	CString			m_sCode;
	HWND			m_hList;
	class CSubList		*m_pSubList = nullptr;

	CArray <_JCode, _JCode> m_pHCode;
	CQArray <_JCode, _JCode> m_pJCode;
	CArray<struct upcode*, struct upcode*>		m_pUCode;

};

/////////////////////////////////////////////////////////////////////////////
// CodeEdit window 

class CCodeEdit : public CEdit
{
// Construction
public:
	CCodeEdit(class CCodeCtrl *pParent, CWnd* pTarget, int type);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeEdit)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SetUnit(int unit);
	void	ListCode(CString code);
	void	ChangeHangulMode(HWND hWnd, DWORD *saveMode);
	void	SetCode(CString code);

private:

	class CCodeCtrl	*m_pParent;
	CWnd		*m_pTarget;
	int		m_Type;

	int		m_Unit;
	CString		m_preCode;
	bool		m_bInitFocus;
	bool		m_bKeyChange;
	bool		m_bListKey;
	DWORD		m_SaveMode;
};

/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl window

class CCodeCtrl : public CWnd
{
// Construction
public:
	CCodeCtrl(CWnd *pParent, CWnd *pView, CFont *pFont, int type, LPCTSTR userPath);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString	GetEditData();
	void	SetEditData(CString sData);
	void	SetUnit(int unit);
	void	SetKkind(int kkind);
	void	HistoryMode();
	void	CodeListMode();
	bool	IsAllNumbers(CString sCode);
	void	LButtonUp();
	void	Draw(CDC *pDC);

	class CCodeEdit		*m_pEdit;
	class CCodeCombo	*m_pCombo;
	CRect			m_btnRect;
	CRect			m_editRect;
private:
	CWnd		*m_pParent;
	CWnd		*m_pView;
	CFont		*m_pFont;
	int		m_Type;
	int		m_Unit;
	int		m_Kkind;

	bool		m_bBtnDown;
	CString		m_userPath;

	// Drawing Combo
	COLORREF	m_cbRGB;		//색상 저장변수
	COLORREF	m_csRGB;

public:
	CString GetHistoryFileName(LPCTSTR key);
	CString GetHistory(LPCTSTR key);
	void	AddHistory(LPCTSTR key, LPCTSTR code, LPCTSTR name);
	void	SetComboPalette(COLORREF bg, COLORREF fg) {m_cbRGB = bg; m_csRGB = fg;}
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODECTRL_H__4BDE51A6_0565_46F6_8481_F6E65E5E3236__INCLUDED_)
