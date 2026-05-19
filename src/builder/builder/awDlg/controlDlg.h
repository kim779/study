#pragma once

#include "resource.h"

// CControlDlg 대화 상자입니다.

class AFX_EXT_CLASS CControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CControlDlg)

public:
	CControlDlg(int fN, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CControlDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CONTROL };
	CListCtrl m_list;

	class	CPropGrid* m_grid;
	CString	m_ctrlParam;
	CString	m_ctrlName;

protected:
	int	m_fN;
	int	m_beforeIdx;
	CStringArray	m_ctrlArr;
	CStringArray	m_dataArr;

	void	loadFile();
	BOOL	addItem(CListCtrl *LC, int item, int subItem, CString &str);
	BOOL	addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	CString	parse(CString &src, CString sub);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnItemChangedNameList(NMHDR *pNMHDR, LRESULT *pResult);
};
