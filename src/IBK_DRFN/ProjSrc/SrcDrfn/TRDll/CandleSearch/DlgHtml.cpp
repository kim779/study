// DlgHtml.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgHtml.h"


// CDlgHtml 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgHtml, CDialog)
CDlgHtml::CDlgHtml(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgHtml::IDD, pParent)
{
	m_pHtmlView = NULL;
	m_strURL = _T("");
}

CDlgHtml::~CDlgHtml()
{
	if(m_pHtmlView)
	{
		delete m_pHtmlView;
		m_pHtmlView = NULL;
	}
}
BOOL CDlgHtml::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pHtmlView = new CHelpHtmlCtrl();

	CRect rect;
	GetDlgItem(IDC_STATIC_TEXT)->GetWindowRect(rect);		
	ScreenToClient(&rect);		
	m_pHtmlView->CreateWnd(rect, this, IDC_STATIC_TEXT);

	ShowHtml();	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDlgHtml::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgHtml, CDialog)
END_MESSAGE_MAP()


void CDlgHtml::ShowHtml()
{
	m_pHtmlView->Navigate(m_strURL);
}

// CDlgHtml 메시지 처리기입니다.
