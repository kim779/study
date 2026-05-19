#pragma once

#include "resource.h"

// CBaseInfoPage 대화 상자입니다.

class CBaseInfoPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CBaseInfoPage)

public:
	CBaseInfoPage();
	virtual ~CBaseInfoPage();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_BASEINFO };
	CIPAddressCtrl m_addr;
	int m_displayMode;
	int m_gap;
	CString m_maps;
	CString m_path;

protected:
	CString	m_ips;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

private:
	CString	m_current;

	void getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBrowser();
	afx_msg void OnBrowser2();
	afx_msg void OnDisplaydataMode();

};
