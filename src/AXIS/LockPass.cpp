// LockPass.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "LockPass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLockPass dialog


CLockPass::CLockPass(CWnd* pParent /*=NULL*/)
	: CDialog(CLockPass::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLockPass)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
} 


void CLockPass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLockPass)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLockPass, CDialog)
	//{{AFX_MSG_MAP(CLockPass)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(9898, OnBtnCert)
	ON_BN_CLICKED(9897, OnBtnAxisClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLockPass message handlers

void CLockPass::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}



BOOL CLockPass::OnEraseBkgnd( CDC* pDC )
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);
		
		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);
		
		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CLockPass::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);
	
	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CLockPass::OnBtnCert()
{
	std::unique_ptr<char[]> pdata = std::make_unique<char[]>(10);
	m_pParent->SendMessage(WD_LOCKPASS, 0, (long)pdata.get());

	CString slog;
	slog.Format("%s", pdata.get());
	if (slog.Find("success") >= 0)
		OnOK();
}

BOOL CLockPass::OnInitDialog()
{
	CDialog::OnInitDialog();
	CRect rec;
	CWnd* pwnd = (CWnd*)GetDlgItem(IDC_BTN_CERT);
	pwnd->GetWindowRect(rec);
	ScreenToClient(rec);

#ifdef DF_USE_CPLUS17
	m_btn_cert = std::make_unique<CImgBtn>();
#else
	m_btn_cert = new CImgBtn();
#endif

	m_btn_cert->Create("", WS_CHILD | WS_VISIBLE | BS_BITMAP, rec, this, 9898);

	m_btn_cert->m_hBitmap = Axis::GetBitmap("btn_certificate_ok_normal")->operator HBITMAP();
	m_btn_cert->m_hBitmap_dn = Axis::GetBitmap("btn_certificate_ok_click")->operator HBITMAP();
	m_btn_cert->m_hBitmap_hv = Axis::GetBitmap("btn_certificate_ok_over.bmp")->operator HBITMAP();

	m_btn_cert->FitImageSize();
	// TODO: Add extra initialization here


	pwnd = (CWnd*)GetDlgItem(IDC_BTN_AXIS);
	pwnd->GetWindowRect(rec);
	ScreenToClient(rec);

#ifdef DF_USE_CPLUS17
	m_btn_HTS_Close = std::make_unique<CImgBtn>();
#else
	m_btn_HTS_Close = new CImgBtn();
#endif

	m_btn_HTS_Close->Create("HTSÁ¾·á", WS_CHILD | WS_VISIBLE | BS_BITMAP, rec, this, 9897);

	m_btn_HTS_Close->m_hBitmap = Axis::GetBitmap("5BTN")->operator HBITMAP();
	m_btn_HTS_Close->m_hBitmap_dn = Axis::GetBitmap("5BTN_DN")->operator HBITMAP();
	m_btn_HTS_Close->m_hBitmap_hv = Axis::GetBitmap("5BTN_EN.bmp")->operator HBITMAP();

	m_btn_HTS_Close->FitImageSize();
	m_btn_HTS_Close->ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CLockPass::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CLockPass::OnBtnAxisClose()
{
	// TODO: Add your control notification handler code here
	m_pParent->SendMessage(WM_CLOSE, 0, 0);
}