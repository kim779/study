#pragma once

#include "resource.h"

// CAutoToolTip 대화 상자입니다.

class CAutoToolTip : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoToolTip)

public:
	CAutoToolTip(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoToolTip();

	int	GetWidth();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTOTIP };
	long	m_start;

protected:
	CWnd*	m_parent;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();

protected:
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
};
