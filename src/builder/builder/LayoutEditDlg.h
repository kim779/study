#pragma once
#include "afxcmn.h"


// CLayoutEditDlg 대화 상자입니다.

class CLayoutEditDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLayoutEditDlg)

public:
	CLayoutEditDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLayoutEditDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_FORM_LAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


protected:
	CString		m_strLayoutName;
	CListCtrl	m_LayoutList;

public:
	CMapStringToString	m_mapData;
	CStringArray		m_arList;

public:
	void	SetLayoutList(CMapStringToString* pMap, CStringArray* parLayout);
	void	SetCurLayOutName(CString sName){ m_strLayoutName = sName; }
public:
	afx_msg void OnBnClickedBtnAdd();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnDelete();
	afx_msg void OnLvnItemchangedLayoutList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnRename();
	afx_msg void OnBnClickedOk();
};
