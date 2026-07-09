#if !defined(AFX_RESULTEDITCTRL_H__24A58341_DC32_11D5_A165_0050BF5DF9F6__INCLUDED_)
#define AFX_RESULTEDITCTRL_H__24A58341_DC32_11D5_A165_0050BF5DF9F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResultEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResultEditCtrl window

class CResultEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CResultEditCtrl();

public:
	bool	m_bSNDRCV, m_bRTM, m_bMode;
	CString	m_sRtn;
protected:
	CMenu *m_pRMenu;
	HCURSOR		m_hcursor;
private:

public:
protected:
private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	long OnMessage(WPARAM wParam, LPARAM lParam);
	void Clear();
	virtual ~CResultEditCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CResultEditCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTEDITCTRL_H__24A58341_DC32_11D5_A165_0050BF5DF9F6__INCLUDED_)
