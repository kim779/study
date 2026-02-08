// EConclusion.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "EConclusion.h"
#include "ntable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_SLIDE	100
#define	TMI_SLIDE	100
#define	SLIDEGAP	10
#define	GAPX		4
/////////////////////////////////////////////////////////////////////////////
// CEConclusion dialog


CEConclusion::CEConclusion(CWnd* pParent /*=NULL*/)
	: CDialog(CEConclusion::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEConclusion)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_table = NULL;
}

CEConclusion::~CEConclusion()
{
#ifndef DF_USE_CPLUS17
	if (m_table)	delete m_table;
#endif
}

void CEConclusion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEConclusion)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEConclusion, CDialog)
	//{{AFX_MSG_MAP(CEConclusion)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEConclusion message handlers

BOOL CEConclusion::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEConclusion::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TM_SLIDE:	Slide();break;
	default:		break;
	}
	CDialog::OnTimer(nIDEvent);
}

const char* section = "EACHCONCLUSION";

void CEConclusion::Init()
{
	int	x{}, y{}, gapX{}, gapY{}, rowN{};
	const int GAP = 10;
	CString	keys;

	CProfile profile(GetProfileFileName(pkSTInfo));
	rowN = profile.GetInt(section, "rowN", 5);

#ifdef DF_USE_CPLUS17
	if (!m_table.get())
		m_table = std::make_unique<CNTable>(rowN);
	if (!m_table->Create(NULL, "EACH CONCLUSION",
		WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, (int)m_table.get()))
	{
		m_table = NULL;
		return;
	}
#else
	if (!m_table)	m_table = new CNTable(rowN);
	if (!m_table->Create(NULL, "EACH CONCLUSION", 
		WS_VISIBLE|WS_CHILD|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, (int) m_table))
	{
		delete m_table;
		m_table = NULL;
		return;
	}
#endif

	CStringArray	ary;
	for (int ii = 0; ii < rowN; ii++)
	{
		keys.Format("title%02d", ii);
		ary.Add(profile.GetString(section, keys, keys));
	}
	m_table->SetTitle(ary, "  0123456789012345  ");
	ary.RemoveAll();

	CRect	wRc;
	const CRect bRc;
	m_table->GetWindowRect(wRc);

	gapX  = GetSystemMetrics(SM_CXFIXEDFRAME)*2;
	gapX += GetSystemMetrics(SM_CXBORDER) * 4 + wRc.Width();

	gapY  = GetSystemMetrics(SM_CYFIXEDFRAME)*2;
	gapY += GetSystemMetrics(SM_CYBORDER) * 4 + wRc.Height();

	x = GetSystemMetrics(SM_CXSCREEN) - gapX - GAPX;
	y = GetSystemMetrics(SM_CYSCREEN) - gapY - GAPX;
	SetWindowPos(NULL, x, y, gapX, gapY, SWP_HIDEWINDOW);
	m_width  = gapX;
	m_height = gapY;
}

void CEConclusion::ChangePalette()
{
	if (m_table)	m_table->ChangePalette();
}

void CEConclusion::ChangeData()
{
	if (m_table)	m_table->ChangeData();
}

void CEConclusion::ShowSlide(CRect baseRC)
{
	m_baseRc.CopyRect(&baseRC);
	int x = m_baseRc.right - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
	int y = m_baseRc.bottom - m_height - GetSystemMetrics(SM_CYFIXEDFRAME);

	if (x < 0)	x = 0;	if (y < m_height)	y = m_height;
	if (m_baseRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - m_width;
	if (m_baseRc.bottom > GetSystemMetrics(SM_CXSCREEN))
		y = GetSystemMetrics(SM_CXSCREEN) - m_height;

	SetWindowPos(&wndTop, x, y, m_width, m_height, SWP_NOACTIVATE|SWP_SHOWWINDOW);
}

void CEConclusion::HideSlide()
{
	SetTimer(TM_SLIDE, TMI_SLIDE, NULL);
}

// 통보제목\t계좌\t종목명\t매매구분\t체결수량\t체결가격
void CEConclusion::SetData(CString dat, bool bFail)
{
/*
	ignore title
	int pos = dat.Find('\t');
	if (pos == -1)	return;
	CString title = dat.Left(pos++);
	dat = dat.Mid(pos);

	pos = title.Find('(');
	if (pos != -1)
		title = title.Left(pos);
	title.TrimRight();
	SetWindowText(title);
*/	
	if (bFail)
		m_table->SetHeadColor(RGB(215, 0, 0));
	if (m_table)	m_table->SetData(dat);
}

void CEConclusion::Slide()
{
	CRect	wRc;
	int height{}, x{}, y{};
	UINT uFlags = SWP_SHOWWINDOW;
	
	GetWindowRect(wRc);
	height  = wRc.Height() - SLIDEGAP;
	if (height <= 10)
	{
		KillTimer(TM_SLIDE);
		ShowWindow(SW_HIDE);
		return; 
	}

	x = m_baseRc.right - m_width - GetSystemMetrics(SM_CXFIXEDFRAME);
	y = m_baseRc.bottom - height - GetSystemMetrics(SM_CYFIXEDFRAME);

	if (x < 0)	x = 0;	if (y < m_height)	y = m_height;
	if (m_baseRc.right > GetSystemMetrics(SM_CXSCREEN))
		x = GetSystemMetrics(SM_CXSCREEN) - m_width;
	if (m_baseRc.bottom > GetSystemMetrics(SM_CXSCREEN))
		y = GetSystemMetrics(SM_CXSCREEN) - height;

	if (GetFocus() != this)	uFlags = SWP_NOACTIVATE|SWP_SHOWWINDOW;
	SetWindowPos(&wndTop, x, y, m_width, height, uFlags);
}

void CEConclusion::ChangeTitle(int index, CString title)
{
	if (m_table)	m_table->ChangeTitle(index, title);
}

void CEConclusion::ChangeTitleK(int kind)
{
	if (!m_table)	return;
	
	int	ii{};

	CProfile profile(GetProfileFileName(pkSTInfo));

	switch (kind)
	{
	case 101:		// Buy Order
	case 102:		// sell Order
		for (ii = 2; ii < 5; ii++)
		{
			m_table->ChangeTitle(ii, profile.GetString(section, Format("orderT%02d", ii)));
		}
		break;
	case 103:		// Modify Order
		for (ii = 2; ii < 5; ii++)
		{
			m_table->ChangeTitle(ii, profile.GetString(section, Format("modifyT%02d", ii)));
		}
		break;
	case 104:		// Cancel Order
		for (ii = 2; ii < 5; ii++)
		{
			m_table->ChangeTitle(ii, profile.GetString(section, Format("cancelT%02d", ii)));
		}
		break;
	default:
		return;
	}
}

void CEConclusion::StopSlide()
{
	KillTimer(TM_SLIDE);
}
