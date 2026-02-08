// ImageNotice.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "ImageNotice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageNotice dialog


CImageNotice::CImageNotice(CWnd* pParent, LPCSTR szImagePath)
	: CDialog(CImageNotice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImageNotice)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_path = szImagePath;
}


void CImageNotice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImageNotice)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImageNotice, CDialog)
	//{{AFX_MSG_MAP(CImageNotice)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageNotice message handlers

BOOL CImageNotice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HBITMAP hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(),	(LPCTSTR)m_path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBmp)
	{
		BITMAP bmp;

		m_image.Attach(hBmp);
		m_image.GetObject(sizeof(bmp), &bmp);
		SetWindowPos(&wndTopMost, 0, 0, bmp.bmWidth+4, bmp.bmHeight+20, SWP_NOMOVE);
	}
	
	return TRUE;
}

void CImageNotice::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(rc);

	if (m_image.GetSafeHandle())
	{
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		CBitmap *pBmp = (CBitmap*)memDC.SelectObject(m_image.GetSafeHandle());
		dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pBmp);
	}
}
