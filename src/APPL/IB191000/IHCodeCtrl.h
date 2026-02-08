#pragma once
// IHCodeCtrl.h : header file
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

/////////////////////////////////////////////////////////////////////////////
// QuickSort
template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);

		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}


template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	int ii, jj;
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];
	do {
		if (bAscending) 
		{
			while (pArr[jj].Name < str.Name) jj++;
			while (pArr[ii].Name > str.Name) ii--;
		} 
		else 
		{
			while (pArr[jj].Name > str.Name) jj++;
			while (pArr[ii].Name < str.Name) ii--;
		}

		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}

			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);
}

/////////////////////////////////////////////////////////////////////////////
// CListBox window

class CIHSubList  : public CWnd
{
public:
	CIHSubList(class CIHCodeCtrl *pCodeCtrl);
	virtual ~CIHSubList();

protected:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIHSubList)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

	class CIHCodeCtrl	*m_pCodeCtrl;
};

/////////////////////////////////////////////////////////////////////////////
// CIHCodeCombo window

class CIHCodeCombo : public CComboBox
{
// Construction
public:
	CIHCodeCombo(class CIHCodeCtrl *pParent);

	enum { DM_NONE, DM_HISTORY, DM_CODELIST};

	struct _JCode
	{
		CString Code;
		CString Name;
	};

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIHCodeCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIHCodeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIHCodeCombo)
	afx_msg void OnSelchange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	bool	IsValidCode(CString sCode);
	bool	OCodeLoad(CString tabPath);
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
	HWND	GetListHwnd() 
	{ 
		return m_pSubList->m_hWnd; 
	}
public:
	int			m_DataMode;

private:
	class CIHCodeCtrl	*m_pParent;
	CString			m_sCode;
	HWND			m_hList;
	class CIHSubList		*m_pSubList;

	struct OCode
	{
		CString	Price;
		char	Atm;
		struct CALL
		{
			char	Exist;
			CString Name;
			CString	Code;
		} Call[4];
		struct PUT
		{
			char	Exist;
			CString	Name;
			CString	Code;
		} Put[4];
	};
	struct	OCode	oCode;

	CArray <_JCode, _JCode> m_pHCode;	// history list
	CQArray <_JCode, _JCode> m_pJCode;	// code list
};

/////////////////////////////////////////////////////////////////////////////
// CodeEdit window 

class CIHCodeEdit : public CEdit
{
// Construction
public:
	CIHCodeEdit(class CIHCodeCtrl *pParent, CWnd* pTarget);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIHCodeEdit)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIHCodeEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIHCodeEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
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

	class CIHCodeCtrl* m_pParent;
	CWnd*		m_pTarget;

	int		m_Unit;
	CString		m_preCode;
	bool		m_bInitFocus, m_bKeydown;
};

/////////////////////////////////////////////////////////////////////////////
// CIHCodeCtrl window

class CIHCodeCtrl : public CWnd
{
// Construction
public:
	CIHCodeCtrl(CWnd* pWnd, CString root);

public:
	class CIHCodeEdit*  m_pEdit;
	class CIHCodeCombo* m_pCombo;

	CRect		m_btnRect;
	CString		m_sHistory;
	CString		m_tabPath;
	bool		m_bValidCheck;
private:
	CWnd*		m_parent;
	CWnd*		m_view;
	CFont*		m_pFont;
	int		m_Unit;
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

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIHCodeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIHCodeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CIHCodeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	BOOL	m_bEdit, m_bCombo;
};

/////////////////////////////////////////////////////////////////////////////

