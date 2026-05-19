#pragma once
#include "afxwin.h"
#include "resource.h"

// CCheckOutDlg 대화 상자입니다.

class CCheckOutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckOutDlg)

public:
	CCheckOutDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCheckOutDlg();


	void SetFormName(CString sName){ m_sMapN = sName; }

// 대화 상자 데이터입니다.
	enum { IDD = IDD_CHEKCOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_sMapN;
//	virtual void PreInitDialog();
	virtual BOOL OnInitDialog();
};
