#pragma once
#include "resource.h"

// CDiffViewerDlg 대화 상자입니다.

class AFX_EXT_CLASS CDiffViewerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDiffViewerDlg)

public:
	CDiffViewerDlg(CString strDiffViewer);   // 표준 생성자입니다.
	virtual ~CDiffViewerDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DIFF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strDiffViewer;
	CString m_LeftFile;
	CString m_RightFile;
public:
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
