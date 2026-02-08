// ImageViewer.cpp : implementation file
//

#include "stdafx.h"
#include "wdrchartdgndlg.h"
#include "ImageViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageViewer

CImageViewer::CImageViewer()
{
	m_strFile = "";
}

CImageViewer::~CImageViewer()
{
}


BEGIN_MESSAGE_MAP(CImageViewer, CStatic)
	//{{AFX_MSG_MAP(CImageViewer)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageViewer message handlers

void CImageViewer::OnPaint() 
{
	CPaintDC dc(this);

	CRect rcBounds;
	GetClientRect(&rcBounds);
	_DoDraw(&dc, rcBounds);
}

void CImageViewer::OnDestroy() 
{
	CStatic::OnDestroy();
}


void CImageViewer::LoadImage(LPCSTR szFile)
{
	m_strFile = szFile;
}

void CImageViewer::_DoDraw(CDC* pdc, const CRect& rcBounds)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	if( !m_strFile.IsEmpty() )
	{
		HANDLE bmp;
		bmp = ::LoadImage( NULL, m_strFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE );
		CBitmap bitmap, *pOldBitmap;
		BITMAP  bmpInfo;
		bitmap.Attach( bmp );
		bitmap.GetBitmap( &bmpInfo );

		CDC MemDC;
		MemDC.CreateCompatibleDC(pdc);
		pOldBitmap = (CBitmap*)MemDC.SelectObject( bitmap );
		pdc->StretchBlt(0, 0, rcBounds.Width(), rcBounds.Height(), &MemDC, 0, 0, bmpInfo.bmWidth,bmpInfo.bmHeight, SRCCOPY);
		MemDC.SelectObject( pOldBitmap );
		bitmap.DeleteObject();
		MemDC.DeleteDC();
	}
}

