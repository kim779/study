#pragma once

#include "resource.h"

// CComboItem 대화 상자입니다.

class AFX_EXT_CLASS CComboItem : public CDialogEx
{
	DECLARE_DYNAMIC(CComboItem)

public:
	CComboItem(CWnd* pParent, int count, int size);   // 표준 생성자입니다.
	virtual ~CComboItem();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COMBOITEM };
	CListBox m_data;
	CListBox m_display;
	CString m_edata;
	CString m_edisplay;

	int	m_count;
	CString	m_datB, m_disB;

protected:
	int	m_sel;
	int	m_size;

	CString parse(CString &src, CString sub);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnDblClkData();
	afx_msg void OnSelChangeData();
	afx_msg void OnDblClkDisplay();
	afx_msg void OnSelChangeDisplay();
	afx_msg void OnDelete();
	afx_msg void OnDown();
	afx_msg void OnUp();
	afx_msg void OnInsert();
	afx_msg void OnModify();

};
