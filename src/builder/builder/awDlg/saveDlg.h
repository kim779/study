#pragma once

#include "resource.h"

// CSaveDlg 대화 상자입니다.

class AFX_EXT_CLASS CSaveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveDlg)

public:
	CSaveDlg(CWnd* pParent, CString path);   // 표준 생성자입니다.
	virtual ~CSaveDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PATH };
	CString m_path;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBrowser();
};
