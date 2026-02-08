#if !defined(AFX_KOBAELWNOTIFY_H__192B774A_AC50_4024_8060_1027AA6629CA__INCLUDED_)
#define AFX_KOBAELWNOTIFY_H__192B774A_AC50_4024_8060_1027AA6629CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ContGrid.h"

#define IDC_KOBANOTIFY_GRID		(100101)

class CKobaElwNotify : public CDialog
{
// Construction
public:
	CKobaElwNotify(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKobaElwNotify();
	void ChangePallete();
	void Init();
	COLORREF GetColor(int index);
	void AddNotify(LPCSTR data);
protected:
	CContGrid	*m_pGrid{};

// Dialog Data
	//{{AFX_DATA(CKobaElwNotify)
	enum { IDD = IDD_KOBA_NOTIFY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKobaElwNotify)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKobaElwNotify)
	virtual BOOL OnInitDialog();
	afx_msg void OnClear();
	afx_msg void OnViewKobaelw();
	afx_msg void OnConfig();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KOBAELWNOTIFY_H__192B774A_AC50_4024_8060_1027AA6629CA__INCLUDED_)
