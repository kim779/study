#pragma once

#include "resource.h"

// CPublicDlg 대화 상자입니다.

class AFX_EXT_CLASS CPublicDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPublicDlg)

public:
	CPublicDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPublicDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PUBLIC };
	CString m_helps;
	CListBox m_newList;
	CListBox m_oldList;
	CListCtrl m_propList;

	CString		m_publicR;

protected:
	CStringArray	m_HelpArr;
	CStringArray	m_proArr;

	void	loadFile();
	BOOL	addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	BOOL	addItem(CListCtrl *LC, int item, int subItem, CString &str);
	void	onPublicInfo(CString data);
	CString	parse(CString &src, CString sub);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnInBtn();
	afx_msg void OnOutBtn();
	afx_msg void OnSelChangeNewlist();
	afx_msg void OnDblclkNewlist();
	afx_msg void OnSelChangeOldlist();
	afx_msg void OnDblclkOldlist();

};
