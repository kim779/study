// SecCalcDlg.h : header file
//

#if !defined(AFX_SECCALCDLG_H__EA27E191_8139_4252_9113_E9A7FC60E3A6__INCLUDED_)
#define AFX_SECCALCDLG_H__EA27E191_8139_4252_9113_E9A7FC60E3A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSecCalcDlg dialog

class CSecCalcDlg : public CDialog
{
// Construction
public:
	CSecCalcDlg(CWnd* pParent = NULL);	// standard constructor
	void	CreateControls(CRect rc);
	void	SelectPage(int index);
	void	ChangeFee();
	COLORREF	GetIndexColor(int index);
	
	class CMyTab		*m_pTabCtrl;
	class COrderableDlg	*m_pPage1;
	class CTradeDlg		*m_pPage2;
	class CProfitDlg	*m_pPage3;

protected:
	CBitmap*	CreateBmp( LPCTSTR sBMPFile);
	void		DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap, bool bTrans = false);
	void		DrawRoundBitmap(CDC *dc, CRect rc, HBITMAP *hBitmap);
	CString		GetPalette();
	CString		GetRoot();
	void		SetBitmap();
	CString		Parser(CString &srcstr, CString substr);
	

	CString m_strRoot;
	CString	m_strPal;
	HBITMAP	m_hBmpRoundPanel[4];

// Dialog Data
	//{{AFX_DATA(CSecCalcDlg)
	enum { IDD = IDD_SECCALC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSecCalcDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSecCalcDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg long OnChild(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SECCALCDLG_H__EA27E191_8139_4252_9113_E9A7FC60E3A6__INCLUDED_)
