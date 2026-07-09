#if !defined(AFX_LOGIN_H__4EB74A11_D36D_4554_B475_772546DC5D57__INCLUDED_)
#define AFX_LOGIN_H__4EB74A11_D36D_4554_B475_772546DC5D57__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//
#include "resource.h"
//#include "Pane.h"
#include "HBBtn.h"
/////////////////////////////////////////////////////////////////////////////
// CLogin dialog

class AFX_EXT_CLASS CLogin : public CDialog
{
// Construction
public:
	CLogin(CWnd* pParent = NULL);   // standard constructor
	CString		m_currentPath;
	CWnd		*m_pParent;
	void		setParent(CWnd* pParent);
	void		setMessage(CString sMsg);
	void		EnableLogin(bool bEnable = true);

protected:
private:
	void	SetFileImage(CString sFName);
	CSize	BitmapSize(HBITMAP hBitmap);
	void	drawBitmap(CDC *pDC);
public:
	CSize		m_bitmapSz;
	CBitmap		m_Bitmap;
	CString		m_sErr;
	CBrush		m_brush;
// Dialog Data
	//{{AFX_DATA(CLogin)
	enum { IDD = IDD_LOGIN };
	CHBBtn		m_connectBtn;
	CHBBtn		m_cancelBtn;
	CHBBtn		m_setBtn;
	BOOL		m_offlineChk;
	CString		m_sPass;
	CString		m_sUser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogin)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnSetbtn();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__4EB74A11_D36D_4554_B475_772546DC5D57__INCLUDED_)
