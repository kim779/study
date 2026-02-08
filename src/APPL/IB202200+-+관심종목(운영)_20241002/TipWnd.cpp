// TipWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202200.h"
#include "TipWnd.h"
#include "MemDC.h"
#include "sharemsg.h"
#define TM_CHECKTIP	101
#define TMI_CHECKTIP	100
#include "MapWnd.h"

#define COMPILE_MULTIMON_STUBS
#include "multimon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTipWnd
static CRect	_mywndRECT;
CTipWnd::CTipWnd(CWnd* pView, CWnd* pWnd, bool bMap)
{
	m_pView = pView;
	m_pWnd = pWnd;
	m_font.CreatePointFont(90, "±¼¸²Ã¼");
	m_nHeight = 0;
	m_pChild = nullptr;
	m_bMap = bMap;=
}

CTipWnd::~CTipWnd()
{

}


BEGIN_MESSAGE_MAP(CTipWnd, CWnd)
	//{{AFX_MSG_MAP(CTipWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTipWnd message handlers

void CTipWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect	 cRc;
	CMemDC	memDC(&dc);

	GetClientRect(cRc);
	
	memDC.SetBkColor(RGB(255, 255, 225));
	memDC.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, nullptr);

	CFont*	font = memDC.SelectObject(&m_font);
	
	if (m_tips.Find("\n") < 0)
	{
		cRc.OffsetRect(0, 1);
		memDC.DrawText(m_tips, cRc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	}
	else
	{
		cRc.DeflateRect(5, 5);
		CRect	rcText;
		rcText = cRc;
		rcText.bottom = rcText.top + m_nHeight;
		int	nMode = memDC.SetBkMode(TRANSPARENT);
		COLORREF	color = memDC.SetTextColor(RGB(0, 0, 0));		
		int	ncnt = m_arScreen.GetSize();
		CString	str;
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{			
			str = m_arScreen.GetAt(ii);
			memDC.DrawText(str, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
			rcText.OffsetRect(0, m_nHeight);
		}
		memDC.SetTextColor(color);
		memDC.SetBkMode(nMode);
	}
	memDC.SelectObject(font);	
}

void CTipWnd::ShowTips(CRect tRc, CRect wRc, CString tips)
{
	CRect	rcTemp = tRc;
	m_pWnd->ClientToScreen(rcTemp);
	if (rcTemp == m_rect)
		return;

	m_rect.CopyRect(&tRc);
	m_pWnd->ClientToScreen(m_rect);


	CString	tok = "\n";
	tips.TrimLeft(), tips.TrimRight();	
	m_tips = tips;
	CString	tmpstr;
	tmpstr.Format("%s", tips);	
	int	ncnt = tmpstr.Replace(tok, tok);
	

	CDC*	pDC = GetDC();
	CFont*	font = pDC->SelectObject(&m_font);
	CSize	size;

	m_arScreen.RemoveAll();
	if (ncnt)
	{
		int	pos = 0;
		int	nMax = 0;
		CString	sztmp = _T("");

		for ( int ii = 0 ; ii < ncnt + 1; ii++ )
		{
			pos = tmpstr.Find(tok);
			if (pos < 0)
			{
				tmpstr.TrimLeft(), tmpstr.TrimRight();
				size = pDC->GetTextExtent(tmpstr + " ");
				
				m_arScreen.Add(tmpstr);
			}
			else
			{
				sztmp = tmpstr.Left(pos);
				sztmp.TrimLeft(), sztmp.TrimRight();
				size = pDC->GetTextExtent(sztmp + " ");		
				
				m_arScreen.Add(sztmp);
				
				tmpstr = tmpstr.Mid(pos + 1);
			}

			if (size.cx > nMax)
				nMax = size.cx;			
		}
		size.cx = nMax;		
	}
	else
	{
		size = pDC->GetTextExtent(tips + "  ");	
	}

	pDC->SelectObject(font);	
	ReleaseDC(pDC);

	CRect	dispRc;
	tRc.OffsetRect(0, tRc.Height() + 10);
	m_nHeight = size.cy * 4/3;
	size.cy = m_nHeight * (ncnt + 1);
	dispRc.SetRect(tRc.left, tRc.top, tRc.left + size.cx, tRc.top + size.cy);
	
	if (ncnt)
		dispRc.InflateRect(5, 5);
	
	if (dispRc.right > (wRc.right - 10))
	{
		dispRc.right = wRc.right - 10;
		dispRc.left = dispRc.right - size.cx;
	}

	CPoint	pt;
	GetCursorPos(&pt);
	m_pWnd->ClientToScreen(dispRc);
	ScreenToClient(dispRc);
	ClientToScreen(dispRc);
	int	nwidth = dispRc.Width();
	dispRc.left = pt.x + 10;
	dispRc.right = dispRc.left + nwidth;
	MoveWindow(dispRc);
	SetWindowPos(nullptr, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);	
	Invalidate(FALSE);
	SetTimer(TM_CHECKTIP, TMI_CHECKTIP, nullptr);
}

void CTipWnd::HideTips()
{
	ShowWindow(SW_HIDE);

	KillTimer(TM_CHECKTIP);
	m_tips = _T("");
	m_rect.SetRectEmpty();	
}

void CTipWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_CHECKTIP)
	{
		CPoint	pt;
		GetCursorPos(&pt);

		if (m_bMap)
		{
			if (!m_rect.PtInRect(pt) && !m_rect2.PtInRect(pt))
			{
				HideTips();
			}
		}
		else
		{
			if (!m_rect.PtInRect(pt))
			{
				HideTips();
			}
		}

		
	}
	CWnd::OnTimer(nIDEvent);
}

void CTipWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	if (m_pChild)
		m_pChild->DestroyWindow();
	KillTimer(TM_CHECKTIP);	


	m_font.DeleteObject();
}




int CTipWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_bMap && m_pChild == nullptr)
	{
		m_pChild = std::make_unique<CMapWnd>(m_pView);
		m_pChild->Create(nullptr, "submap", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 100);
	}
	return 0;
}

void CTipWnd::ShowMap(CRect tRc, CRect wRc, CString mapname, CString option)
{	
	CRect	rcTmp = tRc;
	
	m_pWnd->ClientToScreen(rcTmp);
	if (m_rect == rcTmp)
		return;

	m_rect.CopyRect(&tRc);
	m_pWnd->ClientToScreen(m_rect);

	int type = m_pView->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), (long)(char *) mapname.operator LPCTSTR());
	int size = m_pView->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0), (long)(char *) mapname.operator LPCTSTR());
	int cx = LOWORD(size);	int cy = HIWORD(size);		
	CRect	dispRc;
	
	dispRc.SetRect(tRc.right - 1, tRc.top, tRc.left + cx + 20, tRc.top + cy + 2);
	CPoint	pt;
	GetCursorPos(&pt);
	m_pWnd->ClientToScreen(dispRc);
	ScreenToClient(dispRc);
	ClientToScreen(dispRc);
	dispRc = FitSize(dispRc, pt);		
	MoveWindow(dispRc);
	m_rect2.CopyRect(dispRc);
	GetClientRect(dispRc);
	dispRc.DeflateRect(0, 0, 0, 0);
	m_pChild->MoveWindow(dispRc);
	m_pChild->ChangeMap(mapname, option);
	m_pChild->ShowWindow(SW_SHOW);
	m_pChild->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);	
	SetWindowPos(&wndBottom, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);	
	SetTimer(TM_CHECKTIP, TMI_CHECKTIP, nullptr);		
}

BOOL CALLBACK MyInfoEnumProc(HMONITOR hMonitor,  HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFO lpmi ;
	memset(&lpmi,0,sizeof(MONITORINFO));
	lpmi.cbSize = sizeof(MONITORINFO);
	if(!GetMonitorInfo(hMonitor,  &lpmi ))
		return FALSE;

	if (_mywndRECT.left  > lpmi.rcWork.left)	_mywndRECT.left   = lpmi.rcWork.left;
	if (_mywndRECT.top   > lpmi.rcWork.top)		_mywndRECT.top    = lpmi.rcWork.top;
	if (_mywndRECT.right < lpmi.rcWork.right)	_mywndRECT.right  = lpmi.rcWork.right;
	if (_mywndRECT.bottom< lpmi.rcWork.bottom)	_mywndRECT.bottom = lpmi.rcWork.bottom;
	return TRUE;
}


CRect CTipWnd::FitSize(CRect rect, CPoint pt)
{
	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);
	CRect	ret = rect;
	_mywndRECT.SetRectEmpty();
	EnumDisplayMonitors(nullptr, nullptr, MyInfoEnumProc, 0);

	if (rect.left < _mywndRECT.left)
		rect.OffsetRect(_mywndRECT.left - rect.left, 0);

	if (rect.right > _mywndRECT.right)
		rect.OffsetRect(_mywndRECT.right- rect.right, 0);		

	if (rect.top < _mywndRECT.top)
		rect.OffsetRect(0, _mywndRECT.top - rect.top);		
		
	if (rect.bottom > _mywndRECT.bottom)
		rect.OffsetRect(0, _mywndRECT.bottom - rect.bottom);
		
	return rect;
}