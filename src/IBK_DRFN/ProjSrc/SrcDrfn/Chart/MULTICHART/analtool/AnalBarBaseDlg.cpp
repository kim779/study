// AnalBarBaseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnalBarBaseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnalBarBaseDlg dialog


CAnalBarBaseDlg::CAnalBarBaseDlg(UINT nID, CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent)
{
	//{{AFX_DATA_INIT(CAnalBarBaseDlg)
	//}}AFX_DATA_INIT
//  m_hBrush = ::CreateSolidBrush(::GetSysColor(COLOR_BACKGROUND));
	m_hChart = NULL;
}

CAnalBarBaseDlg::~CAnalBarBaseDlg()
{
//	::DeleteObject(m_hBrush);
}


void CAnalBarBaseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnalBarBaseDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAnalBarBaseDlg, CDialog)
	//{{AFX_MSG_MAP(CAnalBarBaseDlg)
//	ON_WM_COPYDATA()
//	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnalBarBaseDlg message handlers

BOOL CAnalBarBaseDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CAnalBarBaseDlg::EnableDlgControls(bool bEnable)
{
}
	
void CAnalBarBaseDlg::ResizeControl(int cx, int cy)
{
}

LRESULT CAnalBarBaseDlg::SendMessageToChartViaMain(UINT nMsgId, LPVOID lpCode, DWORD dwParam)
{
	CWnd* pWnd = AfxGetMainWnd();
	if(!pWnd->GetSafeHwnd())
		return -1L;
	
	NMHDR nmhdr;
	::memset(&nmhdr, 0x00, sizeof(NMHDR));
	nmhdr.hwndFrom = m_hWnd;
	nmhdr.idFrom = nMsgId;
	nmhdr.code = (UINT)lpCode;

	return pWnd->SendMessage(WM_NOTIFY, (WPARAM)dwParam, (LPARAM)&nmhdr);
}
