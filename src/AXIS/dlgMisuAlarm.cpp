#include "stdafx.h"
#include "axis.h"
#include "dlgMisuAlarm.h"
#include "axmsg.hxx"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDlgMisuAlarm::CDlgMisuAlarm(CWnd* pParent)
	: CDialog(CDlgMisuAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMisuAlarm)
	//}}AFX_DATA_INIT
	m_mainframe = pParent;
	m_brush.CreateSolidBrush(RGB(238, 238, 238));
}

CDlgMisuAlarm::~CDlgMisuAlarm()
{
	m_brush.DeleteObject();
}

void CDlgMisuAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMisuAlarm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMisuAlarm, CDialog)
	//{{AFX_MSG_MAP(CDlgMisuAlarm)
	ON_BN_CLICKED(IDC_MAP, OnMap)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDlgMisuAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;
}

void CDlgMisuAlarm::Init()
{
	CRect wRC, mRC;
	GetWindowRect(wRC);
	const CSize szMon = CSize(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	mRC.left = szMon.cx / 2 - wRC.Width() / 2;
	mRC.right = mRC.left + wRC.Width();
	mRC.top = szMon.cy / 2 - wRC.Height() / 2;
	mRC.bottom = mRC.top + wRC.Height();

	SetWindowPos(NULL, mRC.left, mRC.top, mRC.Width(), mRC.Height(), SWP_HIDEWINDOW);
}

void CDlgMisuAlarm::OnMap() 
{
	if (m_mainframe)
		m_mainframe->SendMessage(WM_AXIS, MAKEWPARAM(axMinuAlarm, 0), 0);
}

void CDlgMisuAlarm::OnOK() 
{

	CDialog::OnOK();
}

HBRUSH CDlgMisuAlarm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	const int nID = pWnd->GetDlgCtrlID();
	switch (nID)
	{
	case IDC_STATIC1:	case IDC_STATIC2:	case IDC_STATIC3:
	case IDC_STATIC4:	case IDC_STATIC5:	case IDC_STATIC6:
	case IDC_STATIC7:	case IDC_STATIC8:	case IDC_STATIC9:
	case IDC_STATIC10:	case IDC_STATIC11:
		hbr = m_brush;
		if (nID == IDC_STATIC4 || nID == IDC_STATIC8 || nID == IDC_STATIC9 ||
			nID == IDC_STATIC10)
			pDC->SetTextColor(RGB(255, 0, 0));
		else
			pDC->SetTextColor(RGB(0, 0, 0));
		pDC->SetBkMode(TRANSPARENT);
		break;
	default:
		hbr = m_brush;
		break;
	}
	return hbr;
}

void CDlgMisuAlarm::OnDestroy() 
{
	CDialog::OnDestroy();
}
