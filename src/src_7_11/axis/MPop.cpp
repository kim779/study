// MPop.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "MPop.h"
#include "axmsg.hxx"
#include "baseMP.h"

#include "../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int osVISTA = 6;
/////////////////////////////////////////////////////////////////////////////
// CMPop dialog


CMPop::CMPop(CWnd* pParent /*=NULL*/)
	: CDialog(CMPop::IDD, pParent)
{
	m_actW = pParent;
	m_wnd = NULL;
	m_cursor = 0;

//	m_lock = false;
}
CMPop::~CMPop()
{
	if (m_wnd)	delete m_wnd;
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
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_AXIS, OnAXIS)
	ON_MESSAGE(WM_CHILDMSG, OnCHILDMSG)
//	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMPop message handlers
LRESULT CMPop::OnAXIS(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_AXIS, wParam, lParam);
	return 0;
}

LRESULT CMPop::OnCHILDMSG(WPARAM wParam, LPARAM lParam)
{
	AfxGetMainWnd()->SendMessage(WM_CHILDMSG, wParam, lParam);
	return 0;
}

BOOL CMPop::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int	gapX = 0;
	UINT	flag = SWP_SHOWWINDOW;

	m_level = osLevel();
	if (m_level >= osVISTA) gapX = 1;

	m_cxBorder  = GetSystemMetrics(SM_CXDLGFRAME);
	m_cyBorder  = GetSystemMetrics(SM_CYDLGFRAME);
	m_cyCaption = GetSystemMetrics(SM_CYCAPTION);

	switch (m_wpos)
	{
	case RtopPOS:		// right_top
		m_pos.x -= m_cxBorder;		break;

	case LbottomPOS:	// left_bottom
		m_pos.y -= m_cyBorder;		break;

	case RbottomPOS:	// right_bottom
		m_pos.x -= m_cxBorder;
		m_pos.y -= m_cyBorder;		break;
	default:
		flag &= ~SWP_SHOWWINDOW;
		break;		
	}

	if (!m_wnd) m_wnd = new CBaseMP();

	if (!m_wnd->GetSafeHwnd())
	{
		if (!m_wnd->Create(NULL, "", WS_VISIBLE|WS_CHILD, CRect(gapX, gapX, m_size.cx+gapX, m_size.cy+gapX), this, -1))
		{
			delete m_wnd; m_wnd = NULL;
			AfxMessageBox(ST_MSG_FAIL_WINDOWCREATE);
			return TRUE;
		}
	}

	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axMODAL, 0), (LPARAM)MAKELONG(m_size.cx, m_size.cy));
	SetWindowPos(&wndTopMost, m_pos.x, m_pos.y, m_size.cx+(m_cxBorder*2)+(gapX*2),
							m_size.cy+(m_cyBorder*2)+m_cyCaption+(gapX*2), flag);
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
	if (m_wnd && m_wnd->GetSafeHwnd())
		m_wnd->SetWindowPos(NULL, 0, 0, m_size.cx, m_size.cy, SWP_NOMOVE|SWP_NOZORDER);
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


void CMPop::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect	clientRC;

	if (m_level >= osVISTA)
	{
		HRGN	m_rgn;

		GetClientRect(&clientRC);
		clientRC.DeflateRect(m_cxBorder, m_cxBorder);

		clientRC.right += (m_cxBorder*2); clientRC.bottom += ((m_cyBorder*2) + m_cyCaption);
		m_rgn = ::CreateRectRgn(clientRC.left, clientRC.top, clientRC.right, clientRC.bottom);
		SetWindowRgn(m_rgn, TRUE);

		GetClientRect(&clientRC);
		dc.Draw3dRect(clientRC, RGB(44, 54, 139), RGB(44, 54, 139));
	}
}

int CMPop::osLevel()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	
	if(!(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)))
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
			return 0;
	}
	
	return osvi.dwMajorVersion;
}


