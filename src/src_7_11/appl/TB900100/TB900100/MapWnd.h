#pragma once


// CMapWnd
#include "UnBond.h"


class CMapWnd : public CWnd
{
	DECLARE_DYNAMIC(CMapWnd)

public:
	CMapWnd(CWnd* pWizard);
	virtual ~CMapWnd();

	CWnd*	m_pWizard;
	CUnBond* m_pWnd;

	int GetKey();
	CString Variant(int nComm, CString strData = _T(""));
	CString GetRoot();
	CWnd* GetWizard();
	CString GetName();
	CBitmap* GetBitmap(CString strPath);
	COLORREF GetIndexColor(int nIndex);
	CString Parser(CString &strSrc, CString strSub = _T("\t"));
	void SetTitle(CString strdata);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg	LRESULT OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
};


