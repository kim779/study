#pragma once

#include "control/HelpHtmlCtrl.h"
#include "CandlePattern.h"	
#include "DlgHtml.h"

// CDlgHtml 대화 상자입니다.

class CDlgHtml : public CDialog
{
	DECLARE_DYNAMIC(CDlgHtml)

public:
	CDlgHtml(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgHtml();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_HTML };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CHelpHtmlCtrl* m_pHtmlView;
	void	ShowHtml();	


	CString m_strURL;
	void SetURL(LPCTSTR lpszURL){m_strURL = lpszURL;};
};
