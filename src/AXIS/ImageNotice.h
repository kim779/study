#if !defined(AFX_IMAGENOTICE_H__79F66CA6_DD52_41E3_BFC8_B9B3E0EF0175__INCLUDED_)
#define AFX_IMAGENOTICE_H__79F66CA6_DD52_41E3_BFC8_B9B3E0EF0175__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageNotice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageNotice dialog

class CImageNotice : public CDialog
{
// Construction
public:
	CImageNotice(CWnd* pParent, LPCSTR szImagePath);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CImageNotice)
	enum { IDD = IDD_IMAGE_NOTICE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageNotice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	CString m_path;
	CBitmap m_image;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CImageNotice)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGENOTICE_H__79F66CA6_DD52_41E3_BFC8_B9B3E0EF0175__INCLUDED_)
