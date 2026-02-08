// TMenu.cpp : implementation file
//

#include "stdafx.h"
#include "TMenu.h"
#include "iMenu.h"

#include "../../axis/axMsg.hxx"

#define	BKCOLOR		RGB(230, 235, 215)// RGB(224, 223, 228) // 20070129

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTMenu

CTMenu::CTMenu(CWnd* parent, int height)
{
	m_parent = parent;
	m_height = height + GetSystemMetrics(SM_CYBORDER)*2;
	m_iMenu = NULL;
}

CTMenu::~CTMenu()
{
#ifndef DF_USESTL_TMENU
	if (m_iMenu)	delete m_iMenu;
#endif
}


BEGIN_MESSAGE_MAP(CTMenu, CDialogBar)
	//{{AFX_MSG_MAP(CTMenu)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMenu message handlers
CSize CTMenu::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect	rect;
	rect.SetRectEmpty();
	CalcInsideRect(rect, bHorz);

	CRect	client;
	GetClientRect(client);

	CSize	size;
	CRect	fRc;
//	size.cx = 32767;

	size.cy = m_height;
	size.cy += (rect.Height());

	if (m_iMenu && GetMenuMode() == MM_SMALL)
		size.cy = m_iMenu->GetSmallHeight();

	if (IsFloating())
	{
		GetDockingFrame()->GetParent()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}
	else
	{
		GetDockingFrame()->GetWindowRect(fRc);
		size.cx = fRc.Width();
	}

	return size;
}

void CTMenu::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
	CCmdUI	state;

	state.m_pOther = this;
	state.m_nIndexMax = 1;
	state.m_nID = AFX_IDW_STATUS_BAR;

	if (CControlBar::OnCmdMsg(state.m_nID, CN_UPDATE_COMMAND_UI, &state, NULL))
		return;
	state.DoUpdate(pTarget, FALSE);
}

void CTMenu::Init(int cmdBase)
{
#ifdef DF_USESTL_TMENU
	m_iMenu = std::make_unique<CIMenu>(m_parent, cmdBase);
	if (!m_iMenu->Create(NULL, "", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, (UINT)m_iMenu.get()))
		m_iMenu = NULL;
#else
	m_iMenu = new CIMenu(m_parent, cmdBase);
	if (!m_iMenu->Create(NULL, "", WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, (UINT)m_iMenu))
	{
		delete m_iMenu;
		m_iMenu = NULL;
	}
#endif

	
}

BOOL CTMenu::InitMenu(UINT nResource, int cmdBase)
{
	if (m_iMenu)	return FALSE;

	Init(cmdBase);
	m_iMenu->LoadFramemenu(nResource);
	m_iMenu->LoadMenufromfile(); 
	fitMenu();
	return TRUE;
}

BOOL CTMenu::ReloadMenu(UINT nResource, int cmdBase)
{
#ifdef DF_USESTL_TMENU
	if (!m_iMenu)	return FALSE;
	m_iMenu = NULL;
#else
	if (!m_iMenu)	return FALSE;
	delete	m_iMenu;
	m_iMenu = NULL;
#endif

	InitMenu(nResource, cmdBase);
	fitMenu();
	return TRUE;
}

//test AI
//void CTMenu::AddMenu(CString smenu)
//{
//	m_iMenu->AddMenu(smenu);
//	fitMenu();
//}

BOOL CTMenu::PreTranslateMsg(MSG* pMsg)
{
	if (m_iMenu && m_iMenu->PreTranslateMsg(pMsg))
		return TRUE;
	return FALSE;
}

int  CTMenu::GetMCount()			{ return m_iMenu->GetMCount(); }
BOOL CTMenu::ExistMenu(CString mapN)	{ return m_iMenu->ExistMenu(mapN); }
BOOL CTMenu::IsWeb(CString mapN)		{ return FALSE; } //***m_iMenu->IsWeb(mapN); }
BOOL CTMenu::IsMappinged(CString mapN)	{ return m_iMenu->IsMappinged(mapN); }
BOOL CTMenu::GetSelectList(CString input, CStringArray& array)
					{ return m_iMenu->GetSelectList(input, array); }
CString	CTMenu::GetURL(CString mapN)	{ return m_iMenu->GetURL(mapN); }
CString	CTMenu::GetSName(CString mapN)	{ return m_iMenu->GetSName(mapN); }
CString	CTMenu::GetDesc(CString mapN)	{ return m_iMenu->GetDesc(mapN); }
CString CTMenu::GetMenuName(CString mapN)
{
	return m_iMenu->GetMenuName(mapN);
}
CString	CTMenu::GetInfo(CString mapN)	{ return m_iMenu->GetInfo(mapN); }
CString	CTMenu::GetMap(int index)	{ return m_iMenu->GetMap(index); }
CString	CTMenu::GetMap(CString dispN)	{ return m_iMenu->GetMap(dispN); }
CString	CTMenu::GetMapping(CString mapN){ 
	
	return m_iMenu->GetMapping(mapN); 
}
CString	CTMenu::GetDispN(CString mapN)	{ return m_iMenu->GetDispN(mapN); }

CMenu* CTMenu::GetPopupMenu(int index)	{ return m_iMenu->GetPopupMenu(index); }
CMenu* CTMenu::GetNewMenu()		{ return m_iMenu->GetNewMenu(); }

void CTMenu::fitMenu() 
{ 
	if (!m_iMenu)	return;

	CRect	cRc;
	GetClientRect(cRc);
	m_iMenu->MoveWindow(cRc);
}

void CTMenu::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);

	if (m_iMenu)
	{
		CRect	cRc;

		GetClientRect(cRc);
		m_iMenu->MoveWindow(cRc);
	}
}

void CTMenu::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect	cRc;
	CDC	memDC;
	//BITMAP	bm;
	GetClientRect(cRc);
//	cRc.DeflateRect(0, 1);

}

BOOL CTMenu::IsvalidMap(CString mapN)
{
	if (!m_iMenu)	return FALSE;
	return m_iMenu->ExistMenu(mapN);
}

void CTMenu::MeasureMenuItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if (m_iMenu)
		m_iMenu->MeasureMenuItem(nIDCtl,lpMeasureItemStruct); 
//TRACE("	OnMeasureItem.....\n");
//	CDialogBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CTMenu::ShowFullMenu(CPoint pt)
{
	if (m_iMenu)
		m_iMenu->ShowFullMenu(pt);
}

void CTMenu::DrawGradient(CDC *pDC, CRect drawRC, COLORREF sColor, COLORREF eColor, bool bRight)
{
	CRect	rect;
	int	nWidth{}, nHeight{};

	drawRC.right += 1;
	drawRC.bottom += 1;
	nWidth = drawRC.Width(); 
	nHeight = drawRC.Height();

	int rs{}, gs{}, bs{};
	rs = GetRValue(sColor);
	gs = GetGValue(sColor);
	bs = GetBValue(sColor);

	int re{}, ge{}, be{};
	re = GetRValue(eColor);
	ge = GetGValue(eColor);
	be = GetBValue(eColor);

	float	rStep{}, gStep{}, bStep{};
	CPen* sPen{};
	
	if (bRight)
	{
		if (!nWidth)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nWidth);
		gStep = float(gs - ge)/float(nWidth);
		bStep = float(bs - be)/float(nWidth);

		if (!nHeight)
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left + ii, drawRC.top);
				pDC->LineTo(drawRC.left + ii + 1, drawRC.top);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nWidth; ++ii)
			{
				rect.SetRect(drawRC.left + ii, drawRC.top, drawRC.left + ii + 1, drawRC.bottom);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
	else
	{
		if (!nHeight)
		{
			CPen	cPen(PS_SOLID, 1, sColor);
			sPen = pDC->SelectObject(&cPen);
			
			pDC->MoveTo(drawRC.left, drawRC.top);
			pDC->LineTo(drawRC.right, drawRC.bottom);
			pDC->SelectObject(sPen);
			return;
		}

		rStep = float(rs - re)/float(nHeight);
		gStep = float(gs - ge)/float(nHeight);
		bStep = float(bs - be)/float(nHeight);

		if (!nWidth)
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				CPen	cPen(PS_SOLID, 1, RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				sPen = pDC->SelectObject(&cPen);

				pDC->MoveTo(drawRC.left, drawRC.top + ii);
				pDC->LineTo(drawRC.left, drawRC.top + ii + 1);
				pDC->SelectObject(sPen);
			}
		}
		else
		{
			for (int ii = 0; ii < nHeight; ++ii)
			{
				rect.SetRect(drawRC.left, drawRC.top + ii, drawRC.right, drawRC.top + ii + 1);
				
				CBrush brush;
				brush.CreateSolidBrush(RGB(rs - rStep*ii, gs - gStep*ii, bs - bStep*ii));
				pDC->FillRect(&rect, &brush);
				brush.DeleteObject();
			}
		}
	}
}


int CTMenu::GetMenuMode()
{
	if (m_iMenu)
		return m_iMenu->GetMenuMode();
	else
		return MM_NORMAL;
}

int CTMenu::GetHeight()
{
	return m_height;
}