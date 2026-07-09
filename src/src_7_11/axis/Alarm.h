#if !defined(AFX_ALARM_H__C1674483_D5D9_4816_B165_3713F5525836__INCLUDED_)
#define AFX_ALARM_H__C1674483_D5D9_4816_B165_3713F5525836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Alarm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlarm dialog

class CAlarm : public CDialog
{
// Construction
public:
	CAlarm(CString home, CWnd* pParent = NULL);   // standard constructor
	~CAlarm();
	void	SetMsg(CString msg) { m_msg = msg; }
	CString Parser(CString &srcstr, CString substr);
// Dialog Data
	//{{AFX_DATA(CAlarm)
	enum { IDD = IDD_NOTICE };
	CStatic	m_skin;
	CString	m_msg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlarm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString	m_home;
	HBITMAP	m_bitmap;
	CBrush*				m_pCtlBrush;
	// Generated message map functions
	//{{AFX_MSG(CAlarm)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CEdit m_edit;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARM_H__C1674483_D5D9_4816_B165_3713F5525836__INCLUDED_)
