#pragma once

#include "resource.h"

// CCellEdit 대화 상자입니다.

class AFX_EXT_CLASS CCellEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CCellEdit)

public:
	CCellEdit(CWnd* pParent, BYTE kind, BYTE charType, BYTE whatEdit, DWORD auxValues, CString editFormat,
		CString name, DWORD attributes, int size);   // 표준 생성자입니다.
	virtual ~CCellEdit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CEDIT };
	CString m_editFormat;
	CString m_title;

	BYTE	m_whatEdit;
	DWORD	m_auxValues;
	DWORD	m_attributes;

protected:
	BYTE	m_kind;
	BYTE	m_type;
	int	m_size;
	bool	m_first;
	CString	m_name;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEdit();
	afx_msg void OnColor();
	afx_msg void OnNOP();
	afx_msg void OnFormat();
	afx_msg void OnCoRGB();
};
