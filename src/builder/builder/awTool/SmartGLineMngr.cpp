
#include "stdafx.h"
#include "../ChildView.h"
#include "../h/mapform.h"
#include "awObjectLoad.h"
#include "SmartGLineMngr.h"
#include "SmartGLine.h"

CSmartGLineMngr::CSmartGLineMngr()
{
}

CSmartGLineMngr::CSmartGLineMngr(CChildView* view)
{
	m_pSGLineLeft = NULL;
	m_pSGLineTop = NULL;
	m_pSGLineRight = NULL;
	m_pSGLineBottom = NULL;

	m_iLeft = 0;
	m_iTop = 0;
	m_iRight = 0;
	m_iBottom = 0;
	m_view = view;
	Init();
}

CSmartGLineMngr::~CSmartGLineMngr()
{
	DelAllGLine();
}

void CSmartGLineMngr::CheckSmartGLine(int Sel, CRect rc, CRect ScreenRc)
{
	//DelAllGLine();
	CformProp* Prop = NULL;
	for (int ii = 0; ii < m_view->m_mapH->forms.GetCount(); ii++)
	{
		if (ii == Sel) continue;
		Prop = ((CObjectLoad*)m_view->m_mapH->forms.GetAt(ii))->getProp();
		if (m_pSGLineLeft == NULL) drawLineWnd(&m_pSGLineLeft, LINE_X, rc.left, Prop->rect.left, ScreenRc.left);
		if (m_pSGLineLeft == NULL) drawLineWnd(&m_pSGLineLeft, LINE_X, rc.left, Prop->rect.right, ScreenRc.left);
		if (m_pSGLineRight == NULL) drawLineWnd(&m_pSGLineRight, LINE_X, rc.right, Prop->rect.left, ScreenRc.right);
		if (m_pSGLineRight == NULL) drawLineWnd(&m_pSGLineRight, LINE_X, rc.right, Prop->rect.right, ScreenRc.right);

		if (m_pSGLineTop == NULL) drawLineWnd(&m_pSGLineTop, LINE_Y, rc.top, Prop->rect.top, ScreenRc.top);
		if (m_pSGLineTop == NULL) drawLineWnd(&m_pSGLineTop, LINE_Y, rc.top, Prop->rect.bottom, ScreenRc.top);
		if (m_pSGLineBottom == NULL) drawLineWnd(&m_pSGLineBottom, LINE_Y, rc.bottom, Prop->rect.top, ScreenRc.bottom);
		if (m_pSGLineBottom == NULL) drawLineWnd(&m_pSGLineBottom, LINE_Y, rc.bottom, Prop->rect.bottom, ScreenRc.bottom);
	}
}

void CSmartGLineMngr::drawLineWnd(CSmartGLine** pSGLine, int lineXY, int DrawPos, int ObjPos, int WndPos)
{
	if (DrawPos == ObjPos)
	{
		CRect rcc, rcx;
		m_view->GetClientRect(rcc);
		m_view->ClientToScreen(rcc);

		if (lineXY == LINE_X)
		{
			rcc.left = WndPos;
			rcc.right = rcc.left + 1;
		}
		else
		{
			rcc.top = WndPos;
			rcc.bottom = rcc.top + 1;
		}

		*pSGLine = new CSmartGLine(m_view);
		(*pSGLine)->CreateEx(WS_EX_LAYERED, AfxRegisterWndClass(0, LoadCursor(0, IDC_ARROW), (HBRUSH)GetStockObject(WHITE_BRUSH)),
			NULL, WS_POPUP, rcc, m_view->GetParent(), NULL);
		::SetLayeredWindowAttributes((*pSGLine)->m_hWnd, RGB(255, 255, 255), 255, LWA_ALPHA | LWA_COLORKEY);
		(*pSGLine)->Init(1, PS_USERSTYLE, lineXY);
		(*pSGLine)->SaveRect(rcc);
		(*pSGLine)->ShowWindow(SW_SHOWNOACTIVATE);
	}
	else
	{	
		//if (*pSGLineLeft != NULL)
		//{
		//	delete (*pSGLineLeft);
		//	(*pSGLineLeft) = NULL;
		//}
	}
}

void CSmartGLineMngr::Init()
{
}

void CSmartGLineMngr::DelAllGLine()
{
	DelGLine(&m_pSGLineLeft);
	DelGLine(&m_pSGLineTop);
	DelGLine(&m_pSGLineRight);
	DelGLine(&m_pSGLineBottom);
}

void CSmartGLineMngr::DelGLine(CSmartGLine** pSGLine)
{
	if (*pSGLine)
	{
		delete (*pSGLine);
		(*pSGLine) = NULL;
	}
}

void CSmartGLineMngr::DelSGLineWnd(int left, int right, int top, int bottom)
{
	if (m_iLeft != left)
	{
		DelGLine(&m_pSGLineLeft);
		m_iLeft = left;
	}
	if (m_iRight != right)
	{
		DelGLine(&m_pSGLineRight);
		m_iRight = right;
	}
	if (m_iTop != top)
	{
		DelGLine(&m_pSGLineTop);
		m_iTop = top;
	}
	if (m_iBottom != bottom)
	{
		DelGLine(&m_pSGLineBottom);
		m_iBottom = bottom;
	}
}