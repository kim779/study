#pragma once


// CFuncWnd
#include "CSedit.h"

class CFuncWnd : public CWnd
{
	DECLARE_DYNAMIC(CFuncWnd)

public:
	CFuncWnd() {};
	CFuncWnd(bool (*callback)(int, WPARAM, LPARAM));
	virtual ~CFuncWnd();
	bool	(*m_sticker_call)(int, WPARAM, LPARAM);

	void Init();

	std::unique_ptr<class CSedit> m_pEdit{};
	CRect m_Rect{};
	CString m_strData{};
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


