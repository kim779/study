// ManageInfo.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ManageInfo.h"
#include "mtable.h"
#include "multimon.h"
#include "axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManageInfo dialog
#define	TM_SLIDE	100
#define	TMI_SLIDE	100
#define	SLIDEGAP	10
#define	GAPX		4


CManageInfo::CManageInfo(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CManageInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManageInfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_home  = home;
	m_table = NULL;
	m_pos = 3;
}


void CManageInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManageInfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManageInfo, CDialog)
	//{{AFX_MSG_MAP(CManageInfo)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageInfo message handlers


void CManageInfo::OnDestroy() 
{
	CDialog::OnDestroy();
	
	if (m_table)
	{
		if (m_table->m_hWnd && ::IsWindow(m_table->m_hWnd))
			m_table->DestroyWindow();
		delete m_table;
	}
}

void CManageInfo::OnTimer(UINT_PTR nIDEvent) 
{
	switch (nIDEvent)
	{
	case 9999:
		{
			KillTimer(9999);
			HideSlide();
		}
		break;
	case TM_SLIDE:	Slide();break;
	default:		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CManageInfo::Init()
{
	int	x, y, gapX = 0, gapY = 0, GAP = 10, rowN = 2;

	if (!m_table)	m_table = new CMTable(m_home, rowN);

	if (!m_table->Create(NULL, "장운영 정보", 
		WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, (int) m_table))
	{
		delete m_table;
		m_table = NULL;
		return;
	}

	m_table->SetTitle("일이삼사오육칠팔구십일이삼사오가나다라마바사아자차");

	CRect	wRc, cRc, bRc;
	m_table->GetWindowRect(wRc);

	gapX += wRc.Width();
	gapY += wRc.Height();

	x = GetSystemMetrics(SM_CXSCREEN) - gapX - GAPX;
	y = GetSystemMetrics(SM_CYSCREEN) - gapY - GAPX;
	SetWindowPos(NULL, x, y, gapX, gapY, SWP_HIDEWINDOW);
	m_width  = gapX;
	m_height = gapY;
}

void    CManageInfo::SetContent(CString strdata)
{
	int	x, y, gapX = 0, gapY = 0, GAP = 10, rowN = 2;

	m_height = 120;
	if(strdata.IsEmpty())
		m_table->SetTitle("일이삼사오육칠팔구십일이삼사오가나다라마바사아자차");
	else
		m_table->SetContent(strdata);

	CRect	wRc, cRc, bRc;
	m_table->GetWindowRect(wRc);

	gapX += wRc.Width();
	gapY += wRc.Height();

	x = GetSystemMetrics(SM_CXSCREEN) - gapX - GAPX;
	y = GetSystemMetrics(SM_CYSCREEN) - gapY - GAPX;
	SetWindowPos(NULL, x, y, gapX, gapY, SWP_HIDEWINDOW);
	m_width  = gapX;
	m_height = gapY;
}

void CManageInfo::ChangePalette()
{
	if (m_table)	m_table->ChangePalette();
}

void CManageInfo::ChangeData()
{
	if (m_table)	m_table->ChangeData();
}

void CManageInfo::ShowSlide(CRect baseRC, int itime)
{
	CRect wRc;
	m_baseRc.CopyRect(&baseRC);
	m_table->GetWindowRect(wRc);

	int screenX = GetSystemMetrics(SM_CXMAXIMIZED);
	int screenY = GetSystemMetrics(SM_CYMAXIMIZED);

	int x, y;
	switch (m_pos)
	{
	case 0:
		x = m_baseRc.left + GetSystemMetrics(SM_CXFIXEDFRAME);
		y = m_baseRc.top + GetSystemMetrics(SM_CYFIXEDFRAME);
		break;
	case 1:
		x = m_baseRc.right - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
		y = m_baseRc.top + GetSystemMetrics(SM_CYFIXEDFRAME);
		break;
	case 2:
		x = m_baseRc.left + GetSystemMetrics(SM_CXFIXEDFRAME);
		y = m_baseRc.bottom - m_height - GetSystemMetrics(SM_CYFIXEDFRAME);
		break;
	case 3:
		x = screenX - m_width - 15;// - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
		y = screenY - wRc.Height() - 15;// - m_height - GetSystemMetrics(SM_CYFIXEDFRAME);
		//x = m_baseRc.right - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
		//y = m_baseRc.bottom - m_height - GetSystemMetrics(SM_CYFIXEDFRAME);
	}
	
	if (x < 0)	x = 0;
	if (y < 0)	y = 0;

	if (GetSystemMetrics(SM_CMONITORS) <= 1 && m_baseRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - m_width;
	if (m_baseRc.top < 0)
		y = 0;
	
	SetWindowPos(&wndTop, x, y, m_width, wRc.Height(), SWP_NOACTIVATE|SWP_SHOWWINDOW);
	SetTimer(9999, itime, NULL);
}

void CManageInfo::HideSlide()
{
	SetTimer(TM_SLIDE, TMI_SLIDE, NULL);
}

void CManageInfo::SetData(CString dat, int kind, int pos)
{
	m_pos = pos;
	if (m_table)
	{
		if (kind == 0)
			m_table->SetHeadColor(RGB(255,254,211));
		else
			m_table->SetColor();
		m_table->SetData(dat);
	}
}

void CManageInfo::Slide()
{
	CRect	wRc;
	int height, x, y;
	UINT uFlags = SWP_SHOWWINDOW;
	m_table->GetWindowRect(wRc);

	int screenX = GetSystemMetrics(SM_CXMAXIMIZED);
	int screenY = GetSystemMetrics(SM_CYMAXIMIZED);
	
	GetWindowRect(wRc);
	height  = wRc.Height() - SLIDEGAP;
	if (height <= 1)
	{
		KillTimer(TM_SLIDE);
		ShowWindow(SW_HIDE);
		GetParent()->SendMessage(WM_USER+500, axSLIDEEND, (LPARAM)0);
		CRect rc;
		GetWindowRect(rc);
		rc.top -= m_height;
		rc.top -= 15;
		SetWindowPos(&wndTop, rc.left, rc.top, m_width, m_height, SWP_HIDEWINDOW);
		return;
	}

	switch (m_pos)
	{
	case 0:
		x = m_baseRc.left + GetSystemMetrics(SM_CXFIXEDFRAME);
		y = m_baseRc.top + GetSystemMetrics(SM_CYFIXEDFRAME);
		break;
	case 1:
		x = m_baseRc.right - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
		y = m_baseRc.top + GetSystemMetrics(SM_CYFIXEDFRAME);
		break;
	case 2:
		x = m_baseRc.left + GetSystemMetrics(SM_CXFIXEDFRAME);
		y = m_baseRc.bottom - m_height - GetSystemMetrics(SM_CYFIXEDFRAME);
		break;
	case 3:
	//	x = m_baseRc.right - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
	//	y = m_baseRc.bottom - m_height - GetSystemMetrics(SM_CYFIXEDFRAME);
		x = screenX - m_width - 15;// - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
		y = screenY - wRc.Height() - 15;// - m_height - GetSystemMetrics(SM_CYFIXEDFRAME);
	}

	if (x < 0)	x = 0;	
	if (y < 0)	y = 0;
	
	if (GetSystemMetrics(SM_CMONITORS) <= 1 && m_baseRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - m_width;
	if (m_baseRc.top < 0)
		y = 0;
	
	if (GetFocus() != this)	
		uFlags = SWP_NOACTIVATE|SWP_SHOWWINDOW;

	SetWindowPos(&wndTop, x, y, m_width, height, uFlags);
	//m_table->SetWindowPos(&wndTop, x, y, m_width, height, uFlags);
}

void CManageInfo::StopSlide()
{
//	KillTimer(TM_SLIDE);
}
