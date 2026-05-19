#pragma once
#include "resource.h"
#include "afxwin.h"

// CCustomObjDlg 대화 상자입니다.

class AFX_EXT_CLASS CCustomObjDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomObjDlg)

public:
	CCustomObjDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCustomObjDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_CTMOBJ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void ObjList();
	CString m_strName;
public:
	virtual BOOL OnInitDialog();
	CListBox m_listboxObj;
	afx_msg void OnBnClickedOk();
};
