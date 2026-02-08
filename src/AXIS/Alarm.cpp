// Alarm.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Alarm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlarm dialog

CAlarm::CAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlarm)
	m_msg = _T("");
	//}}AFX_DATA_INIT
	m_bitmap = NULL;
	m_btnClose = NULL;
}

CAlarm::~CAlarm()
{
}

void CAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlarm)
	DDX_Text(pDX, IDC_MSG, m_msg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlarm, CDialog)
	//{{AFX_MSG_MAP(CAlarm)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_EN_CHANGE(IDC_MSG, OnChangeMsg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlarm message handlers

BOOL CAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_bitmap = Axis::GetBitmap("message_bg");
	UpdateData(FALSE);
	GetDlgItem(IDC_MSG)->SetFocus();

	if (m_bitmap)
	{
		BITMAP bm;
		m_bitmap->GetBitmap(&bm);

		SetWindowPos(NULL, 0, 0, bm.bmWidth, bm.bmHeight, SWP_NOMOVE | SWP_NOZORDER);
		CenterWindow();

		CWnd* wnd = GetDlgItem(IDC_MSG);

		CRect rc(6 + 4, 26 + 4, 315, 223);

		wnd->MoveWindow(rc);
	}

	const int width = 15;
	const int height = 15;

#ifdef DF_USE_CPLUS17
	m_btnClose = std::make_unique< CShapeButton>();
#else
	m_btnClose = new CShapeButton;
#endif
	m_btnClose->SubclassWindow(GetDlgItem(IDOK)->m_hWnd);
	m_btnClose->SetImgBitmap("GRAY_Ã¢´Ý±â", 2);

	CRect rc;
	GetClientRect(rc);

	m_btnClose->SetWindowPos(NULL, rc.right - width - 5, 3, width, height, SWP_NOZORDER | SWP_SHOWWINDOW);

	GetDlgItem(IDC_MSG)->HideCaret();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlarm::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rc;
	GetClientRect(rc);
	rc.bottom = 19;
	if (rc.PtInRect(point))
		PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

BOOL CAlarm::OnEraseBkgnd(CDC* pDC) 
{
	if (m_bitmap)
	{
		CRect rc;
		GetClientRect(rc);

		CDC mdc;
		mdc.CreateCompatibleDC(pDC);
		CBitmap* oldBmp = mdc.SelectObject(m_bitmap);
		pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
		mdc.SelectObject(oldBmp);
		
		return TRUE;
	}

	return CDialog::OnEraseBkgnd(pDC);
}


void CAlarm::OnChangeMsg() 
{
	UpdateData(FALSE);
}

void CAlarm::OnDestroy() 
{
#ifndef DF_USE_CPLUS17
	delete m_btnClose;
#endif
	CDialog::OnDestroy();
}
