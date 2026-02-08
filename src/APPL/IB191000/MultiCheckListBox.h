#pragma once
// MultiCheckListBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiCheckListBox window

class CMultiCheckListBox : public CCheckListBox
{
// Construction
public:
	CMultiCheckListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiCheckListBox)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMultiCheckListBox();
	int  CheckFromPoint(CPoint point, BOOL& bInCheck);
	void InvalidateCheck(int nIndex);
	void SetCheck(int nIndex, int nCheck);
	int  GetCheck(int nIndex);
	int  InMultiCheck(CPoint point, BOOL& bInMultiCheck);
	void InvalidateMultiCheck(int nIndex);
	int  GetMultiCheck(int nIndex);
	void SetMultiCheck(int nIndex, int nCheck);

	// Generated message map functions
protected:
	void PreDrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	//{{AFX_MSG(CMultiCheckListBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

