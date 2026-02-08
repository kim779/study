#if !defined(AFX_IMAGEVIEWER_H__296DA51E_82F8_402F_87C6_81072D77E99A__INCLUDED_)
#define AFX_IMAGEVIEWER_H__296DA51E_82F8_402F_87C6_81072D77E99A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CImageViewer window

class CImageViewer : public CStatic
{
// Construction
public:
	CImageViewer();

// Attributes
public:
	CString m_strFile;
	
// Operations
public:
	void LoadImage(LPCSTR szFile);

protected:
	void _DoDraw(CDC* pdc, const CRect& rcBounds);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageViewer)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageViewer();



	// Generated message map functions
protected:
	//{{AFX_MSG(CImageViewer)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEVIEWER_H__296DA51E_82F8_402F_87C6_81072D77E99A__INCLUDED_)
