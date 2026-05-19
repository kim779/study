#pragma once

#include "resource.h"

// COption 대화 상자입니다.

class AFX_EXT_CLASS COption : public CDialogEx
{
	DECLARE_DYNAMIC(COption)

public:
	COption(CWnd* pParent = NULL)	{}   // 표준 생성자입니다.
	COption(struct _mapH* mapH, CWnd* pParent = NULL);
	virtual ~COption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GOPTION };
	BOOL	m_opt01;
	BOOL	m_opt02;
	BOOL	m_opt03;
	BOOL	m_opt04;
	BOOL	m_opt05;
	BOOL	m_opt06;
	BOOL	m_opt07;
	BOOL	m_opt08;
	BOOL	m_opt09;

protected:
	struct	_mapH*	m_mapH;
	CButton*	m_typeB[8];

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClick(UINT cmdID);
};
