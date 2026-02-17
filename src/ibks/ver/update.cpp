// update.cpp : implementation file
//

#include "stdafx.h"
#include "axisver.h"
#include "update.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUpdate dialog


CUpdate::CUpdate(CString root, CString images, CWnd* pParent /*=NULL*/)
	: CDialog(CUpdate::IDD, pParent)
{
	m_root = root;
	m_images = images;
	m_bitmap = (HBITMAP) NULL;
	if (m_images.IsEmpty())
		m_images = _T("AXISUPDATE.BMP");
	//{{AFX_DATA_INIT(CUpdate)
	//}}AFX_DATA_INIT
}

CUpdate::~CUpdate()
{
	m_font.DeleteObject();
	if (m_bitmap)
		DeleteObject(m_bitmap);
}


void CUpdate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUpdate)
	DDX_Control(pDX, IDC_BITMAP, m_static);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUpdate, CDialog)
	//{{AFX_MSG_MAP(CUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUpdate message handlers

BOOL CUpdate::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString	path;
	CWinApp* app = AfxGetApp();

	m_font.DeleteObject();
	m_font.CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE,
			false, false, HANGEUL_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, "");
	SetFont(&m_font);

	path.Format("%s\\%s\\%s", m_root, IMAGEDIR, m_images);
	m_bitmap = (HBITMAP) LoadImage(NULL, path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_bitmap)
	{
		CSize	size, gap;
		BITMAP	bi;
		CBitmap	bitmap;
		CRect	cRc, wRc;
		CWnd	*item;
		double	hRatio, vRatio;
		int	idcX[] = { IDC_BITMAP, IDC_PROGRESS };

		bitmap.DeleteObject();
		bitmap.Attach(m_bitmap);
		bitmap.GetBitmap(&bi);
		bitmap.Detach();

		size.cx = bi.bmWidth;
		size.cy = bi.bmHeight;

		GetWindowRect(&cRc);
		GetClientRect(&wRc);
		gap.cx = cRc.Width() - wRc.Width();
		gap.cy = cRc.Height() - wRc.Height();
		m_static.GetClientRect(&cRc);

		hRatio = vRatio = 1.;
		hRatio = (double)size.cx / (double)cRc.Width();
		vRatio = (double)size.cy / (double)cRc.Height();
		
		wRc.left   = (int)(wRc.left * hRatio);
		wRc.right  = (int)(wRc.right * hRatio);
		wRc.top    = (int)(wRc.top * vRatio);
		wRc.bottom = (int)(wRc.bottom * vRatio);

		wRc.right  += gap.cx;
		wRc.bottom += gap.cy;
		SetWindowPos(NULL, 0, 0, wRc.Width(), wRc.Height(), SWP_NOZORDER|SWP_NOMOVE|SWP_SHOWWINDOW);
		CenterWindow();

		for (int ii = 0; ii < sizeof(idcX) / sizeof(int); ii++)
		{
			item = GetDlgItem(idcX[ii]);
			item->SetFont(&m_font);
			item->GetWindowRect(&wRc);
			this->ScreenToClient(&wRc);

			wRc.left   = (int)(wRc.left   * hRatio);
			wRc.right  = (int)(wRc.right  * hRatio);
			wRc.top    = (int)(wRc.top    * vRatio);
			wRc.bottom = (int)(wRc.bottom * vRatio);
			item->SetWindowPos(this, wRc.left, wRc.top, wRc.Width(), wRc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
		}
		m_static.SetBitmap(m_bitmap);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
