// MPop.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "MPop.h"
#include "axmsg.hxx"
#include "baseMP.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define FIXEDEX		81
/////////////////////////////////////////////////////////////////////////////
// CMPop dialog


CMPop::CMPop(CWnd* pParent /*=NULL*/)
	: CDialog(CMPop::IDD, pParent)
{
	m_actW = pParent;
	//{{AFX_DATA_INIT(CMPop)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_wnd = NULL;
	m_cursor = 0;
	m_bDll	= false;
//	m_lock = false;
}
CMPop::~CMPop()
{
#ifndef DF_USE_CPLUS17
	if (m_wnd)	delete m_wnd;
#endif
}

void CMPop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMPop)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMPop, CDialog)
	//{{AFX_MSG_MAP(CMPop)
	ON_WM_CLOSE()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPop message handlers
LONG CMPop::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_AXIS, wParam, lParam);
	return 0;
}

LONG CMPop::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_CHILDMSG, wParam, lParam);
	return 0;
}

BOOL CMPop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int	cx{}, cy{};
	CRect	cRc, wRc;
	UINT	flag = SWP_SHOWWINDOW;

	GetClientRect(cRc);
	GetWindowRect(wRc);
	cx = abs(wRc.Width() - cRc.Width());
	cy = abs(wRc.Height() - cRc.Height());
	switch (m_wpos)
	{
	case RtopPOS:		// right_top
		m_pos.x -= cx;		break;

	case LbottomPOS:	// left_bottom
		m_pos.y -= cy;		break;

	case RbottomPOS:	// right_bottom
		m_pos.x -= cx;
		m_pos.y -= cy;
	default:
		flag &= ~SWP_SHOWWINDOW;
		break;		
	}
#ifdef DF_USE_CPLUS17
	if (!m_wnd)	m_wnd = std::make_unique<CBaseMP>();
#else
	if (!m_wnd)	m_wnd = new CBaseMP();
#endif
	const CSize xx = ((CMainFrame*)m_main)->GetFrameGap();
	m_wnd->m_bDll = m_bDll;

	//cx = GetSystemMetrics(SM_CXDLGFRAME) * 2;
	//cy = GetSystemMetrics(SM_CYDLGFRAME) * 2;

	if (!m_wnd->GetSafeHwnd())
	{
		if (!m_wnd->Create(NULL, "", WS_VISIBLE|WS_CHILD, CRect(0, 0, m_size.cx +cx , m_size.cy +cy), this, -1))
		{
#ifndef DF_USE_CPLUS17
			delete m_wnd;
#endif
			m_wnd = NULL;
			Axis::MessageBox(ST_MSG_FAIL_WINDOWCREATE);
			return TRUE;
		}
	}

	if ((m_vwTYPE == FIXEDEX) &&(m_mapN.Mid(2,2)=="94")) ModifyStyle(WS_SYSMENU, NULL, SWP_DRAWFRAME);

	const int width = m_size.cx + xx.cx;
	const int height = m_size.cy + xx.cy;
	SetWindowPos(NULL, m_pos.x, m_pos.y, width , height, flag);
	//FitWindow();

	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axMODAL, 0), (LPARAM)MAKELONG(m_size.cx, m_size.cy));

	m_wnd->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMPop::OnOK() 
{
	SendMessage(WM_KEYDOWN, (WPARAM)VK_RETURN, (LPARAM)0);
	//CDialog::OnOK();
}

void CMPop::OnClose() 
{
//	if (m_lock)
//		EndDialog(IDCANCEL);
//	else
		EndDialog(IDOK);
	CDialog::OnClose();
}

void CMPop::SetPos(int x, int y)
{
	m_pos.x = x;
	m_pos.y = y;
}

void CMPop::SetSize(int cx, int cy)
{
	m_size.cx = cx;
	m_size.cy = cy;

	/**
	m_size.cy += GetSystemMetrics(SM_CYCAPTION);
	m_size.cy += GetSystemMetrics(SM_CYFRAME) * 2;
	m_size.cx += GetSystemMetrics(SM_CXFRAME) * 2;
	**/

//	if (m_wnd && m_wnd->GetSafeHwnd())
//		m_wnd->SetWindowPos(NULL, 0, 0, m_size.cx, m_size.cy, SWP_NOMOVE|SWP_NOZORDER);
}

BOOL CMPop::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_cursor == 1)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;	
	}	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CMPop::OnCancel() 
{
	EndDialog(IDOK);	
//	CDialog::OnCancel();
}

void CMPop::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	FitWindow();	
}

void CMPop::FitWindow()
{
	if (m_wnd && m_wnd->GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(&rc);
		m_wnd->SetWindowPos(NULL, 0, 0, rc.Width() + 50, rc.Height() + 20, SWP_NOZORDER);
	}
}

BOOL CMPop::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		
		if( pMsg->wParam == VK_ESCAPE )
		{
			if ((m_vwTYPE == FIXEDEX) &&(m_mapN.Mid(2,2)=="94"))
				return TRUE;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
