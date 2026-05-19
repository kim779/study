#if !defined(AFX_TEXTCOLORDLG_H__6510E887_3F31_4685_8B25_CDE4C8D76999__INCLUDED_)
#define AFX_TEXTCOLORDLG_H__6510E887_3F31_4685_8B25_CDE4C8D76999__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextColorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextColorDlg dialog

class CTextColorDlg : public CDialog
{
// Construction
public:
	CTextColorDlg(CWnd* pParent = NULL);   // standard constructor
	~CTextColorDlg();

// Dialog Data
	//{{AFX_DATA(CTextColorDlg)
	enum { IDD = IDD_TEXTCOLORSETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	COLORREF GetTColor(int idx) { return m_color[idx]; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COLORREF m_color[5];
	CBrush	m_brush[5];


	void	loadInfo();
	void	saveInfo();
	void	ChangeColor(int kind);
	// Generated message map functions
	//{{AFX_MSG(CTextColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	afx_msg void OnSetallcolor();
	afx_msg void OnSetsnrcolor();
	afx_msg void OnSetrtmcolor();
	afx_msg void OnSetsyscolor();
	afx_msg void OnSetrptcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTCOLORDLG_H__6510E887_3F31_4685_8B25_CDE4C8D76999__INCLUDED_)
