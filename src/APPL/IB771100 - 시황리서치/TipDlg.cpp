// TipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "TipDlg.h"
//#include "TipWnd.h"
#include "MemDC.h"
#include "sharemsg.h"
#define TM_CHECKTIP		101
#define TM_CHECKTIPKEY	102
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
// CTipDlg dialog

static CRect	_mywndRECT;

CTipDlg::CTipDlg(CWnd* pView, CWnd* pWnd, bool bMap, CWnd* pParent /*=NULL*/)
	: CDialog(CTipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pView = pView;
	m_pWnd = pWnd;
	m_font.CreatePointFont(90, "±¼¸²Ã¼");
	m_nHeight = 0;
	m_pChild = NULL;
	m_bMap = bMap;
}


void CTipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTipDlg, CDialog)
	//{{AFX_MSG_MAP(CTipDlg)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTipDlg message handlers

BOOL CTipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_bMap && m_pChild == NULL)
	{
		m_pChild = std::make_unique<CMapWnd>(m_pView);
		m_pChild->Create(NULL, "submap", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 100);
	}	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTipDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	KillTimer(TM_CHECKTIP);	
//	KillTimer(TM_CHECKTIPKEY);
	if (m_pChild)
	{
		m_pChild->DestroyWindow();
		//delete m_pChild;
	}
	m_font.DeleteObject();
	
}

void CTipDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
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
			CRect	rect;
			rect = m_rect;
			rect.InflateRect(0, 1);
			if (!rect.PtInRect(pt))
			{
				HideTips();
			}
		}
	}
	else if( nIDEvent == TM_CHECKTIPKEY)
	{
		KillTimer(TM_CHECKTIPKEY);
		HideTips();
	}
	CDialog::OnTimer(nIDEvent);
}

void CTipDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect	 cRc;
	CMemDC	memDC(&dc);

	GetClientRect(cRc);
	
	memDC.SetBkColor(RGB(255, 255, 225));
	memDC.ExtTextOut(cRc.left, cRc.top, ETO_OPAQUE, cRc, "", 0, NULL);

	CFont*	font = memDC.SelectObject(&m_font);
	
	if (m_tips.Find("\n") < 0)
	{
		CString	str, str2;

		str.Format("%s", m_tips);
		if (str.Find("\t") >= 0)
		{
			str2 = Parser(str, "\t");
			cRc.OffsetRect(0, 1);
			memDC.DrawText(str2, cRc, DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			if (!str.IsEmpty())
				memDC.DrawText(str, cRc, DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		}
		else
			memDC.DrawText(str, cRc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{
		cRc.DeflateRect(5, 5);
		CRect	rcText;
		rcText = cRc;
		rcText.bottom = rcText.top + m_nHeight;
		const int	nMode = memDC.SetBkMode(TRANSPARENT);
		const COLORREF	color = memDC.SetTextColor(RGB(0, 0, 0));
		const int	ncnt = m_arScreen.GetSize();
		CString	str, str2;
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{			
			str = m_arScreen.GetAt(ii);
			if (str.Find("\t") >= 0)
			{
				str2 = Parser(str, "\t");
				memDC.DrawText(str2, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
				if (!str.IsEmpty())
					memDC.DrawText(str, rcText, DT_VCENTER|DT_RIGHT|DT_SINGLELINE);
			}
			else
				memDC.DrawText(str, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
			rcText.OffsetRect(0, m_nHeight);
		}
		memDC.SetTextColor(color);
		memDC.SetBkMode(nMode);
	}
	memDC.SelectObject(font);
	DeleteObject(font);
}

void CTipDlg::ShowTipsInItem(CRect tRc, CRect wRc, CString tips)
{
	CRect	rcTemp = tRc;
	m_pWnd->ClientToScreen(rcTemp);
	if (rcTemp == m_rect && tips == m_tips)
		return;
	
	m_rect.CopyRect(&tRc);
	m_pWnd->ClientToScreen(m_rect);
	
	
	CString	tok = "\n";
	tips.TrimLeft(), tips.TrimRight();	
	m_tips = tips;
	CString	tmpstr;
	tmpstr.Format("%s", tips);	
	const int	ncnt = tmpstr.Replace(tok, tok);
	
	
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
	const int nwidth = dispRc.Width();
	dispRc.left = pt.x + 10;
	dispRc.right = dispRc.left + nwidth;
	MoveWindow(dispRc);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);	
	Invalidate(FALSE);
	SetTimer(TM_CHECKTIPKEY, 800, NULL);
}

void CTipDlg::ShowTips(CRect tRc, CRect wRc, CString tips)
{
	CRect	rcTemp = tRc;
	m_pWnd->ClientToScreen(rcTemp);
	if (rcTemp == m_rect && tips == m_tips)
		return;

	m_rect.CopyRect(&tRc);
	m_pWnd->ClientToScreen(m_rect);


	CString	tok = "\n";
	tips.TrimLeft(), tips.TrimRight();	
	m_tips = tips;
	CString	tmpstr;
	tmpstr.Format("%s", tips);	
	const int	ncnt = tmpstr.Replace(tok, tok);
	

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
	const int	nwidth = dispRc.Width();
	dispRc.left = pt.x + 10;
	dispRc.right = dispRc.left + nwidth;
	MoveWindow(dispRc);
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);	
	Invalidate(FALSE);
	SetTimer(TM_CHECKTIP, TMI_CHECKTIP, NULL);
}

void CTipDlg::HideTips()
{
	ShowWindow(SW_HIDE);

	KillTimer(TM_CHECKTIP);
//	KillTimer(TM_CHECKTIPKEY);
	m_tips = _T("");
	m_rect.SetRectEmpty();
	if (m_bMap)
		m_pWnd->SetFocus();
}

void CTipDlg::ShowMap(CRect tRc, CRect wRc, CString mapname, CString option)
{	
	CRect	rcTmp = tRc;
	
	m_pWnd->ClientToScreen(rcTmp);
	if (m_rect == rcTmp)
		return;

	m_rect.CopyRect(&tRc);
	m_pWnd->ClientToScreen(m_rect);

	const int type = m_pView->SendMessage(WM_USER, MAKEWPARAM(typeDLL, 0), (long)(char *) mapname.operator LPCTSTR());
	const int size = m_pView->SendMessage(WM_USER, MAKEWPARAM(sizeMAP, 0), (long)(char *) mapname.operator LPCTSTR());
	const int cx = LOWORD(size);
	const int cy = HIWORD(size);

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
	
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW|SWP_NOACTIVATE);	
	SetTimer(TM_CHECKTIP, TMI_CHECKTIP, NULL);		
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


CRect CTipDlg::FitSize(CRect rect, CPoint pt)
{
	WINDOWPLACEMENT	pl;
	GetWindowPlacement(&pl);
	const CRect	ret = rect;
	_mywndRECT.SetRectEmpty();
	EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);

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

int CTipDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (m_bMap && m_pChild == NULL)
	{
		m_pChild = std::make_unique<CMapWnd>(m_pView);
		m_pChild->Create(NULL, "submap", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, 100);
	}	

	return 0;
}
