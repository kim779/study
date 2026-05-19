#pragma once

#include "resource.h"
#include "hoverButton.h"

// CLogin 대화 상자입니다.

class AFX_EXT_CLASS CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLogin();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOGIN };
	BOOL m_offline;
	CString m_user;
	CString m_pass;
	CHoverButton m_setBN;
	CHoverButton m_cancelBN;
	CHoverButton m_connectBN;

	CString	m_currentPath;

protected:
	CWnd*	m_parent;
	CSize	m_bitmapSz;
	CBitmap	m_bitmap;
	CString	m_errs;
	CBrush	m_brush;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CSize	bitmapSize(HBITMAP hBitmap);
	void	drawBitmap(CDC *pDC);

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetup();

	void	SetParentW(CWnd* pParent);
	void	SetMessage(CString sMsg);
	void	EnableLogin(bool bEnable = true);
};
