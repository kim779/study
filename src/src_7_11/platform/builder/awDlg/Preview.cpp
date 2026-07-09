// Preview.cpp : implementation file
//

#include "stdafx.h"
#include "Preview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreview dialog


CPreview::CPreview(CWnd* pParent /*=NULL*/)
	: CDialog(CPreview::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPreview)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPreview::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPreview)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPreview, CDialog)
	//{{AFX_MSG_MAP(CPreview)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreview message handlers

BOOL CPreview::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	SetWindowText(m_caption);
	m_pShow = new CShow(this, m_palette, m_draw, m_rect, m_root, m_filename);
	SetWindowPos(NULL, 0, 0, m_rect.Width() + GetSystemMetrics(SM_CXFRAME) * 2 + GetSystemMetrics(SM_CXEDGE), 
		m_rect.Height() + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYEDGE), SWP_NOMOVE | SWP_NOZORDER);
Invalidate();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CPreview::setShow(CAxisPalette* palette, CAxisDraw* draw, CRect rect, CString root, CString fileN, CString caption, CString backBrush)
{
	m_caption = caption;
	m_palette = palette;
	m_draw = draw;
	m_root = root;
	m_filename = fileN;
	if (backBrush.IsEmpty())
		m_pBackBrush = new CBrush(PALETTERGB(192, 192, 192));
	else
	{
		if (atoi(backBrush) & 0x02000000/*userCOLOR*/)
			m_pBackBrush = new CBrush(atoi(backBrush));
		else
			m_pBackBrush = new CBrush(m_palette->GetPaletteRGB(atoi(backBrush)));
	}
	m_rect = rect;
}

void CPreview::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_pShow)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = dc.SelectObject(m_pBackBrush); 
		
		dc.PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATCOPY);
		dc.SelectObject(oldBrush); 
		
		m_pShow->Load(m_filename, 1);
		m_pShow->Resize(m_rect);
		m_pShow->Draw(&dc);
	}
	// Do not call CDialog::OnPaint() for painting messages
}

void CPreview::OnClose() 
{
	if (m_pShow)
		delete m_pShow;
	if (m_pBackBrush)
	{
		m_pBackBrush->DeleteObject();
		delete m_pBackBrush;
	}
	CDialog::OnClose();
}

void CPreview::OnSize(UINT nType, int cx, int cy) 
{
	cx = m_rect.Width() + GetSystemMetrics(SM_CXFRAME) * 2 + GetSystemMetrics(SM_CXEDGE);
	cy = m_rect.Height() + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYFRAME) * 2 + GetSystemMetrics(SM_CYEDGE);
	CDialog::OnSize(nType, cx, cy);
		
}
