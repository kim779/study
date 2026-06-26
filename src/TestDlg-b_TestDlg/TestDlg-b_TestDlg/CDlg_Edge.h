#pragma once


// CDlg_Edge 대화 상자
#include "BrowserWindowEdge.h"
#include "CBrowserWnd.h"
#include "CBrowserComp.h"


class CDlg_Edge : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg_Edge)

public:
	CDlg_Edge(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlg_Edge();

	CBrowserWnd* m_pBrowserWnd{};

	CBrowserComp* m_pBrowserCmp{};

	CString m_slog;

	//Edge window
	std::unique_ptr<BrowserWindowEdge> browserEdge;
	void Checkenv();
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EDGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
