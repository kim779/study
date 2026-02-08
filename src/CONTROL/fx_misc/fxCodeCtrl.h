#pragma once
// fxCodeCtrl.h : header file
//


#include <afxtempl.h>

#define GU_NONE		-1
#define	GU_CODE		0
#define	GU_FUTURE	1
#define	GU_OPTION	2
#define	GU_INDEX	3
#define	GU_FOCODE	4
#define	GU_POPTION	5
#define	GU_ELWCODE	6

#define	sz_JCODE	6
#define	sz_FCODE	6
#define	sz_OPTION	8
#define	sz_INDEX	3
#define	sz_FOCODE	8
#define sz_ELW		6


struct fCode
{
	CString Code;
	CString Name;
};


/////////////////////////////////////////////////////////////////////////////
// QuickSort

/////////////////////////////////////////////////////////////////////////////
// CListBox window

class CSubList  : public CWnd
{
public:
	CSubList(class CfxCodeCtrl *pCodeCtrl);
	virtual ~CSubList();

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSubList)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

	class CfxCodeCtrl	*m_pCodeCtrl;
};

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo window

class CCodeCombo : public CComboBox
{
// Construction
public:
	CCodeCombo(class CfxCodeCtrl *pParent);
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
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	bool	IsValidCode(CString sCode);
	bool	FCodeLoad(CString tabPath);
	void	SearchCode(CString code);
	CString	GetSelectCode();
	void	Key(int key);
	void	InitCodeList();
	void	InitHistory(CString history);
	bool	IsHistoryMode();
	bool	IsCodeListMode();
	bool	JCodeLoad(CString tabPath);
	bool	ELWCodeLoad(CString tabPath);
	void	RemoveCode();
	CString GetCodeName( CString strCode );
public:
	int			m_DataMode;

private:
	class CfxCodeCtrl	*m_pParent{};
	CString			m_sCode;
	HWND			m_hList{};
	std::unique_ptr<class CSubList> m_pSubList{};
	std::vector<std::pair<CString, CString>>	_vCode, _vHCode;
};

/////////////////////////////////////////////////////////////////////////////
// CodeEdit window 

class CCodeEdit : public CEdit
{
// Construction
public:
	CCodeEdit(class CfxCodeCtrl *pParent, CWnd* pTarget);

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
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	bool	isHexNumeric(CString str);
	bool	isNumeric(CString str);
	void	SetUnit(int unit) { m_Unit = unit; }
	void	ListCode(CString code);
	void	ChangeHangulMode(HWND hWnd, bool bHangle = true);
public:
	bool	m_bValidCheck;
private:

	class CfxCodeCtrl* m_pParent;
	CWnd*		m_pTarget;

	int		m_Unit;
	CString		m_preCode;
	bool		m_bInitFocus, m_bKeydown;
};

/////////////////////////////////////////////////////////////////////////////
// CfxCodeCtrl window

class AFX_EXT_CLASS CfxCodeCtrl : public CWnd
{
// Construction
public:
	CfxCodeCtrl(CWnd* parent, CString tabPath);

// Attributes
public:
	std::unique_ptr<class CCodeEdit>  m_pEdit;
	std::unique_ptr<class CCodeCombo> m_pCombo;

	CRect		m_btnRect;
	CString		m_sHistory;
	CString		m_tabPath;
	bool		m_bValidCheck;
private:
	CWnd*		m_parent;
	CWnd*		m_view;
	CFont*		m_pFont;
	int			m_Unit;
	bool		m_btnDown;
// Operations
public:
	BOOL	Create(CWnd* parent, CRect rect, UINT id);

	void	SetHistory(CString sHistory);
	void	SetFont(CFont* pFont, bool redraw = true);
	CString	GetEditData();
	void	SetEditData(CString sData);
	void	SetUnit(int unit);
	void	HistoryMode();
	void	CodeListMode();
	void	LButtonUp();
	void	SetValidCheck(bool bValid);
	CString GetCodeName( CString strCode ) { return m_pCombo->GetCodeName(strCode); } 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxCodeCtrl)

	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CfxCodeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CfxCodeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

