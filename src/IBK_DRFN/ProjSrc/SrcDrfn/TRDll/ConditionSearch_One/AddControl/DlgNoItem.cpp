// DlgNoItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgNoItem.h"

extern HINSTANCE g_hInstance;

// CDlgNoItem 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgNoItem, CDialog)

CDlgNoItem::CDlgNoItem(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNoItem::IDD, pParent)
{
	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_font);

	pParent->GetWindowRect(m_reParent);
}

CDlgNoItem::~CDlgNoItem()
{
}

void CDlgNoItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgNoItem, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgNoItem 메시지 처리기입니다.

void CDlgNoItem::OnBnClickedOk()
{
	KillTimer(0);
	m_font.DeleteObject();
	OnOK();
}

BOOL CDlgNoItem::OnInitDialog()
{	
	SetWindowPos(GetParent(), m_reParent.left + 150, m_reParent.bottom - 300, 
		m_reParent.right - 200, m_reParent.bottom + 50, SWP_NOSIZE);

	return TRUE;	
}

int CDlgNoItem::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);

	CRect reClinet;
	GetClientRect(reClinet);

	int x = reClinet.Width() / 2;
	int y = reClinet.Height() / 2;

	CRect reTemp;
	reTemp.left = reClinet.left + x - 200;
	reTemp.right = reTemp.left + 400;
	reTemp.top = reClinet.top + y - 10;
	reTemp.bottom = reTemp.top + 20;
	
	m_staticText.Create(_T("유효한 조건이 아닙니다. 설정된 조건을 확인 하시기 바랍니다."), WS_VISIBLE | WS_CHILD | SS_CENTERIMAGE | SS_CENTER,
		reTemp, this, 5000);
	m_staticText.SetFont(&m_font);

	SetTimer(0, 2000, 0);

	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CDlgNoItem::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 0)
	{
		OnBnClickedOk();
	}

	CDialog::OnTimer(nIDEvent);
}
