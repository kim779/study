#pragma once
// AxBrower.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxBrower html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

#include "../../dll/sm/MenuXP.h"
#include "../../h/axisfire.h"

#define WM_SIZE_WEBBROW WM_USER + 1008
#define WM_SIZING_WEBBROW WM_USER + 1009
#define ID_FINDMSGBOX 8888

enum IEScaleSize {IESS_Smallest, IESS_Small, IESS_Medium, IESS_Large, IESS_Largest};

class CAxBrower : public CHtmlView
{
public:
	CAxBrower();           // protected constructor used by dynamic creation
	virtual ~CAxBrower();
	DECLARE_DYNCREATE(CAxBrower)

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void		SetParent( CWnd* p ) { m_pParent = p; }

	CString		m_strSelectedCod;
// html Data
public:
	//{{AFX_DATA(CAxBrower)
	//}}AFX_DATA
	int kill_count;
public:
	//{{AFX_VIRTUAL(CAxBrower)
	public:
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	//{{AFX_MSG(CAxBrower)
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSizing( UINT nSide, LPRECT lpRect );
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	//}}AFX_MSG
	afx_msg void OnCurPrice();
	afx_msg void OnSellStock();
	afx_msg void OnBuyStock();
	afx_msg void OnChartStock();

	DECLARE_MESSAGE_MAP()

	
private:
	CWnd*	m_pParent;
};

