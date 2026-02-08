#pragma once
// GubnConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGubnConfigDlg dialog

class CGubnConfigDlg : public CDialog
{
// Construction
public:
	CGubnConfigDlg(CFont* pFont, COLORREF color1, COLORREF color2, COLORREF color3, COLORREF color4, CWnd* pParent = NULL);   // standard constructor

	CNewsListArray			m_arNewsList;
	CButton					m_pNewsButton[MAXCNT];
	CFont*					m_pFont;
	CArray<CRect, CRect>	m_arLblRect;
	CStringArray			m_arTypeList;
	COLORREF				m_backcolor, m_listhdrcolor, m_listlinecolor, m_txtcolor;
	
// Dialog Data
	//{{AFX_DATA(CGubnConfigDlg)
	enum { IDD = IDD_GUBNCONDIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGubnConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGubnConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSetbutton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

