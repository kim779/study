#pragma once

#include "resource.h"

// CFormat 대화 상자입니다.

class AFX_EXT_CLASS CFormat : public CDialogEx
{
	DECLARE_DYNAMIC(CFormat)

public:
	CFormat(CWnd* pParent, BYTE whatEdit, CString editFormat, DWORD attributes, CString name, int size);   // 표준 생성자입니다.
	virtual ~CFormat();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FORMAT };
	CString m_editFormat;
	CString m_name;

	BYTE	m_whatEdit;
	DWORD	m_attributes;
	int	m_size;
	CString	m_edit;

protected:
	bool	m_first;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeFormat();
	afx_msg void OnNoEdit();
};
