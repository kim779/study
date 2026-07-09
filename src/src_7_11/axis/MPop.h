#if !defined(AFX_MPOP_H__14318362_5EDD_406D_B14F_6D5ED5BC6A38__INCLUDED_)
#define AFX_MPOP_H__14318362_5EDD_406D_B14F_6D5ED5BC6A38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MPop.h : header file
//
#define	WM_CHILDMSG		WM_USER+666
/////////////////////////////////////////////////////////////////////////////
// CMPop dialog

class CMPop : public CDialog
{
// Construction
public:
	CMPop(CWnd* pParent = NULL);   // standard constructor
	virtual	~CMPop();

	CWnd*	m_actW;
	int	m_cursor;
	int	m_vwKEY;		// window id
	int	m_vwTYPE;
	int	m_trigger;
	int	m_wpos;
//	bool	m_lock;
	CString	m_mapN;

	class CBaseMP*	m_wnd;

	int		m_cxBorder, m_cyBorder, m_cyCaption;
	HRGN		m_rgn;
	int		m_level;

protected:
	CPoint	m_pos;
	CSize	m_size;
	CWnd*	m_main;
public:
	void	SetPos(int x, int y);
	void	SetSize(int cx, int cy);

	int	osLevel();
// Dialog Data
	//{{AFX_DATA(CMPop)
	enum { IDD = IDD_MODALPOPUP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMPop)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMPop)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg	LRESULT OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg	LRESULT OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MPOP_H__14318362_5EDD_406D_B14F_6D5ED5BC6A38__INCLUDED_)
