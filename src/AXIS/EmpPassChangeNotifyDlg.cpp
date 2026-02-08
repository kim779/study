// EmpPassChangeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "EmpPassChangeNotifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmpPassChangeDlg dialog


CEmpPassChangeNotifyDlg::CEmpPassChangeNotifyDlg(CWnd* pParent, PSWD_NOTIFY_TYPE type)
: CDialog(CEmpPassChangeNotifyDlg::IDD, pParent), m_type(type)
{
	//{{AFX_DATA_INIT(CEmpPassChangeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEmpPassChangeNotifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmpPassChangeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmpPassChangeNotifyDlg, CDialog)
	//{{AFX_MSG_MAP(CEmpPassChangeDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpPassChangeDlg message handlers

void CEmpPassChangeNotifyDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	
// 	CString emsg;
// 	emsg.Format("%d/%d %d/%d %X\n", rc.left, rc.top, rc.Width(), rc.Height(), m_hBack);
// 	OutputDebugString(emsg);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap *pBmp = CBitmap::FromHandle(m_hBack);
	HBITMAP pOldBmp = (HBITMAP)memDC.SelectObject(pBmp);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();

}

void CEmpPassChangeNotifyDlg::OnDestroy() 
{
	::DeleteObject(m_hBack);
	m_font.DeleteObject();

	CDialog::OnDestroy();
}

BOOL CEmpPassChangeNotifyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_hBack = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMPPASSCHANGENOTIFY));
	if (!m_hBack)
	{
		CString emsg;
		emsg.Format("Image is null - %d\n", GetLastError());
		OutputDebugString(emsg);
	}

	CRect rc;
	
	GetDlgItem((m_type==PNT_ADVISE) ? IDC_PHOLDER_1 : IDC_PHOLDER_3)->GetWindowRect(&rc); ScreenToClient(rc);
	m_btnOk.Create("비밀번호변경", WS_CHILD|WS_VISIBLE|BS_BITMAP, rc, this, IDOK);
	m_btnOk.m_hBitmap    = Axis::GetBitmap("6BTN_조회")->operator HBITMAP();
	m_btnOk.m_hBitmap_dn = Axis::GetBitmap("6BTN_조회_DN")->operator HBITMAP();
	m_btnOk.m_hBitmap_hv = Axis::GetBitmap("6BTN_조회_EN")->operator HBITMAP();	


	GetDlgItem(IDC_PHOLDER_2)->GetWindowRect(&rc); ScreenToClient(rc);
	m_btnLater.Create("다음 기회에", WS_CHILD|WS_VISIBLE|BS_BITMAP, rc, this, IDCANCEL);
	m_btnLater.m_hBitmap    = Axis::GetBitmap("6BTN")->operator HBITMAP();
	m_btnLater.m_hBitmap_dn = Axis::GetBitmap("6BTN_DN")->operator HBITMAP();
	m_btnLater.m_hBitmap_hv = Axis::GetBitmap("6BTN_EN")->operator HBITMAP();
	
	CWnd *pLabel1 = GetDlgItem(IDC_INFORMATION1);
	CWnd *pLabel2 = GetDlgItem(IDC_INFORMATION2);

	{
		LOGFONT lf;
		GetFont()->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		m_font.CreateFontIndirect(&lf);
	}
	
	pLabel1->SetFont(&m_font);
	pLabel2->SetFont(&m_font);

	if (m_type==PNT_ADVISE)
	{
		m_btnOk.EnableWindow(TRUE);
		m_btnLater.EnableWindow(TRUE);
		pLabel1->SetWindowText("현재 비밀번호는 사용기간이 얼마 남지 않았습니다.");
		pLabel2->SetWindowText("정보보호를 위하여 새로운 비밀번호로 변경하십시오.");
	}
	else
	{
		m_btnOk.EnableWindow(TRUE);
		m_btnLater.EnableWindow(FALSE);
		m_btnLater.ShowWindow(SW_HIDE);
		pLabel1->SetWindowText("현재 비밀번호는 사용기간이 만료되었습니다.");
		pLabel2->SetWindowText("정보보호를 위하여 새로운 비밀번호로 변경하십시오.");
	}


	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CEmpPassChangeNotifyDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	const int nCtrlID = pWnd->GetDlgCtrlID();
	if (nCtrlID==IDC_INFORMATION1 || nCtrlID==IDC_INFORMATION2)
	{
		HGDIOBJ hBrush = GetStockObject(NULL_BRUSH);
		SetBkMode(pDC->m_hDC, TRANSPARENT);
		return (HBRUSH)hBrush;
	}
	return hbr;
}
