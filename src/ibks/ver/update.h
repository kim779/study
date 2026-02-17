#if !defined(AFX_UPDATE_H__87F43467_44CA_4D87_B41A_382CC26F3535__INCLUDED_)
#define AFX_UPDATE_H__87F43467_44CA_4D87_B41A_382CC26F3535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// update.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpdate dialog

class CUpdate : public CDialog
{
// Construction
public:
	CUpdate(CString root = _T(""), CString images = _T(""), CWnd* pParent = NULL);   // standard constructor
	virtual	~CUpdate();

// Dialog Data
	//{{AFX_DATA(CUpdate)
	enum { IDD = IDD_UPDATE };
	CStatic	m_static;
	CProgressCtrl	m_progress;
	//}}AFX_DATA

private:
	CString	m_root;
	CString	m_images;
	CFont	m_font;
	HBITMAP	m_bitmap;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpdate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpdate)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UPDATE_H__87F43467_44CA_4D87_B41A_382CC26F3535__INCLUDED_)
