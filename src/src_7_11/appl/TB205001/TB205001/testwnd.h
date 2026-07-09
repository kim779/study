#pragma once


// testwnd
#include "InterestPanel.h"
class testwnd : public CWnd
{
	DECLARE_DYNAMIC(testwnd)

public:
	testwnd();
	virtual ~testwnd();

	CWnd*	m_pWizard;
	CInterestPanel* m_pWnd;

	int GetKey();
	CString Variant(int nComm, CString strData = _T(""));
	CString GetRoot();
	CWnd* GetWizard();
	CString GetName();
	CBitmap* GetBitmap(CString strPath);
	COLORREF GetIndexColor(int nIndex);
	CString Parser(CString &strSrc, CString strSub = _T("\t"));
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg	LRESULT OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};


