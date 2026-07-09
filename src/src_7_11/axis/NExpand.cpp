// NExpand.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NExpand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNExpand

CNExpand::CNExpand(CWnd* parent, CString home)
{
	m_parent	= parent;
	m_home		= home;
	m_status[0]	= STAT_DISABLE;
	m_status[1]	= STAT_DISABLE;
	m_selectFlag	= DIR_NONE;
}

CNExpand::~CNExpand()
{
}


BEGIN_MESSAGE_MAP(CNExpand, CWnd)
	//{{AFX_MSG_MAP(CNExpand)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNExpand message handlers

int CNExpand::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HBITMAP	hBitmap;
	CString	filename;

	filename.Format("%s\\image\\%s.bmp", m_home, "exarr");
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), filename,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		m_img.Attach(hBitmap);
		BITMAP	bm;
		m_img.GetBitmap(&bm);
		SetWindowPos(NULL, 0, 0, bm.bmWidth/3, bm.bmHeight, SWP_HIDEWINDOW);
	}
	return 0;
}

void CNExpand::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	drawImg(DIR_LEFT, false, &dc);
	drawImg(DIR_RIGHT, false, &dc);
}

void CNExpand::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	m_selectFlag = SelectButton(point);
	drawImg(m_selectFlag, true);
	CWnd::OnLButtonDown(nFlags, point);
}

void CNExpand::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	drawImg(m_selectFlag);
	if (m_selectFlag == SelectButton(point))
	{
		m_parent->PostMessage(WM_EXPANDEVENT, m_selectFlag, 0);
	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CNExpand::SetStatus(int lStatus, int rStatus)
{
	m_status[0] = lStatus;
	m_status[1] = rStatus;
	Invalidate(FALSE);
}

UINT CNExpand::SelectButton(CPoint pt)
{
	CRect	iRc;
	BITMAP	bm;
	m_img.GetBitmap(&bm);
	int imgW = bm.bmWidth/6;

	iRc.SetRect(0, 0, imgW, bm.bmHeight);
	if (iRc.PtInRect(pt) && m_status[0] != STAT_DISABLE)
		return DIR_LEFT;
	
	iRc.OffsetRect(imgW, 0);
	if (iRc.PtInRect(pt) && m_status[1] != STAT_DISABLE)
		return DIR_RIGHT;
	return DIR_NONE;
}

void CNExpand::drawImg(int dir, bool down, CDC* pDC)
{
	CClientDC	dc(this);
	CDC		memDC;
	if (!pDC)	pDC = &dc;
	BITMAP		bm;
	
	m_img.GetBitmap(&bm);
	int imgW = bm.bmWidth/6;
	if (memDC.CreateCompatibleDC(pDC))
	{
		CBitmap	*pbitmap;
		pbitmap = memDC.SelectObject(&m_img);

		if (dir == DIR_LEFT)
		{
			switch (m_status[0])
			{
			case STAT_DISABLE:
				pDC->BitBlt(0, 0, imgW, bm.bmHeight, &memDC, STAT_DISABLE*imgW, 0, SRCCOPY);
				break;
			default:
				if (down)
					pDC->BitBlt(0, 0, imgW, bm.bmHeight, &memDC, STAT_DOWN*imgW, 0, SRCCOPY);
				else	pDC->BitBlt(0, 0, imgW, bm.bmHeight, &memDC, STAT_ENABLE*imgW, 0, SRCCOPY);
				break;
			}
		}
		
		if (dir == DIR_RIGHT)
		{
			switch (m_status[1])
			{
			case STAT_DISABLE:
				pDC->BitBlt(imgW, 0, imgW, bm.bmHeight, &memDC, (STAT_CNT + STAT_DISABLE)*imgW, 0, SRCCOPY);
				break;
			default:
				if (down)
					pDC->BitBlt(imgW, 0, imgW, bm.bmHeight, &memDC, (STAT_CNT + STAT_DOWN)*imgW, 0, SRCCOPY);
				else	pDC->BitBlt(imgW, 0, imgW, bm.bmHeight, &memDC, (STAT_CNT + STAT_ENABLE)*imgW, 0, SRCCOPY);
				break;
			}
		}
		
		memDC.SelectObject(pbitmap);
		memDC.DeleteDC();
	}
}