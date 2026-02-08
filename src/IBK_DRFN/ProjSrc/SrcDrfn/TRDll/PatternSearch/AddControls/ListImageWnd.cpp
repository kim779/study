// ListImageWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ListImageWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULT_LEFT_MARGIN 5
#define DEFAULT_TOP_MARGIN	5
#define DEFAULT_IMAGE_COUNT 3
#define LINESIZE			32

/////////////////////////////////////////////////////////////////////////////
// CListImageWnd

CListImageWnd::CListImageWnd()
{
    RegisterWindowClass();

	m_nLeftMargin = DEFAULT_LEFT_MARGIN;
	m_nTopMargin = DEFAULT_TOP_MARGIN;

	m_nImageCountPerRow = DEFAULT_IMAGE_COUNT;
	m_nImageWidth = 0;
	m_nImageHeight = 0;
	m_nXGap = 10;
	m_nYGap = 10;

	m_nViewWidth = 0;
	m_nViewHeight = 0;

	m_pParent = NULL;
	m_fontDefault = NULL;

	m_bAutoHorzAlign = TRUE;
	m_bVertScroll = FALSE;
	m_bUseDeleteMenu = FALSE;
	m_bShowToolTip = FALSE;
	m_nCurImgIndex = -1;
}

CListImageWnd::~CListImageWnd()
{
}


BEGIN_MESSAGE_MAP(CListImageWnd, CWnd)
	//{{AFX_MSG_MAP(CListImageWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
#if (_MFC_VER >= 0x0421)
    ON_WM_MOUSEWHEEL()
#endif
END_MESSAGE_MAP()

BOOL CListImageWnd::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();

    if (!(::GetClassInfo(hInst, IMAGELISTCONTROL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = IMAGELISTCONTROL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CListImageWnd message handlers
BOOL CListImageWnd::Create(CRect rcWindow, CWnd* pParent, DWORD dwStyle, int nID)
{
	m_pParent = pParent;

	dwStyle |= WS_HSCROLL;
	dwStyle |= WS_VSCROLL;

    if (!CWnd::Create(IMAGELISTCONTROL_CLASSNAME, "", dwStyle, rcWindow, pParent, nID))
	{
		return FALSE;
	}

	return TRUE;
}

int CListImageWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

void CListImageWnd::CreateTooltip() 
{
	m_tipInfo.Create(this, "BASE", 1);
	if(m_fontDefault) m_tipInfo.SetFont(m_fontDefault);
	m_tipInfo.AddTool(this, (LPCTSTR)" ");

	m_bShowToolTip = TRUE;
}

void CListImageWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if(m_bShowToolTip){
	m_tipInfo.RemoveTool(this);
		m_tipInfo.DestroyWindow();
	}
	m_ImageList.DeleteImageList();
}

BOOL CListImageWnd::OnEraseBkgnd(CDC* pDC) 
{
    return TRUE;    // Don't erase the background.
}

void CListImageWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// Do not call CWnd::OnPaint() for painting messages
	
 	CRect rcClient;
	GetClientRect(rcClient);

	CBitmap bitmap, *pOldMemDC;
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	pOldMemDC = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rcClient, RGB(255,255,255));

	int nImageCnt = m_ImageList.GetImageCount();
	BOOL bDraw = TRUE;

	if(nImageCnt == 0 || m_nImageWidth == 0 || m_nImageHeight == 0) bDraw = FALSE;
	if(m_nViewWidth == 0 || m_nViewHeight == 0) bDraw = FALSE;

	if(bDraw && nImageCnt > 0)
	{
		// change the window origin of the device for paint
		memDC.SetWindowOrg(m_nHScrollPos, m_nVScrollPos);

		CPoint pt;
		for(int i = 0; i < nImageCnt; i++)
		{
			pt = GetImagePos(i);
	// 		if(m_nImageSelected != -1 && m_nImageSelected == i)
	// 			m_ImageList.Draw(&memDC, i, pt, ILD_BLEND50); 
	// 		else
				m_ImageList.Draw(&memDC, i, pt, ILD_NORMAL); 
		}
		// restore the window orign
		memDC.SetWindowOrg(0, 0);
		
	}
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);	

	memDC.SelectObject(pOldMemDC);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}

CPoint CListImageWnd::GetImagePos(int nIndex)
{
	int nX = nIndex%m_nImageCountPerRow;
	int nY = nIndex / m_nImageCountPerRow;

	CPoint pt(m_nLeftMargin + nX * m_nImageWidth + m_nXGap*nX, 
				m_nTopMargin + nY* m_nImageHeight + m_nYGap*nY);

	// 스크롤바가 없을 때 이미지 가운데 정렬 적용
// 	SCROLLINFO scInfo;
// 	scInfo.cbSize = sizeof(scInfo);
//     scInfo.fMask  = SIF_ALL;
//	BOOL bRet = GetScrollInfo(SB_VERT, &scInfo);
	if(m_bAutoHorzAlign && !m_bVertScroll)
	{
		int nTotalHorz = m_nLeftMargin*2 + m_nImageWidth*m_nImageCountPerRow + m_nXGap*(m_nImageCountPerRow-1);

		int nDiff = m_nScreenWidth - nTotalHorz;
		int nRest = nDiff/(m_nImageCountPerRow + 1);
		pt.x = pt.x + (nX+1)*nRest;
	}


	return pt;
}

void CListImageWnd::SetImageSpace(int nX, int nY)
{
	m_nXGap = nX;
	m_nYGap = nY;
}

void CListImageWnd::SetMargin(int nX, int nY)
{
	m_nLeftMargin = nX;
	m_nTopMargin = nY;
}

void CListImageWnd::SetUseDeleteMenu(BOOL bUse)
{
	m_bUseDeleteMenu = bUse;
}

void CListImageWnd::SetImageData(CImageList& imgList, int nImageCntPerRow, int nImageWidth, int nImageHeight)
{
	m_ImageList.Create(&imgList);

	m_nImageCountPerRow = nImageCntPerRow;
	m_nImageWidth = nImageWidth;
	m_nImageHeight = nImageHeight;

	Refresh();
}

void CListImageWnd::AddImage(CBitmap bitmap)
{
	m_ImageList.Add(&bitmap, RGB(222,0,255));
}

void CListImageWnd::Refresh()
{
	int nImageCnt = m_ImageList.GetImageCount();
	int nX = nImageCnt%m_nImageCountPerRow;
	int nY = nImageCnt / m_nImageCountPerRow;

	if(nX != 0) nY++;
	if(nY > 0) nX = m_nImageCountPerRow;

	m_nViewWidth = m_nLeftMargin + nX*m_nImageWidth + m_nXGap*(nX-1);
	m_nViewHeight = m_nTopMargin + nY*m_nImageHeight + m_nYGap*(nY-1);

	ResetScrollInfo();
	Invalidate();
}

void CListImageWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	m_nScreenWidth = cx;
	m_nScreenHeight = cy;

	ResetScrollInfo();
}

void CListImageWnd::ResetScrollInfo()
{
	int nHScrollMax = 0;
	m_nHScrollPos = m_nHPageSize = 0;
	
	if(m_nScreenWidth < m_nViewWidth)
	{
		nHScrollMax = m_nViewWidth - 1;
		m_nHPageSize = m_nScreenWidth;
		m_nHScrollPos = min(m_nHScrollPos, m_nViewWidth - m_nHPageSize - 1);
	}

	SCROLLINFO si;
	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nHScrollMax;
	si.nPos = m_nHScrollPos;
	si.nPage = m_nHPageSize;
	SetScrollInfo(SB_HORZ, &si, TRUE);

	int nVScrollMax = 0;
	m_nVScrollPos = m_nVPageSize = 0;

	m_bVertScroll = FALSE;
	if(m_nScreenHeight < m_nViewHeight)
	{
		nVScrollMax = m_nViewHeight -1;
		m_nVPageSize = m_nScreenHeight;
		m_nVScrollPos = min(m_nVScrollPos, m_nViewHeight - m_nVPageSize - 1);
		m_bVertScroll = TRUE;
	}

	si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
	si.nMin = 0;
	si.nMax = nVScrollMax;
	si.nPos = m_nVScrollPos;
	si.nPage = m_nVPageSize;
	SetScrollInfo(SB_VERT, &si, TRUE);
}

BOOL CListImageWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_bVertScroll) return FALSE;

	UINT nWheelScrollLines;
	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &nWheelScrollLines, 0);
	
	BOOL bUp = TRUE;
	int nDelta = zDelta;
	if(zDelta < 0)
	{
		bUp = FALSE;
		nDelta = -nDelta;
	}

    if (nWheelScrollLines == WHEEL_PAGESCROLL)
    {
		PostMessage(WM_VSCROLL, MAKEWPARAM(bUp? SB_PAGEUP:SB_PAGEDOWN, 0), 0);
    }
    else
    {
        int nRowsScrolled = nWheelScrollLines * nDelta / WHEEL_DELTA;

        for (int i = 0; i < nRowsScrolled; i++)
		{
			PostMessage(WM_VSCROLL, MAKEWPARAM(bUp? SB_LINEUP:SB_LINEDOWN, 0), 0);
		}
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CListImageWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nDelta;

	switch(nSBCode)
	{
	case SB_LINELEFT:
		nDelta = -LINESIZE;
		break;
	case SB_LINERIGHT:
		nDelta = LINESIZE;
		break;
	case SB_PAGELEFT:
		nDelta = -m_nHPageSize;
		break;
	case SB_PAGERIGHT:
		nDelta = m_nHPageSize;
		break;
	case SB_THUMBTRACK:
		nDelta = (int)nPos - m_nHScrollPos;
		break;
	default:
		return;
	}
	
	int nScrollPos = m_nHScrollPos + nDelta;
	int nMaxPos = m_nViewWidth - m_nHPageSize;
	if(nScrollPos < 0)
		nDelta = -m_nHScrollPos;
	else if(nScrollPos > nMaxPos)
		nDelta = nMaxPos - m_nHScrollPos;

	if(nDelta != 0)
	{
		m_nHScrollPos += nDelta;
		SetScrollPos(SB_HORZ, m_nHScrollPos, TRUE);
		ScrollWindow(-nDelta, 0);
	}

//	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CListImageWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int nDelta;

	switch(nSBCode)
	{
	case SB_LINEUP:
		nDelta = -LINESIZE;
		break;
	case SB_LINERIGHT:
		nDelta = LINESIZE;
		break;
	case SB_PAGEUP:
		nDelta = -m_nVPageSize;
		break;
	case SB_PAGEDOWN:
		nDelta = m_nVPageSize;
		break;
	case SB_THUMBTRACK:
		nDelta = (int)nPos - m_nVScrollPos;
		break;
	default:
		return;
	}
	
	int nScrollPos = m_nVScrollPos + nDelta;
	int nMaxPos = m_nViewHeight - m_nVPageSize;
	if(nScrollPos < 0)
		nDelta = -m_nVScrollPos;
	else if(nScrollPos > nMaxPos)
		nDelta = nMaxPos - m_nVScrollPos;

	if(nDelta != 0)
	{
		m_nVScrollPos += nDelta;
		SetScrollPos(SB_VERT, m_nVScrollPos, TRUE);
		ScrollWindow(0, -nDelta);
	}
	
//	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CListImageWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bUseDeleteMenu)
	{
		CPoint ptClick(point.x + m_nHScrollPos, point.y + m_nVScrollPos);
		CRect rcInvalidate;
		int nImageCount = m_ImageList.GetImageCount();
		for(int i = 0; i < nImageCount; i++)
		{
			CPoint pt = GetImagePos(i);
			CRect rcImage(pt.x, pt.y, pt.x + m_nImageWidth, pt.y + m_nImageHeight);
	
			if(rcImage.PtInRect(ptClick))
			{
				CMenu	menu;
				menu.CreatePopupMenu();
				menu.AppendMenu(MF_STRING, MENU_RESULTCTRL_INDEX[0], "패턴 삭제"); 
	
				UINT nFlagsForMenu = TPM_LEFTALIGN | TPM_RIGHTBUTTON | TPM_NONOTIFY | TPM_RETURNCMD;
				CPoint ptMenu = point;
				ClientToScreen(&ptMenu);
				int nRetValue = menu.TrackPopupMenu(nFlagsForMenu, ptMenu.x, ptMenu.y, this);
				menu.DestroyMenu();
				
				if( nRetValue>0 )
				{
					m_pParent->PostMessage(RMSG_DELETE_ITEM, i, 0);
				}
				break;
			}
		}
	}

	CWnd::OnRButtonDown(nFlags, point);
}

void CListImageWnd::OnMouseMove(UINT nFlags, CPoint point)
{
// 	if(m_bShowToolTip)
// 	{
// 		CPoint pt(point.x + m_nHScrollPos, point.y + m_nVScrollPos);
// 
// 		int nImageCount = m_ImageList.GetImageCount();
// 		int nHitIndex = -1;
// 
// 		for(int i = 0; i < nImageCount; i++)
// 		{
// 			CPoint ptImg = GetImagePos(i);
// 			CRect rcImage(ptImg.x, ptImg.y, ptImg.x + m_nImageWidth, ptImg.y + m_nImageHeight);
// 
// 			if(rcImage.PtInRect(pt))
// 			{
// 				nHitIndex = i;
// 				
// 				break;
// 			}
// 		}
// 
// 		if(nHitIndex >= 0 && m_nCurImgIndex != nHitIndex)
// 		{
// 			CString strTip;
// 			char szTipText[256];
// 			int nOption = m_pParent->SendMessage(RMSG_GET_TIPTEXT, MAKEWPARAM(GetDlgCtrlID(), nHitIndex), (LPARAM)szTipText);
// 
// 			strTip = szTipText;
// 			if(nOption == 1 || (nOption == 0 && strTip.GetLength() > 10)) 
// 			{
// 				m_tipInfo.UpdateToolTip(this, strTip, 1);
// 				m_tipInfo.Show(strTip);
// 			}
// 		}
// 		else if(nHitIndex == -1)
// 		{
// 			m_tipInfo.Hide();
// 			m_nCurImgIndex = -1;
// 		}
// 	}

	CWnd::OnMouseMove(nFlags, point);
}

void CListImageWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint ptClick(point.x + m_nHScrollPos, point.y + m_nVScrollPos);
	CRect rcInvalidate;
	int nImageCount = m_ImageList.GetImageCount();
	for(int i = 0; i < nImageCount; i++)
	{
		CPoint pt = GetImagePos(i);
		CRect rcImage(pt.x, pt.y, pt.x + m_nImageWidth, pt.y + m_nImageHeight);

		if(rcImage.PtInRect(ptClick)/* && m_nImageSelected != i*/)
		{
// 			pt = GetImagePos(m_nImageSelected);
// 			CRect rcOld(pt.x, pt.y, pt.x + m_nImageWidth, pt.y + m_nImageHeight);
// 
// 			rcOld.OffsetRect(-m_nHScrollPos, -m_nVScrollPos);
//			InvalidateRect(rcOld);

			//m_nImageSelected = i;
			rcImage.OffsetRect(-m_nHScrollPos, -m_nVScrollPos);
			InvalidateRect(rcImage);

			// Send Message to parent
			NMITEMACTIVATE NMHDR;
			
			NMHDR.hdr.hwndFrom = m_hWnd;
			NMHDR.hdr.idFrom = GetDlgCtrlID();
			NMHDR.hdr.code = NM_CLICK;

			NMHDR.iItem = i;
			NMHDR.iSubItem = 0; // not use
			NMHDR.uNewState = 0; // not use
			NMHDR.uOldState = 0; // not use
			NMHDR.uChanged = 0; // not use
			NMHDR.ptAction = point; // not use
			NMHDR.lParam = 0; // not use
			NMHDR.uKeyFlags = 0; // not use

			m_pParent->SendMessage(WM_NOTIFY, NMHDR.hdr.idFrom, (LPARAM)&NMHDR);

			break;
		}
	}

	SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}

