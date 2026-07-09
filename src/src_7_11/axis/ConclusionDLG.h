#if !defined(AFX_CONCLUSIONDLG_H__65E803ED_92FF_4ED7_BE4B_22CF653F192C__INCLUDED_)
#define AFX_CONCLUSIONDLG_H__65E803ED_92FF_4ED7_BE4B_22CF653F192C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConclusionDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConclusionDLG dialog

class CConclusionDLG : public CDialog
{
// Construction
public:
	CConclusionDLG(CWnd* pParent, CString home);   // standard constructor

	~CConclusionDLG();

	void	InsertMsg(CString msg);

	int	m_nMaxData;

	void	RedrawCtrl(bool bResize);

	class CfxGridEx* m_grid;
	CString		m_home;

	CFont		m_GridFont;

	void		ChangePalette();
	void		initGrid();
	void		insertRow(int row);
	COLORREF	GetColor(int index);

// Dialog Data
	//{{AFX_DATA(CConclusionDLG)
	enum { IDD = IDD_CONCLUSION_LIST };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConclusionDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConclusionDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONCLUSIONDLG_H__65E803ED_92FF_4ED7_BE4B_22CF653F192C__INCLUDED_)
