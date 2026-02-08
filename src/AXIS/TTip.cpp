// TTip.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "TTip.h"
#include "../dll/ctrl/MemDC.h"

#include "MainFrm.h"

#define TM_CHECKTIP	101
#define TMI_CHECKTIP	100

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTip

CTTip::CTTip(CWnd* mainframe)
{
	m_mainframe = mainframe;
	m_font.CreatePointFont(90, "±¼¸²Ã¼");
	m_nHeight = 0;
}

CTTip::~CTTip()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CTTip, CWnd)
	//{{AFX_MSG_MAP(CTTip)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTTip message handlers

void CTTip::OnPaint() 
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
		cRc.OffsetRect(0, 1);
		memDC.DrawText(m_tips, cRc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	}
	else
	{
		cRc.DeflateRect(5, 5);
		CRect	rcText;
		rcText = cRc;
		rcText.bottom = rcText.top + m_nHeight;

		const int	nMode = memDC.SetBkMode(TRANSPARENT);
		const COLORREF	color = memDC.SetTextColor(RGB(255, 255, 255));
		memDC.FillSolidRect(rcText, RGB(125, 125, 255));
		memDC.DrawText(m_title, rcText, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
		memDC.SetTextColor(color);

		rcText.OffsetRect(0, 5);
#ifdef _DUP_SCREEN
		const int	ncnt = m_arScreen.GetSize();
		CString	str;
		for ( int ii = 0 ; ii < ncnt ; ii++ )
		{
			rcText.OffsetRect(0, m_nHeight);
			str = m_arScreen.GetAt(ii);
			memDC.DrawText(str, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		}
#else
		POSITION	pos = m_mapScreen.GetStartPosition();
		CString	key, value;
		
		while(pos)
		{
			rcText.OffsetRect(0, m_nHeight);
			m_mapScreen.GetNextAssoc(pos, key, value);
			memDC.DrawText(value, rcText, DT_VCENTER|DT_LEFT|DT_SINGLELINE);
		}
#endif
		memDC.SetBkMode(nMode);
	}
	memDC.SelectObject(font);
}

void CTTip::ShowTips(CRect tRc, CRect wRc, CString tips)
{
	CString	tok = "\n";

	if(tips.IsEmpty())
	{
		return;
	}

	tips.TrimLeft();
	tips.TrimRight();

	if(tips == "")
	{
		return;
	}

	TRY 
	{
		m_tips = tips;
		CString	tmpstr;
		tmpstr.Format("%s", tips);	
		const int	ncnt = tmpstr.Replace(tok, tok);
		m_rect.CopyRect(&tRc);
		
		CDC*	pDC = GetDC();
		CFont*	font = pDC->SelectObject(&m_font);
		CSize	size;
		
		m_title = _T("");
#ifdef _DUP_SCREEN
		m_arScreen.RemoveAll();
#else
		m_mapScreen.RemoveAll();
#endif	
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
					size = pDC->GetTextExtent(tmpstr + "  ");
					
					if (ii == 0)
						m_title = tmpstr;
					else
#ifdef _DUP_SCREEN
						m_arScreen.Add(tmpstr);
#else
					m_mapScreen.SetAt(tmpstr, tmpstr);
#endif
				}
				else
				{
					sztmp = tmpstr.Left(pos);
					sztmp.TrimLeft(), sztmp.TrimRight();
					size = pDC->GetTextExtent(sztmp + "  ");	
					if (ii == 0)
						m_title = sztmp;
					else
					{
#ifdef _DUP_SCREEN
						m_arScreen.Add(sztmp);
#else
						m_mapScreen.SetAt(sztmp, sztmp);
#endif
					}
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
		tRc.OffsetRect(0, tRc.Height()*2);
		m_nHeight = size.cy * 4/3;
#ifndef _DUP_SCREEN
		ncnt = m_mapScreen.GetCount();
#endif
		size.cy = m_nHeight * (ncnt + 1) + 10;
		dispRc.SetRect(tRc.left, tRc.top, tRc.left + size.cx, tRc.top + size.cy);
		
		if (ncnt)
			dispRc.InflateRect(5, 5);
		
		if (dispRc.right > (wRc.right - 10))
		{
			dispRc.right = wRc.right - 10;
			dispRc.left = dispRc.right - size.cx;
		}
		
		MoveWindow(dispRc);
		
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);	
		Invalidate(FALSE);
		SetTimer(TM_CHECKTIP, TMI_CHECKTIP, NULL);
	}
	CATCH (CException, e)
	{
		
	}
	END_CATCH
}

void CTTip::HideTips()
{
	ShowWindow(SW_HIDE);

	KillTimer(TM_CHECKTIP);
	m_tips = _T("");
	m_rect.SetRectEmpty();

}

void CTTip::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TM_CHECKTIP)
	{
		CPoint pt;
		GetCursorPos(&pt);
		if (!m_rect.PtInRect(pt))
			HideTips();
	}
	CWnd::OnTimer(nIDEvent);
}
