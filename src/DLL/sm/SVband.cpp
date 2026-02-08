// SVband.cpp : implementation file
// 상단툴바 광고판

#include "stdafx.h"
#include "SVband.h"
#include "skindlg.h"
#include "NButton.h"
#include "PalButton.h"
#include "NPalette.h"
#include "../../h/axisvar.h"
#include "../../axis/axMsg.hxx"

#include "resource.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	GAP		5

#define DEFAULT_W	20
#define IMG_GAP		3
#define BGAP		2


#define	SELECT_COLOR	RGB(255, 255, 255)
#define	NONE_COLOR	RGB(208, 233, 237)

#define IDX_SETUP	2
#define IDX_DEAL	3
#define IDX_MAX		4

#define	MODE_COOL	0
#define	MODE_OVER	1
#define	MODE_DOWN	2

#define CTRL_CHANGE	0x58

/////////////////////////////////////////////////////////////////////////////
// CSVband
CSVband::CSVband(bool (*callback)(int, WPARAM, LPARAM), bool bSDI)
{
	m_axiscall = callback;
	m_bSDI = bSDI;

	m_pushN = -1;
	m_bk = RGB(242, 242, 242);
	m_vsN = V_SCREEN1;
	m_bshape = false;

	loadPalette();

	m_pChange = NULL;
	m_mode = 0;

	m_type = TTYPE1;

	m_bmpLeft = Axis::GetSkinBitmap("TOOL_LEFTBACK");
	m_bmpMiddle = Axis::GetSkinBitmap("TOOL_MIDDLEBACK");
	m_bmpRight = Axis::GetSkinBitmap("TOOL_RIGHTBACK");
	if (m_bmpLeft)
	{
		BITMAP bm;
		m_bmpLeft->GetBitmap(&bm);

		m_bmLWidth = bm.bmWidth;
		m_bmHeight = bm.bmHeight;
	}
	else
	{
		m_bmLWidth = m_bmHeight = 0;
	}
	if (m_bmpMiddle)
	{
		BITMAP bm;
		m_bmpMiddle->GetBitmap(&bm);

		m_bmMWidth = bm.bmWidth;
	}
	else
	{
		m_bmMWidth = m_bmHeight = 0;
	}
	if (m_bmpRight)
	{
		BITMAP bm;
		m_bmpRight->GetBitmap(&bm);

		m_bmRWidth = bm.bmWidth;
	}
	else
	{
		m_bmRWidth = m_bmHeight = 0;
	}

	m_bmpBand = Axis::GetSkinBitmap("band");
	if (m_bmpBand)
	{
		BITMAP bm;
		m_bmpBand->GetBitmap(&bm);
		m_bmBandWidth = bm.bmWidth;
		m_bmBandHeight = bm.bmHeight;
	}
	else
	{
		m_bmBandWidth = m_bmBandHeight = 0;
	}
}

CSVband::~CSVband()
{
#ifndef DF_USESTL_SVBAND
	if (m_pChange)	delete m_pChange;
#endif
}


BEGIN_MESSAGE_MAP(CSVband, CWnd)
	//{{AFX_MSG_MAP(CSVband)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSVband message handlers

void CSVband::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	if (m_bmpBand && m_bmpBand->m_hObject)
	{
		CDC mdc, ldc, rdc, cdc;
		mdc.CreateCompatibleDC(&dc);

		CBitmap* oldBmp = mdc.SelectObject(m_bmpBand);
		dc.StretchBlt(0, 0, rc.Width(), m_bmBandHeight, &mdc, 0, 0, m_bmBandWidth, m_bmBandHeight, SRCCOPY);
		
		/*
		if (m_bmpLeft && m_bmpLeft->m_hObject)
		{
			mdc.SelectObject(m_bmpLeft);
			dc.BitBlt(0,0, m_bmLWidth, m_bmHeight, &mdc, 0, 0, SRCCOPY);
		}
		*/
		if (rc.Width() >30)
		{
			mdc.SelectObject(oldBmp);

			rdc.CreateCompatibleDC(&dc);
			if (m_bmpRight && m_bmpRight->m_hObject)
			{
				rdc.SelectObject(m_bmpRight);
				dc.BitBlt(rc.right-m_bmRWidth, 3, m_bmRWidth, m_bmHeight+3, &rdc, 0, 0, SRCCOPY);
			}
			ldc.CreateCompatibleDC(&dc);
			if (m_bmpLeft && m_bmpLeft->m_hObject)
			{
				ldc.SelectObject(m_bmpLeft);
				dc.BitBlt(0, 3, m_bmLWidth, m_bmHeight+3, &ldc, 0, 0, SRCCOPY);
			}
			cdc.CreateCompatibleDC(&dc);
			if (m_bmpMiddle && m_bmpMiddle->m_hObject)
			{
				cdc.SelectObject(m_bmpMiddle);
				dc.StretchBlt(m_bmLWidth, 3, rc.right-(m_bmRWidth+m_bmLWidth), m_bmHeight, &cdc, 0, 0, m_bmMWidth, m_bmHeight, SRCCOPY);
			}
		}
		

	}
	else 
	{
		dc.FillSolidRect(rc, CNPalette::GetIndexColor(CLR_TOOL_BK));
	}

	return;
	if (!m_bSDI)
	{
		CPen	pen(PS_SOLID, 1, RGB(255, 255, 255));
		CPen	*pOldPen = dc.SelectObject(&pen);
		dc.MoveTo(0, 0);
		dc.LineTo(rc.right, 0);
		
		dc.SelectObject(pOldPen);
	}
	//**move_Ctrl();
}

void CSVband::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
}

void CSVband::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);
}

void CSVband::change_Skin(CString skinName)
{
	CString	theme = CNPalette::GetTheme();

	if (m_pChange)
	{
		//m_pChange->set_Image(CAxButtonName(Format("mb_change%s_%s", (m_type == TTYPE1 ? "": "2"), theme)));
		m_pChange->set_Image(CAxButtonName(Format("change%s_%s", (m_type == TTYPE1 ? "": "2"), theme)));
	}

	Invalidate();
}

void CSVband::set_Background(COLORREF color)
{
	m_bk = color;
	Invalidate(FALSE);
}

CSize CSVband::GetBandWidth()
{
	/**
	if (m_type == TTYPE1)
	{
		switch (m_mode)
		{
		default:
		case 0:		return CSize(175, 19);
		case 1:		return CSize(130, 19);
		case 2:		return CSize(20, 19);
		}
	}
	else
	{
		switch (m_mode)
		{
		default:
		case 0:		return CSize(134, 19);
		case 1:		return CSize(80, 19);
		case 2:		return CSize(20, 19);
		}
	}
	**/

	return GetWidth();
}

int CSVband::GetBandMode()
{
	return m_mode;
}

void CSVband::loadPalette()
{
}

void CSVband::draw_Seperate(CDC* pDC, CRect rc)
{
	CRect	sRc, cRc;

	GetClientRect(cRc);
	sRc.SetRect(rc.left+1, cRc.top + BGAP, rc.left+3, cRc.bottom - BGAP);
	//pDC->Draw3dRect(sRc, GRIP_RB, GRIP_LT);
}

void CSVband::make_Ctrl(int mode)
{
	m_mode = mode;

	CString		imgC, imgO, imgD, text, theme = CNPalette::GetTheme();
	const DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

#ifdef DF_USESTL_SVBAND
	m_pChange = std::make_unique<CNButton>(CTRL_CHANGE);
	if (!m_pChange->Create(NULL, "버튼 숨기기", dwStyle, CRect(0, 0, 0, 0), this, -1))
		m_pChange = NULL;
	else
	{
		m_pChange->set_ToolTipText("버튼 숨기기");
		m_pChange->set_Image(CAxButtonName(Format("%s_%s", "mb_change", theme)));
	}
#else
	m_pChange = new CNButton(CTRL_CHANGE);
	if (!m_pChange->Create(NULL, "버튼 숨기기", dwStyle, CRect(0, 0, 0, 0), this, -1))
	{
		delete m_pChange;
		m_pChange = NULL;
	}
	else
	{
		m_pChange->set_ToolTipText("버튼 숨기기");
		m_pChange->set_Image(CAxButtonName(Format("%s_%s", "mb_change", theme)));
	}
#endif
	

}

void CSVband::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

}

void CSVband::move_Ctrl()
{
	if (!m_pChange)	return;

	m_pChange->ShowWindow(SW_HIDE);
	return;

	CRect rc, cRc;
	const CRect cRc2;
	GetClientRect(rc);

	if (m_type == TTYPE1)
	{
		const int cx = 5, cy = 6;
		switch (m_mode)
		{
		case 0:		// Full mode
			{
				cRc.SetRect(rc.left + cx, rc.top + cy, rc.left + cx + 17, rc.top + cy + 22);
				m_pChange->MoveWindow(cRc);
				m_pChange->ShowWindow(SW_SHOW);
				
				cRc.SetRect(cRc.right + 6, cRc.top + 1, cRc.right + 6 + 37, cRc.top + 1 + 20);

			}
			break;
		case 1:		// 1개 단축
			{
				cRc.SetRect(rc.left + cx, rc.top + cy, rc.left + cx + 17, rc.top + cy + 22);
				m_pChange->MoveWindow(cRc);
				m_pChange->ShowWindow(SW_SHOW);
				
			}
			break;
		case 2:		// 2개 단축
			{
				cRc.SetRect(rc.left + cx, rc.top + cy, rc.left + cx + 17, rc.top + cy + 22);
				m_pChange->MoveWindow(cRc);
			}
			break;
		}
	}
	else
	{
		const int cx = 5, cy = 6;
		switch (m_mode)
		{
		case 0:		// Full mode
			{
				cRc.SetRect(rc.left + cx, rc.top + cy, rc.left + cx + 17, rc.top + cy + 33);
				m_pChange->MoveWindow(cRc);
				m_pChange->ShowWindow(SW_SHOW);
				
				cRc.SetRect(cRc.right + 6, cRc.top, cRc.right + 6 + 37, cRc.top + 32);
			}
			break;
		case 1:		// 1개 단축
			{
				cRc.SetRect(rc.left + cx, rc.top + cy, rc.left + cx + 17, rc.top + cy + 33);
				m_pChange->MoveWindow(cRc);
			}
			break;
		case 2:		// 2개 단축
			{
				cRc.SetRect(rc.left + cx, rc.top + cy, rc.left + cx + 17, rc.top + cy + 33);
				m_pChange->MoveWindow(cRc);
				
			}
			break;
		}
	}
}

LONG CSVband::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	CString imgC, imgO, imgD;
	switch (LOWORD(wParam))
	{
	case CTRL_CHANGE:	
		m_mode++;
		if (m_mode > 2)		m_mode = 0;
		move_Ctrl();
		GetParent()->SendMessage(WM_XMSG, CTRL_CHANGE, 0);
		break;
	case CTRL_ENVIRONMENT:	// 설정 
	case CTRL_SCREENPRINT:	// 프린트
	case CTRL_HOMEPAGE:	// 홈페이지
	case CTRL_LOCK:		// 화면잠금
		(*m_axiscall)(AXI_FUNCKEY, wParam, lParam);
		break;
	default:
		break;
	}
	return 0;
}

void CSVband::SetBandType(int type)
{
	//m_type = type;
	m_type = TTYPE1;
	
	move_Ctrl();

	change_Skin(CNPalette::GetTheme());
}

int CSVband::GetWidth()
{
	if (m_bmLWidth)
	{
		return m_bmLWidth + BGAP * 2;
	}
	else
	{
		return 0;
	}
}

void CSVband::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDblClk(nFlags, point);

	CProfile profile(pkAxis);

	CString url(profile.GetString("WebLink", "ToolbarLink"));
	if (!url.IsEmpty())
	{
		ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOWNORMAL);	
	}
}

