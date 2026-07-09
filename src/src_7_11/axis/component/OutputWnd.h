#pragma once

#include "resultEditCtrl.h"
// COutputWnd

class COutputWnd : public CSizingControlBarG
{
	DECLARE_DYNAMIC(COutputWnd)

public:
	COutputWnd();
	virtual ~COutputWnd();

	CResultEditCtrl	m_edit;
	void	AddText(CString txt, COLORREF txtColor = RGB(0,0,0));
	CFont*	m_font;
protected:
	bool	m_btextpos;
	long	m_ltextpos;
	

	void	FixedAddText(CString txt);
	void	BarTextClear();
	CString	parse(CString &src, CString sub);
protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnTrace(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


