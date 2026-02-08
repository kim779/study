// Info_Interest.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Info_Interest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInfo_Interest dialog


CInfo_Interest::CInfo_Interest(CWnd* pParent /*=NULL*/)
	: CDialog(CInfo_Interest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInfo_Interest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_backImg =	Axis::GetBitmap("AXISINFO");
}


void CInfo_Interest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInfo_Interest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInfo_Interest, CDialog)
	//{{AFX_MSG_MAP(CInfo_Interest)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInfo_Interest message handlers

void CInfo_Interest::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}

void CInfo_Interest::OnOK()
{
	
	//
}

void CInfo_Interest::SetOK()
{
	CDialog::OnOK();
}

void CInfo_Interest::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc; GetClientRect(rc);
	
	if (m_backImg)
	{
		CDC cdc; 
		cdc.CreateCompatibleDC(&dc);
		cdc.SelectObject(m_backImg);
		dc.BitBlt(rc.left,rc.top,rc.Width(),rc.Height(), &cdc, 0, 0, SRCCOPY);
		//dc.BitBlt(0,0,300,20,&cdc,0,0,SRCCOPY);
	}
	else
		dc.Rectangle(rc);
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

HBRUSH CInfo_Interest::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	const int ids = ::GetWindowLong(pWnd->m_hWnd, GWL_ID);
	
	if (ids == IDC_STATIC_INFO)
	{
		//CBrush br;
		//br.CreateSolidBrush(RGB(255,255,255));
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&Axis::fondBold);
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH)GetStockObject(NULL_BRUSH);;
	}
	

	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CInfo_Interest::OnDestroy() 
{
	CDialog::OnDestroy();
	if (m_backImg)	DeleteObject(m_backImg);
	// TODO: Add your message handler code here
	
}
