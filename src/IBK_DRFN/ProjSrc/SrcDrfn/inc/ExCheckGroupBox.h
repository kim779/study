#if !defined(AFX_EXCHECKGROUPBOX_H__DC1A25F3_BC2D_434E_A903_1FA5BF41DE7D__INCLUDED_)
#define AFX_EXCHECKGROUPBOX_H__DC1A25F3_BC2D_434E_A903_1FA5BF41DE7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExCheckGroupBox.h : header file
//

void AFXAPI DDX_GroupCheck(CDataExchange* pDX, int nIDC, int& value);

/////////////////////////////////////////////////////////////////////////////
// CExCheckGroupBox window

class CLoaderCheckGroupBoxButton;
class CExCheckGroupBox : public CButton
{
// Construction
public:
	CExCheckGroupBox();

// Attributes
protected:
	CLoaderCheckGroupBoxButton*		m_pLoader;

// Operations
public:
	int		GetCheck();
	void	SetCheck(int nCheck);
	void	SetStyle(TouchStyle nStyle);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExCheckGroupBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExCheckGroupBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExCheckGroupBox)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
	afx_msg void OnClicked();
	afx_msg LRESULT OnGetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnSetCheck(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCHECKGROUPBOX_H__DC1A25F3_BC2D_434E_A903_1FA5BF41DE7D__INCLUDED_)
