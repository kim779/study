// SecureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "SecureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSecureDlg dialog


CSecureDlg::CSecureDlg(CWnd* pParent, BOOL bAOS, BOOL bFirewall, BOOL bKeysecure)
	: CDialog(CSecureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSecureDlg)
	m_bNoRetry = FALSE;
	//}}AFX_DATA_INIT
	m_bAOS = bAOS;
	m_bFirewall = bFirewall;
	m_bKeysecure = bKeysecure;
#ifndef DF_USE_CPLUS17
	m_btn_yes = m_btn_no = NULL;
#endif

#ifdef LOG_MODE
	m_hDebug = CreateFile(_T("C:\\secure_dlg.log"), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hDebug==INVALID_HANDLE_VALUE)
		MessageBox("디버그파일 오픈 실패!");
#endif
}


void CSecureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSecureDlg)
	DDX_Check(pDX, IDC_NORETRY, m_bNoRetry);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSecureDlg, CDialog)
	//{{AFX_MSG_MAP(CSecureDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SECUREDLG_YES, OnOkClick)
	ON_BN_CLICKED(IDC_SECUREDLG_NO , OnCancelClick)
END_MESSAGE_MAP()


void CSecureDlg::OnOkClick()
{
	UpdateData();
	CDialog::OnOK();
}

void CSecureDlg::OnCancelClick()
{
	UpdateData();
	CDialog::OnCancel();
}
/////////////////////////////////////////////////////////////////////////////
// CSecureDlg message handlers

BOOL CSecureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bmp_excl = Axis::GetBitmap("알림_ic");
	m_bmp_arrow = Axis::GetBitmap("텍스트화살표_하단");

	m_font.CreateFont( 12, 0, 0, 0,	FW_NORMAL, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("굴림체")); 
	m_bfont.CreateFont( 12, 0, 0, 0, FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("굴림체")); 
#ifdef DF_USE_CPLUS17
	m_btn_yes = std::make_unique<CImgBtn>();
	m_btn_no = std::make_unique<CImgBtn>();
#else
	m_btn_yes = new CImgBtn();
	m_btn_no  = new CImgBtn();
#endif

	m_btn_yes->Create("예", WS_CHILD|WS_VISIBLE|BS_BITMAP, CRect(40,160,100,180), this, IDC_SECUREDLG_YES);
	m_btn_no->Create("아니오", WS_CHILD|WS_VISIBLE|BS_BITMAP, CRect(110,160,170,180), this, IDC_SECUREDLG_NO);

	m_btn_yes->m_hBitmap    = m_btn_no->m_hBitmap    = Axis::GetBitmap("4BTN")->operator HBITMAP();
	m_btn_yes->m_hBitmap_dn = m_btn_no->m_hBitmap_dn = Axis::GetBitmap("4BTN_DN")->operator HBITMAP();
	m_btn_yes->m_hBitmap_hv = m_btn_no->m_hBitmap_hv = Axis::GetBitmap("4BTN_EN")->operator HBITMAP();	

	m_btn_yes->FitImageSize();
	m_btn_no->FitImageSize();

	m_btn_yes->SetFocus();

	return FALSE;
}

void CSecureDlg::OnPaint() 
{
	COLORREF red{}, black{}, blue{};
	CString tmp;
	CFont* oldFont{};
	CPaintDC dc(this);

	CRect crc, rc;
	GetClientRect(crc);

	red = RGB(0xC8, 0, 0);
	black = RGB(0, 0, 0);
	blue = RGB(0, 0, 0xC0);

	const int width = crc.Width(), height = crc.Height();
	int ypos{}, gap;
	ypos = gap = 15;

	dc.SetTextColor(black);
	oldFont = dc.SelectObject(&m_bfont);

	// 백그라운드
	dc.FillSolidRect(crc, 0xF5F5F5);

	// 첫번째 줄
	rc.SetRect(0, ypos, width, ypos+gap);
	dc.DrawText( "[ 보안프로그램 실행 안내 ]", rc, DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	ypos += gap*2 + 5;

	dc.FillSolidRect(10, ypos-13, width-20, 60, 0xEBEBEB);

	// 두번째 줄
	DrawBitmap(&dc, m_bmp_excl, 20, ypos, 0xDDDDDD);
	rc.SetRect(40, ypos, width, ypos+gap);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(red);
	dc.DrawText( "현재 보안프로그램중 일부가 실행되고 있지 않습니다.", rc, DT_SINGLELINE|DT_LEFT|DT_VCENTER);
	ypos += gap + 10;

	// 세번째 줄
	dc.SetTextColor(0);
	dc.SetBkMode(TRANSPARENT);
	dc.TextOut(20, ypos, "메모리보호 : [        ]");
	dc.TextOut(197, ypos, "방화벽 : [        ]");
	dc.TextOut(347, ypos, "키보드보안 : [        ]");
	
	m_bAOS	? dc.SetTextColor(blue) : dc.SetTextColor(red);
	m_bAOS  ? dc.TextOut(122, ypos, "실행중") : dc.TextOut(122, ypos, "미실행");
	m_bFirewall  ? dc.SetTextColor(blue) : dc.SetTextColor(red);
	m_bFirewall  ? dc.TextOut(272, ypos, "실행중") : dc.TextOut(272, ypos, "미실행");
	m_bKeysecure ? dc.SetTextColor(blue) : dc.SetTextColor(red);
	m_bKeysecure ? dc.TextOut(449, ypos, "실행중") : dc.TextOut(449, ypos, "미실행");

	ypos += gap*2 + 5;

	dc.SetTextColor(0);
	dc.SelectObject(&m_font);
	
	DrawBitmap(&dc, m_bmp_arrow, 25, ypos, 0xFF00FF);
	dc.TextOut(35, ypos, "보안프로그램이 실행되지 않을경우 해킹에 노출될 수 있습니다.");
	
	ypos += gap+5;
	DrawBitmap(&dc, m_bmp_arrow, 25, ypos, 0xFF00FF);
	dc.TextOut(35, ypos, "보안프로그램을 실행하시겠습니까?");

	dc.SelectObject(oldFont);
}

void CSecureDlg::DrawBitmap(CDC *pDC, CBitmap *bmp, int x, int y, UINT rgb)
{
	CDC memDC;
	CBitmap* oldBmp{};
	memDC.CreateCompatibleDC(pDC);
	if (memDC.m_hDC && bmp->m_hObject)
	{
		BITMAP bm;
		GetObject(bmp->m_hObject, sizeof(BITMAP), &bm);

		oldBmp = memDC.SelectObject(bmp);
		//pDC->BitBlt(x, y, bm.bmWidth, bm.bmHeight, &memDC, 0, 0, SRCCOPY);
		TransparentBlt(pDC->m_hDC, x, y, bm.bmWidth, bm.bmHeight, memDC.m_hDC, 0, 0, bm.bmWidth, bm.bmHeight, rgb);
		memDC.SelectObject(oldBmp);
		memDC.DeleteDC();
	}
}

HBRUSH CSecureDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	const int nRet = pWnd->GetDlgCtrlID();
	if (nRet==IDC_NORETRY)
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}
	
	return hbr;
}

void CSecureDlg::OnClose() 
{
	UpdateData();
#ifndef DF_USE_CPLUS17
	if (m_btn_yes) delete m_btn_yes;
	if (m_btn_no)  delete m_btn_no;
#endif
	CDialog::OnClose();
}

BOOL CSecureDlg::DestroyWindow() 
{
	return CDialog::DestroyWindow();
}
