// HelpOK.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "HelpOK.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHelpOK dialog


CHelpOK::CHelpOK(CWnd* pParent /*=NULL*/)
	: CDialog(CHelpOK::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHelpOK)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CHelpOK::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHelpOK)
	DDX_Control(pDX, IDC_HBITMAP, m_static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHelpOK, CDialog)
	//{{AFX_MSG_MAP(CHelpOK)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHelpOK message handlers

BOOL CHelpOK::OnInitDialog() 
{
	CDialog::OnInitDialog();

#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif

	
	CString	imgN;
	imgN.Format("%s\\image\\remote_ok.bmp", Axis::home);
	m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			imgN,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);	
	if(m_bitmap)
		m_static.SetBitmap(m_bitmap);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CHelpOK::OnEraseBkgnd(CDC* pDC) 
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

HBRUSH CHelpOK::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CHelpOK::OnDestroy() 
{
	CDialog::OnDestroy();
#ifndef DF_USE_CPLUS17
	delete m_changer;
#endif
}
