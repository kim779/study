
#pragma once

#include "resultEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList 창

class COutputWnd : public CDockablePane
{
// 생성입니다.
public:
	COutputWnd();
	virtual ~COutputWnd();

	CResultEditCtrl	m_edit;

	void	AddText(CString txt, COLORREF txtColor = RGB(0,0,0));

// 특성입니다.
protected:
	bool	m_btextpos;
	long	m_ltextpos;
	CFont	m_font;

	void	FixedAddText(CString txt);
	void	BarTextClear();
	CString	parse(CString &src, CString sub);


protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnTrace(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

