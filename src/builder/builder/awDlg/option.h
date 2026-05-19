#pragma once

#include "resource.h"

// COption 대화 상자입니다.

class AFX_EXT_CLASS COption : public CDialogEx
{
	DECLARE_DYNAMIC(COption)

public:
	COption(CWnd* pParent = NULL)	{}   // 표준 생성자입니다.
	COption(class mapForm* mapForm, CWnd* pParent = NULL);
	virtual ~COption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GOPTION };
	BOOL	m_opt01;
	BOOL	m_opt04;
	BOOL	m_opt06;
	BOOL	m_opt09;
	BOOL	m_opt11;
	BOOL	m_opt12;
	BOOL	m_opt15;
	BOOL	m_opt16;
	BOOL	m_opt17;
	BOOL	m_opt18;
	BOOL	m_opt19;
	BOOL	m_opt22;
	BOOL	m_opt23;
	BOOL	m_opt24;
	BOOL	m_opt25;
	BOOL	m_opt26;
	BOOL	m_opt27;

protected:
	class mapForm*	m_mapH;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClick(UINT cmdID);
};
