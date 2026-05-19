#pragma once

#include "resource.h"
#include "scListBox.h"

// CAutoCmplDlg 대화 상자입니다.

class CAutoCmplDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAutoCmplDlg)

public:
	CAutoCmplDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAutoCmplDlg();

	void	AddString(CString str, int type = -1);
	void	FindString(CString str);
	BOOL	IsEmpty();
	void	Reset();
	int	GetWidth();
	CString	GetSelString();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_AUTOCOMPLETE };
	CScListBox m_aclist;

protected:
	CWnd*	m_parent;
	CImageList m_ImageList;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	void	createImageList();
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
};
