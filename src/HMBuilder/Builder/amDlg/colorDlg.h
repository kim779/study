#pragma once

#include "resource.h"

// CColorDlg 대화 상자입니다.

class AFX_EXT_CLASS CColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorDlg)

public:
	CColorDlg(CWnd* pParent, DWORD color, bool user = true, CRect rect = CRect(0, 0, 0, 0));   // 표준 생성자입니다.
	virtual ~CColorDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COLOR };
	CTabCtrl m_tabCtrl;
	DWORD	m_color;

protected:
	class	CColorBox	*m_pUserColor, *m_pSysColor, *m_pUserColor2;
	bool	m_user;
	bool	m_capture;
	CRect	m_rect;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

	void freeRes();

private:
	int	m_tabPos;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeTabctrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
};
