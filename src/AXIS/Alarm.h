#if !defined(AFX_ALARM_H__C1674483_D5D9_4816_B165_3713F5525836__INCLUDED_)
#define AFX_ALARM_H__C1674483_D5D9_4816_B165_3713F5525836__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Alarm.h : header file
//

#include "ShapeButton.h"
#include "useMaindefine.h"
/////////////////////////////////////////////////////////////////////////////
// CAlarm dialog

class CAlarm : public CDialog
{
// Construction
public:
	CAlarm(CWnd* pParent = NULL);   // standard constructor
	~CAlarm();
	void	SetMsg(CString msg) { m_msg = msg; }
// Dialog Data
	//{{AFX_DATA(CAlarm)
	enum { IDD = IDD_NOTICE };
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
	// Generated message map functions
	//{{AFX_MSG(CAlarm)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChangeMsg();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap* m_bitmap;
#ifdef DF_USE_CPLUS17
	std::unique_ptr < CShapeButton> m_btnClose;
#else
	CShapeButton* m_btnClose;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARM_H__C1674483_D5D9_4816_B165_3713F5525836__INCLUDED_)
