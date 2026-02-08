#if !defined(AFX_THEMEPREVIEWDLG_H__2CB81E6A_07A8_48C0_B461_10932B3324FA__INCLUDED_)
#define AFX_THEMEPREVIEWDLG_H__2CB81E6A_07A8_48C0_B461_10932B3324FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemePreviewDlg.h : header file
//

#include "resource.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CThemePreviewDlg dialog

class CThemePreviewDlg : public CDialog
{
// Construction
public:
	CThemePreviewDlg(int currIndex, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThemePreviewDlg)
	enum { IDD = IDD_THEME_PREVIEW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemePreviewDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThemePreviewDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPrev();
	afx_msg void OnNext();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	//CArray<CFileBitmap*, CFileBitmap*> m_bitmaps;
	std::vector<std::shared_ptr<CFileBitmap>> m_bitmaps;
	CStringArray m_themes;
	int m_index;
	int m_oldIndex;
	
	void LoadBitmaps();
	void FreeBitmaps();

	void DrawSample(CDC* pDC = NULL);
	void DrawGradient(CDC* pDC, CRect rc, COLORREF color1, COLORREF color2, bool bRight);

	//CControlChanger* m_changer{};
	std::unique_ptr <CControlChanger> m_changer;
public:
	int GetIndex();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEPREVIEWDLG_H__2CB81E6A_07A8_48C0_B461_10932B3324FA__INCLUDED_)
