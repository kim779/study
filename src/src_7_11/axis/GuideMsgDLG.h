#pragma once
// GuideMsgDLG.h : header file
//


#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CGuideMsgDLG dialog

class CGuideMsgDLG : public CDialog
{
// Construction
public:
	CGuideMsgDLG(CWnd* pParent, CString home);   // standard constructor
	~CGuideMsgDLG();

	void	InsertMsg(CString msg, int level = 1);

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
	//{{AFX_DATA(CGuideMsgDLG)
	enum { IDD = IDD_GUIDE_MSG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuideMsgDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGuideMsgDLG)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

