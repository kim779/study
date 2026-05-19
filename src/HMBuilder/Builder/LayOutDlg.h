#pragma once

#include "resource.h"
#include "afxwin.h"
#include "prewnd.h"

// CLayOutDlg 대화 상자입니다.

class CLayOutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLayOutDlg)

public:
	CLayOutDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLayOutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	bool	Init();
	void    SetRoot(CString sRoot){ m_sRoot = sRoot;  }
	CString	GetLayOutPath(){ return m_sLayOut;  }

private:

	CString m_sRoot;
	CString m_sLayOut;
	CMapStringToString m_Map;
	CPreWnd*	m_pWnd;;
	CFont	m_Font;


public:
	CListBox m_ListLayOut;
	afx_msg void OnBnClickedOk();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedDelete();
};
